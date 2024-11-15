//
// Created by Andrew Nazareth on 10/29/24.
//

#ifndef OASIS_SOLVEODE_HPP
#define OASIS_SOLVEODE_HPP

#include <vector>
#include "Expression.hpp"
#include "Variable.hpp"

namespace Oasis{
/**
 * This method solves for the homogenous solution to the provided list of terms for the differential equation.
 * This method is tested for real first and second order equations.
 * @param terms Vector of Expression unique pointers representing the homogenous differential equation.
 *              If the input equation is &alpha*y'' + &beta*y' +&gamma*y = 0, terms[0] is &gamma - the constant term,
 *              terms[1] is &beta - the first derivative, terms[2] is &alpha - the second derivative, etc.
 * @param DiffVar Variable of Differentiation
 * @return Solution to equation. This will take on the form e^(&lambda*t) for differentiation variable t.
 */
auto SolveHomogenousODE(std::vector<std::unique_ptr<Expression>>& terms, Variable& DiffVar) -> std::unique_ptr<Expression>;

/**
 * Solves for the particular solution of the differential equation.
 * @param terms Vector of Expression unique pointers representing the differential equation.
*               If the input equation is &alpha*y'' + &beta*y' +&gamma*y = 0, terms[0] is &gamma - the constant term,
*               terms[1] is &beta - the first derivative, terms[2] is &alpha - the second derivative, etc.
 * @param function Vector of terms of the particular equation that is being solved for.
 * @param DiffVar Variable of Differentiation.
 * @return Particular solution to the equation for the function. This will resemble the input function in function.
 */
auto SolveParticularODE(std::vector<std::unique_ptr<Expression>>& terms,
    std::unique_ptr<Expression>& equivalent, Variable& DiffVar) -> std::unique_ptr<Expression>;

/**
 * Solves for the homogenous and particular solution of the differential equation.
 * @param differentialEquation Equation of the differential equation. The solution function
 *                             is assuming
 * @param functionEquation
 * @param SolVar Variable representing the solution in the differential equation.
 * @param DiffVar Variable of Differentiation
 * @return
 */
auto SolveLinearODE(std::unique_ptr<Expression>& differentialEquation,
    std::unique_ptr<Expression>& functionEquation, Variable& SolVar, Variable& DiffVar) -> std::unique_ptr<Expression>;
}

#endif // OASIS_SOLVEODE_HPP
