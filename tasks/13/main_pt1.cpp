#include <iostream>
#include "solver.hpp"
#include "tests.hpp"
using namespace Tests;

void runAllTests() {
    testFindNextPositions();
    testStripString();
    testCommaFromTestString();
    testGetPacket();
    testLines();
}

int main()
{
    //runAllTests();
    int answer = Solver::solveMain("data.dat");
    std::cout << "Answer is: " << answer << "\n";
    return 0;
}
