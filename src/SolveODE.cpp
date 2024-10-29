//
// Created by Andrew Nazareth on 10/29/24.
//

#include "Oasis/SolveODE.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Expression.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Variable.hpp"
#include "Oasis/EulerNumber.hpp"
#include "Oasis/Negate.hpp"

namespace Oasis{
auto SolveHomogenousODE(std::vector<std::unique_ptr<Expression>>& terms) -> std::unique_ptr<Expression>{
    std::vector<std::unique_ptr<Expression>> vals;

    // First Order
    if (terms.size() == 2){
        return Exponent{EulerNumber{}, Multiply{*(Negate<Expression>{*terms[0]}.Simplify()), Variable{"t"}}}.Generalize();
    }

    vals.reserve(terms.size());
    for (int i = 0; i < terms.size(); i++) {
        vals.push_back(Multiply<Expression> { *terms[i], Exponent { Variable { "r" }, Real { static_cast<double>(i) } } }.Generalize());
    }

    return Exponent{EulerNumber{}, Multiply{Real{1}, Variable{"t"}}}.Generalize();
}
}