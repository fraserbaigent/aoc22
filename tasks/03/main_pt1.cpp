#include <fstream>
#include <iostream>
#include <map>
#include <unordered_set>

int getScore(char character) {
    int value = character - 'a';
    if (value < 0) {
	return character - 'A' + 27;
    } else {
	return value + 1;
    }
}

void testCharacters() {
    std::map<char, int> test_cases {
	{'A', 27},
	{'Z', 52},
	{'a', 1},
	{'z', 26},
    };

    for (auto & test : test_cases) {
	if (getScore(test.first) != test.second) {
	    std::cout << "Test failed: " << test.first << " expected " << test.second <<
		" received " << getScore(test.first) <<"\n";
	} else {
	    std::cout << "Test passed: " << test.first <<"\n";
	}
    }    
}


int getBackpackScore(std::string const& line) {
    size_t mid = line.size() / 2;
    std::unordered_set<char> copied;
    std::unordered_set<char> seen_left;
    std::unordered_set<char> seen_right;
    for (size_t l = 0; l < mid; ++l) {
	seen_left.insert(line[l]);
	seen_right.insert(line[l + mid]);
	if (seen_left.find(line[l+mid]) != seen_left.cend()) {
	    copied.insert(line[l+mid]);
	} else if (seen_right.find(line[l]) != seen_right.cend()) {
	    copied.insert(line[l]);
	}    
    }

    int score = 0;
    for (auto c : copied) {
	score += getScore(c);
    }
    return score;
}

void testScores() {
    std::map<std::string, int> test_cases {
	{"vJrwpWtwJgWrhcsFMMfFFhFp",16},
	{"jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",38},
	{"PmmdzqPrVvPwwTWBwg",42},
	{"wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",22},
	{"ttgJtRGJQctTZtZT",20},
	{"CrZsJsPPZsGzwwsLwLmpwMDw",19},
    };
    for (auto & test : test_cases) {
	if (getBackpackScore(test.first) != test.second) {
	    std::cout << "Test failed: " << test.first << " expected " << test.second <<
		" received " << getBackpackScore(test.first) <<"\n";
	} else {
	    std::cout << "Test passed: " << test.first <<"\n";
	}
    }

}

int main()
{
    testCharacters();
    testScores();
    
    std::ifstream infile = std::ifstream("data.dat");
    std::string line;
    int total = 0;
    while (infile.good()) {
	getline(infile, line, '\n');
	int score = getBackpackScore(line);
	total += score;
    }
    
    std::cout << "Total score: " << total << "\n";

    return 0;
}
