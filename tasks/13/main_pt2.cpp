#include <iostream>
#include "solver.hpp"

int main()
{
    //runAllTests();
    int answer = Solver::solveMain2("data.dat");
    std::cout << "Answer is: " << answer << "\n";
    return 0;
}
