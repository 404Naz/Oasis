//
// Created by Andrew Nazareth on 11/5/24.
//

#include "catch2/catch_test_macros.hpp"

#include "Oasis/Add.hpp"
#include "Oasis/Exponent.hpp"
#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
#include "Oasis/RecursiveCast.hpp"
#include "Oasis/Variable.hpp"
#include "Oasis/Real.hpp"
#include "vector"
#include "Oasis/SolveODE.hpp"
#include "Oasis/EulerNumber.hpp"

TEST_CASE("Test First Order Homogenous", "[ODE]")
{
    Oasis::Variable x{"x"};

    std::vector<std::unique_ptr<Oasis::Expression>> eq;
    eq.emplace_back(std::make_unique<Oasis::Real>(5.0));
    eq.emplace_back(std::make_unique<Oasis::Real>(-2.0));

    auto ans = Oasis::SolveHomogenousODE(eq, x);

    auto expected = Oasis::Multiply{Oasis::Variable{"c_0"},
        Oasis::Exponent{Oasis::EulerNumber{}, Oasis::Multiply{Oasis::Real{-5.0/-2.0}, x}}}.Generalize();

    REQUIRE(ans->Equals(*expected));
}