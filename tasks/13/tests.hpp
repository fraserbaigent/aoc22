#ifndef TESTS_HPP
#define TESTS_HPP
#include "solver.hpp"
using namespace Solver;

namespace Tests {

void printTest(std::string const& test_name) {
    static const std::string dash_line(80, '-');
    std::cout << dash_line << "\nTEST: " << test_name << "\n" << dash_line << "\n";
}
    
void testFindNextPositions() {
    printTest("NEXT POSITIONS");
    std::cout << "Running tests for next positions:\n";
    std::vector<std::string> test_cases {
	"1,2,3",
	"[1,2,3]",
	"[1,2,3],4,5",
	"1,2,[3,4,5]",
	"1,2,[3,4],5",
	"1,2,[],4,[5,6]",
	"1,2,[],4,[5,6]",
	"1,2,[],4,5,6",
	"[[1,2,3,4]]",
	"[[1,2,3,4]]",	
    };

    std::vector<std::pair<int,int>> answers {
	{-1,-1},
	{0,6},
	{0,6},
	{4,10},
	{4,8},
	{4,5},
	{9,13},
	{-1,-1},
	{0,10},
	{1,9},
    };

    std::vector<int> offsets {
	0,0,0,0,0,0,5,5,0,1,
    };
    for (int i = 0; i < test_cases.size(); ++i) {
	auto res = findNextSquareBraces(test_cases[i], offsets[i]);
	if (res != answers[i]) {
	    std::cout << "Failed with: (" << res.first << ", " << res.second << ") not equalling (" <<
		answers[i].first << ", " << answers[i].second << ") from " << test_cases[i] << " with offset " << offsets[i] <<"\n";
	} else {
	    std::cout << "Passed test: " << test_cases[i]<< " with offset " << offsets[i] << "\n";
	}
    }    
}

void testStripString() {
    printTest("STRIP STRING");
    std::vector<std::pair<int,int>> indices {
	{0,3},
	{1,3},
	{3,5},
	{4,5},
	{0,9},
    };

    auto base_string = "0123456789";
    std::vector<std::string> test_cases {
	"12",
	"2",
	"4",
	"",
	"12345678",
    };
    for (int i = 0; i < test_cases.size(); ++i) {
	auto answer = strippedArrayStringFrom(base_string, indices[i]);
	if (answer != test_cases[i]) {
	    std::cout << "Failed with: (" << indices[i].first << ", " << indices[i].second << ") - " <<
		test_cases[i] << " received " << answer <<"\n";
	} else {
	    std::cout << "Passed test: " << test_cases[i]<< " - (" << indices[i].first << ", " << indices[i].second << ")\n";
	}
    }    
}

void testCommaFromTestString() {
    printTest("LIST FROM CSV");
    std::vector<std::string> test_cases {
	"1,2,3,4,5",
	"0,20,30,40,50",
    };

    std::vector<std::vector<int>> results {
	{1,2,3,4,5},
	{0,20,30,40,50},
    };

    for (int i = 0; i < test_cases.size(); ++i) {
	auto res = getValuesFromCommaListString(test_cases[i]);
	if (res != results[i]) {
	    std::cout << "Failed test with " << test_cases[i] << " - Received ";
	    for (auto v : res) {
		std::cout << v << ",";
	    }
	    std::cout << std::endl;
	} else {
	    std::cout << "Passed test: " << test_cases[i] << "\n";
	}
    }
}
void testGetPacket() {
    printTest("PACKET FROM LINE");
    std::vector<std::pair<std::string, std::string>> test_lines {
	{"[1,1,3,1,1]",
	 "[1,1,5,1,1]"},
	{"[[1],[2,3,4]]",
	 "[[1],4]"},
	{"[9]",
	 "[[8,7,6]]"},
	{"[[4,4],4,4]",
	 "[[4,4],4,4,4]"},
	{"[7,7,7,7]",
	 "[7,7,7]"},
	{"[]",
	 "[3]"},
	{"[[[]]]",
	 "[[]]"},
	{"[1,[2,[3,[4,[5,6,7]]]],8,9]",
	 "[1,[2,[3,[4,[5,6,0]]]],8,9]"},
    };
    std::vector<bool> results {
	true,true,false,true,false,true,false,false,
    };
    int total = 0;
    for (int i = 0; i < test_lines.size(); ++i) {
	auto const& t = test_lines[i];
	auto l = getPacket(t.first);
	auto r = getPacket(t.second);
	auto result = inCorrectOrder(l, r) == Solver::TRUE;
	if (result == results[i]) {
	    std::cout << "Passed test " << i << ".\n";
	} else {
	    std::cout << "Failed test " << i << " - Received " << result << " expected " << results[i] <<".\n";
	}
	if (result) {
	    total += i + 1;
	}
    }
    static constexpr int expected = 13;
    if (total != expected ) {
	std::cout << "Test failed - total was not correct. Received " << total << " expected " << expected << ".\n";
    } else {
	std::cout << "Test passed - total was correct.\n";
    }
}

void testLines() {
    printTest("COMPARE PACKETS");
    std::vector<std::pair<std::string, std::string>> test_lines {
	{"[[],[[[3,1,4],4,8]]]",
	 "[[2],[[0,0],[]],[8],[4,9,5,[6]]]"},
	{"[[],[[[6],[6,5,2],[6]]],[6,4,7,0]]",
	 "[[[[0,2,3],3,[8,2,1],6,2],[[],[],[4,5,4],4]],[[9,[0],8,5]],[[8,[2,0],[0,4,8],10,1],[10]]]"},
	{"[[[6]],[]]",
	 "[[8,[],7,10],[],[10,[[0,8,5,8,10]],6,1,[[9],[5,2,7,1],[3,1]]],[[[6,5],[]]],[0,5]]"},
	{"[[0],[[[2,2],3,[3,3,5,2,3],10],8,[3,[10,0,5,5,9],[8,9,2],[1,6],1],[6,[3,3,10,6],[8,6,8,2]]],[],[[],[[6,8],[5,6,10],9,[],5],9,10,0],[[9,[10,2,8,2],[8,1,1,1],[]],[8,[1,10,8,3],[1,5]],2,[10,[3,9,7],[],10,5],[5,9,3,1]]]",
	 "[0,0,7,3]"},
    };
     std::vector<bool> results {
	 true,
	 true,
	 true,
	 false
    };
    int total = 0;
    int expected = 0;
    for (int i = 0; i < test_lines.size(); ++i) {
	auto const& t = test_lines[i];
	auto result = inCorrectOrder(t.first, t.second);
	if (result == results[i]) {
	    std::cout << "Passed test " << i << "\n";
	} else {
	    std::cout << "Failed test " << i << ". Expected " << results[i] << " but received " << result << "\n";
	}
	if (result) {
	    total += i + 1;
	}
	if (results[i]) {
	    expected += i + 1;
	}
    }
    if (total != expected) {
	std::cout << "Test failed - total was not correct.\n";
    } else {
	std::cout << "Test passed - total was correct.\n";
    }

    exit(1);
}

};

#endif
