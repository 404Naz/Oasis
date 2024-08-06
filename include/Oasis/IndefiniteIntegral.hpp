//
// Created by Levy Lin on 2/09/2024.
//

#ifndef OASIS_INTEGRATE_HPP
#define OASIS_INTEGRATE_HPP

#include "BinaryExpression.hpp"
#include "Expression.hpp"
#include "Real.hpp"

namespace Oasis {

template <IExpression Integrand, IExpression Differential>
class IndefiniteIntegral;

/// @cond
template <>
class IndefiniteIntegral<Expression, Expression> : public BinaryExpression<IndefiniteIntegral> {
public:
    IndefiniteIntegral() = default;
    IndefiniteIntegral(const IndefiniteIntegral<Expression, Expression>& other) = default;

    IndefiniteIntegral(const Expression& integrand, const Expression& differential);

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final;
    [[nodiscard]] auto Simplify(const Expression& upper, const Expression& lower) const -> std::unique_ptr<Expression> /* final */;

    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final;

    static auto Specialize(const Expression& other) -> std::unique_ptr<IndefiniteIntegral>;
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<IndefiniteIntegral>;

    EXPRESSION_TYPE(Integral)
    EXPRESSION_CATEGORY(Associative | Commutative)
};
/// @endcond

/**
 * The IndefiniteIntegral expression integrates the two expressions together.
 *
 * @tparam IntegrandT The type of the expression to be integrated.
 * @tparam DifferentialT The type of the expression defining which variable is integrating in the IntegrandT.
 */
template <IExpression IntegrandT = Expression, IExpression DifferentialT = IntegrandT>
class IndefiniteIntegral : public BinaryExpression<IndefiniteIntegral, IntegrandT, DifferentialT> {
public:
    IndefiniteIntegral() = default;
    IndefiniteIntegral(const IndefiniteIntegral<IntegrandT, DifferentialT>& other)
        : BinaryExpression<IndefiniteIntegral, IntegrandT, DifferentialT>(other)
    {
    }

    IndefiniteIntegral(const IntegrandT& integrand, const DifferentialT& differential)
        : BinaryExpression<IndefiniteIntegral, IntegrandT, DifferentialT>(integrand, differential)
    {
    }

    IMPL_SPECIALIZE(IndefiniteIntegral, IntegrandT, DifferentialT)

    auto operator=(const IndefiniteIntegral& other) -> IndefiniteIntegral& = default;

    EXPRESSION_TYPE(Integral)
    EXPRESSION_CATEGORY(Associative | Commutative)
};

} // namespace Oasis

#endif // OASIS_INTEGRATE_HPP
