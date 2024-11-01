//
// Created by bachia on 4/12/2024.
//

#ifndef OASIS_DERIVATIVE_HPP
#define OASIS_DERIVATIVE_HPP

#include "BinaryExpression.hpp"
#include "Exponent.hpp"
#include "Real.hpp"

namespace Oasis {

template <IExpression Exp, IExpression Var>
class Derivative;

/// @cond
template <>
class Derivative<Expression, Expression> : public BinaryExpression<Derivative> {
public:
    Derivative()
        : BinaryExpression(), degree(1)
    {
    }
//    Derivative(const Derivative<Expression, Expression>& other) = default;
    Derivative(const Derivative<Expression, Expression>& other) = default;

    // Derivative(const Expression& Exp, const Expression& Var); OLD
    Derivative(const Expression& Exp, const Expression& Var, int deg = 1)
        : BinaryExpression(Exp, Var), degree(deg)
    {
    }

    int GetDegree();
    void SetDegree(int deg);

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final;

    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> override;

    EXPRESSION_TYPE(Derivative)
    EXPRESSION_CATEGORY(BinExp)

private:
    int degree;
};
/// @endcond

/**
 * The Derivative class template calculates the derivative of given expressions.
 *
 * @tparam DependentT The expression type that the derivative will be calculated of.
 * @tparam IndependentT The type of the variable with respect to which the derivative will be calculated.
 */
template <IExpression DependentT = Expression, IExpression IndependentT = DependentT>
class Derivative : public BinaryExpression<Derivative, DependentT, IndependentT> {
public:
    Derivative() = default;
    Derivative(const Derivative<DependentT, IndependentT>& other)
        : BinaryExpression<Derivative, DependentT, IndependentT>(other)
        , degree(other.degree)
    {
    }

    Derivative(const DependentT& exp, const IndependentT& var, int deg = 1)
        : BinaryExpression<Oasis::Derivative, DependentT, IndependentT>(exp, var)
        , degree(deg)
    {
    }

    auto operator=(const Derivative& other) -> Derivative& = default;
    int GetDegree()
    {
        return degree;
    }
    void SetDegree(int deg)
    {
        degree = deg;
    }

    EXPRESSION_TYPE(Derivative)
    EXPRESSION_CATEGORY(BinExp)
private:
    int degree;
};

} // namespace Oasis

#endif // OASIS_DERIVATIVE_HPP
