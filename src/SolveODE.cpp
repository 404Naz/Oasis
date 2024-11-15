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

auto SolveParticularODE(std::vector<std::unique_ptr<Expression>>& terms, std::unique_ptr<Expression>& equivalent,
    Variable& DiffVar) -> std::unique_ptr<Expression>
{
    // Sinusoid equivalent

    // Exponential Equivalent

    // Polynomial Equivalent


    return std::unique_ptr<Expression>{};
}

auto SolveLinearODE(std::unique_ptr<Add<Expression>>& differentialEquation, std::unique_ptr<Expression>& functionEquation,
    Variable& SolVar, Variable& DiffVar) -> std::unique_ptr<Expression>
{
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

    auto homogenous = SolveHomogenousODE(terms, DiffVar);
    auto particular = SolveParticularODE(terms, functionEquation, DiffVar);

    return Add{*homogenous, *particular}.Generalize();
}

}