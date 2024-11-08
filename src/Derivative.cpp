//
// Created by bachia on 4/12/2024.
//

#include "Oasis/Derivative.hpp"
#include "Oasis/Add.hpp"
#include "Oasis/Divide.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Expression.hpp"
#include "Oasis/Log.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/Subtract.hpp"
#include "Oasis/Undefined.hpp"

#include <cmath>
#include <string>

namespace Oasis {
auto Derivative<Expression>::Simplify() const -> std::unique_ptr<Expression>
{
    auto simplifiedExpression = mostSigOp ? mostSigOp->Simplify() : nullptr;
    auto simplifiedVar = leastSigOp ? leastSigOp->Simplify() : nullptr;

    if (simplifiedExpression == nullptr || simplifiedVar == nullptr){
        return this->Copy();
    }

    if (this->degree != 1){
        auto diffExp = simplifiedExpression->Copy();
        for (int i = 0; i < degree; i++){
            auto nextDiffExp = diffExp->Differentiate(*simplifiedVar);
            if (!nextDiffExp) {
                return this->Copy();
            }
            diffExp = std::move(nextDiffExp);
        }
        return diffExp->Generalize();
    }
    return simplifiedExpression->Differentiate(*simplifiedVar);
}

std::unique_ptr<Expression> Derivative<Expression, Expression>::Differentiate(const Expression&) const
{
    return mostSigOp->Differentiate(*leastSigOp);
}

}
