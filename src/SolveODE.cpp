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
#include <string>

namespace Oasis{
auto SolveHomogenousODE(std::vector<std::unique_ptr<Expression>>& terms, Variable& DiffVar) -> std::unique_ptr<Expression>{
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
}