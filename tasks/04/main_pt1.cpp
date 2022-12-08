#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

bool overlaps(int a_0, int a_1, int b_0, int b_1) {
    return (a_0 <= b_0 && a_1 >= b_1) ||
	(b_0 <= a_0 && b_1 >= a_1);
}

std::vector<int> getNumbersFromAssignment(std::string const& assignment) {
    size_t delim = assignment.find('-');
    return {
	std::stoi(assignment.substr(0,delim)),
	std::stoi(assignment.substr(delim+1))};	    
}

bool assignmentsOverlap(std::string const& assignment) {
    size_t delim = assignment.find(',');
    std::string assignment_1 = assignment.substr(0,delim);
    std::string assignment_2 = assignment.substr(delim+1);

    auto vals_1 = getNumbersFromAssignment(assignment_1);
    auto vals_2 = getNumbersFromAssignment(assignment_2);
    
    return overlaps(vals_1[0], vals_1[1], vals_2[0], vals_2[1]);
}

void testAssignmentOverlaps() {
    std::unordered_map<std::string, bool> test_cases {
	{"2-4,6-8",false},
	{"2-3,4-5",false},
	{"5-7,7-9",false},
	{"2-8,3-7",true},
	{"6-6,4-6",true},
	{"2-6,4-8",false},
	{"46-97,47-96", true},
    };
    
    for (auto & test : test_cases) {
	if (assignmentsOverlap(test.first) != test.second) {
	    std::cout << "Test failed: " << test.first << " expected " << test.second <<
		" received " << assignmentsOverlap(test.first) <<"\n";
	} else {
	    std::cout << "Test passed: " << test.first <<"\n";
	}
    }    
    }



int main()
{
    testAssignmentOverlaps();
    std::ifstream infile = std::ifstream("data.dat");
    std::string line;
    int total = 0;
    while (infile.good()) {
	getline(infile, line, '\n');
	if (assignmentsOverlap(line)) {
	    total+=1;
	} 
    }
    
    std::cout << "Total score: " << total << "\n";

    return 0;
}
