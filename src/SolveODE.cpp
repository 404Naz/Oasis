//
// Created by Andrew Nazareth on 10/29/24.
//

#include "Oasis/SolveODE.hpp"
#include "Oasis/Add.hpp"
#include "Oasis/Divide.hpp"
#include "Oasis/EulerNumber.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Expression.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Negate.hpp"
#include "Oasis/Variable.hpp"
#include "Oasis/Derivative.hpp"
#include "Oasis/Negate.hpp"
#include <string>

namespace Oasis{
auto SolveHomogenousODE(std::vector<std::unique_ptr<Expression>>& terms, Variable& DiffVar) -> std::unique_ptr<Expression>{
    // Zeroth Order
    if (terms.size() == 1){
        return std::make_unique<Real>(0);
    }

    // First Order
    if (terms.size() == 2){
        auto r = Divide{*(Negate<Expression>{*terms[0]}.Simplify()), *terms[1]}.Simplify();
        return Multiply{Variable{"c_0"}, Exponent{EulerNumber{}, Multiply{*r, DiffVar}}}.Generalize();
    }

    std::vector<std::unique_ptr<Expression>> vals;

    vals.reserve(terms.size());
    for (int i = 0; i < terms.size(); i++) {
        vals.push_back(Multiply<Expression> { *terms[i], Exponent { Variable { "r" }, Real { static_cast<double>(i) } } }.Generalize());
    }

    std::unique_ptr<Add<Expression>> expr = BuildFromVector<Add>(vals);
    auto zeroes = expr->FindZeros();

    std::vector<std::unique_ptr<Expression>> ans;
    ans.reserve(zeroes.size());
    for (int i = 0; i < zeroes.size(); i++){
        ans.emplace_back(std::make_unique<Multiply<Expression>>(Variable{std::string("c_").append(std::to_string(i))},
            Exponent<Expression>{EulerNumber{}, Multiply{*(zeroes[i]->Copy()->Simplify()), *DiffVar.Copy()}}));
    }

    auto eq = BuildFromVector<Add>(ans);
    return eq;
}

auto SolveParticularODE(std::vector<std::unique_ptr<Expression>>& terms, std::vector<std::unique_ptr<Expression>>& equivalent,
    Variable& DiffVar) -> std::unique_ptr<Expression>
{
    if (terms.size() == 1){
        if (equivalent.size() > 1){
            return BuildFromVector<Add>(equivalent);
        } else {
            return equivalent[0]->Copy();
        }
    }

    std::vector<std::unique_ptr<Expression>> guess;
    int polyDegree = 0;
    int k_n = 0;
    // for every element in equivalent
    for (int i=0; i<equivalent.size(); i++){
        // if polynomial: guess adds k_... * x^...
        if (auto exp = RecursiveCast<Exponent<Variable, Real>>(*equivalent[i]->Copy()); exp != nullptr){
            if (exp->GetLeastSigOp().GetValue() > polyDegree) polyDegree = (int)exp->GetLeastSigOp().GetValue();
            continue;
        }
        if (auto exp = RecursiveCast<Multiply<Real, Exponent<Variable, Real>>>(*equivalent[i]->Copy()); exp != nullptr){
            if (exp->GetLeastSigOp().GetLeastSigOp().GetValue() > polyDegree) polyDegree = (int)exp->GetLeastSigOp().GetLeastSigOp().GetValue();
            continue;
        }
        if (auto exp = RecursiveCast<Variable>(*equivalent[i]->Copy()); exp != nullptr){
            if (polyDegree < 1) polyDegree = 1;
            continue;
        }
        if (auto exp = RecursiveCast<Multiply<Real, Variable>>(*equivalent[i]->Copy()); exp != nullptr){
            if (polyDegree < 1) polyDegree = 1;
            continue;
        }

        // if sinusoid: guess adds k_... * sin() + k_... * cos()
        // TODO: Implement once sin and cos are integrated

        // if e^x: guess adds k_... * e^x + k_... * xe^x
        if (auto exp = RecursiveCast<Exponent<EulerNumber, Expression>>(*equivalent[i]->Copy()); exp != nullptr){
            auto ex = Multiply{Variable{std::to_string(k_n)}, *exp->GetLeastSigOp().Copy()};
            guess.push_back(ex.Generalize());
            k_n++;
            auto xex = Multiply{Variable{std::to_string(k_n)}, Multiply{*DiffVar.Copy(), *exp->GetLeastSigOp().Copy()}};
            guess.push_back(xex.Generalize());
            k_n++;
        }
        if (auto exp = RecursiveCast<Multiply<Real, Exponent<EulerNumber, Expression>>>(*equivalent[i]->Copy()); exp != nullptr){
            auto ex = Multiply{Variable{std::to_string(k_n)}, *exp->GetLeastSigOp().GetLeastSigOp().Copy()};
            guess.push_back(ex.Generalize());
            k_n++;
            auto xex = Multiply{Variable{std::to_string(k_n)}, Multiply{*DiffVar.Copy(), *exp->GetLeastSigOp().GetLeastSigOp().Copy()}};
            guess.push_back(xex.Generalize());
            k_n++;
        }
    }
    // add polynomial built from the highest polynomial power
    for (int i=0; i<polyDegree; i++){
        auto exp = Multiply{Variable{std::to_string(k_n)}, Exponent{*DiffVar.Copy(), Real{(double)i}}};
        guess.push_back(exp.Copy());
        k_n++;
    }

    auto guessExp = BuildFromVector<Add>(guess)->Simplify();

    std::vector<std::unique_ptr<Expression>> expressionVec;

    // guessExp differentiated i times multipled by coefficient
    for (int i=0; i<terms.size(); i++){
        auto diffExp = guessExp->Copy();
        for (int j = 0; j < i; j++){
            auto nextDiffExp = diffExp->Differentiate(*DiffVar.Copy());
            diffExp = std::move(nextDiffExp);
        }
        Multiply<Expression> term = Multiply<Expression>{*terms[i]->Copy(), *diffExp->Copy()};
        expressionVec.push_back(term.Generalize());
    }

    // Convert to linear and solve

    // replace variable with value and return

    return std::unique_ptr<Expression>{};
}

auto SolveLinearODE(std::unique_ptr<Add<Expression>>& differentialEquation, std::unique_ptr<Expression>& functionEquation,
    Variable& SolVar, Variable& DiffVar) -> std::unique_ptr<Expression>
{
    if (differentialEquation == nullptr || functionEquation == nullptr){
        return std::make_unique<Real>(0.0);
    }
    std::vector<std::unique_ptr<Expression>> diffeq;

    differentialEquation->Flatten(diffeq);

    std::vector<std::unique_ptr<Expression>> terms;

    terms.reserve(diffeq.size());
    for (int i=0; i<diffeq.size(); i++) {
        if (auto diff = RecursiveCast<Multiply<Expression, Derivative<Expression>>>(*diffeq[i]); diff != nullptr){
            terms[diff->GetLeastSigOp().GetDegree()] = diff->GetMostSigOp().Copy();
            continue;
        }
        if (auto diff = RecursiveCast<Derivative<Expression>>(*diffeq[i]); diff != nullptr){
            terms[diff->GetDegree()] = diff->Copy();
            continue;
        }
        if (auto ans = RecursiveCast<Multiply<Expression, Variable>>(*diffeq[i]); ans != nullptr){
            if (ans->GetLeastSigOp().Equals(SolVar)) {
                terms[0] = ans->GetMostSigOp().Copy();
                continue;
            }
        }
        if (auto ans = RecursiveCast<Variable>(*diffeq[i]); ans != nullptr){
            if (ans->Equals(SolVar)){
                terms[0] = Real{1.0}.Generalize();
                continue;
            }
        }

        // Variable or other value that doesn't fit form
        functionEquation = Add{Negate<Expression>{*diffeq[i]}, *functionEquation->Copy()}.Generalize();
    }

    std::vector<std::unique_ptr<Expression>> eqVec;
    if (auto addEq = RecursiveCast<Add<Expression>>(*functionEquation); addEq != nullptr){
        addEq->Flatten(eqVec);
    } else {
        eqVec.push_back(functionEquation->Copy());
    }

    auto homogenous = SolveHomogenousODE(terms, DiffVar);
    auto particular = SolveParticularODE(terms, eqVec, DiffVar);

    return Add{*homogenous, *particular}.Generalize();
}

}