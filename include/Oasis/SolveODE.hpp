//
// Created by Andrew Nazareth on 10/29/24.
//

#ifndef OASIS_SOLVEODE_HPP
#define OASIS_SOLVEODE_HPP

#include <vector>
#include "Expression.hpp"

namespace Oasis{
/**
 * This method solves for the homogenous solution to the provided list of terms for the differential equation.
 * @param terms Vector of Expression unique pointers representing the homogenous differential equation.
 *              If the input equation is &alpha*y'' + &beta*y' +&gamma*y = 0, terms[0] is &gamma - the constant term,
*              terms[1] is &beta - the first derivative, terms[2] is &alpha - the second derivative, etc.
 * @return Solution to equation. This will take on the form e^(&lambda*t) for differentiation variable t.
 */
static auto SolveHomogenousODE(std::vector<std::unique_ptr<Expression>>& terms) -> std::unique_ptr<Expression>;

/**
 * Solves for the particular solution of the differential equation.
 * @param terms Terms of the differential equation.
 * @param function Vector of terms in the
 * @return Particular solution to the equation for the function. This will resemble the input function in function
 */
static auto SolveParticularODE(std::vector<std::unique_ptr<Expression>>& terms,
    std::vector<std::unique_ptr<Expression>>& equivalent) -> std::unique_ptr<Expression>;

/**
 * Solves for the homogenous and particular solution of the differential equation.
 * @param differentialEquation
 * @param functionEquation
 * @return
 */
static auto SolveODE(std::unique_ptr<Expression>& differentialEquation,
    std::unique_ptr<Expression>& functionEquation) -> std::unique_ptr<Expression>;
}

#endif // OASIS_SOLVEODE_HPP
