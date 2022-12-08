#include <fstream>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>
#include <unordered_map>

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

std::unordered_set<char> getCommonLetters(std::string const& line, std::unordered_set<char> const& seen_letters) {
    std::unordered_set<char> common_letters;
    for (auto c : line) {
	if (seen_letters.size() == 0 ||
	    seen_letters.find(c) != seen_letters.cend()) {
	    common_letters.emplace(c);
	}
    }
    return common_letters;
}


int getScoreFromTeam(std::vector<std::string> const& backpacks) {
    std::unordered_set<char> seen_letters;
    for (auto & line : backpacks) {
	seen_letters = getCommonLetters(line, seen_letters);
    }
    return getScore(*seen_letters.begin());
}

void testGetScoreFromTeam() {
    std::vector<std::vector<std::string>> test_cases {
	{"vJrwpWtwJgWrhcsFMMfFFhFp", "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL", "PmmdzqPrVvPwwTWBwg"},
	{"wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn", "ttgJtRGJQctTZtZT", "CrZsJsPPZsGzwwsLwLmpwMDw"},
    };

    std::vector<int> scores { 18, 52 };

    for (int i = 0; i < test_cases.size(); ++i) {
	if (getScoreFromTeam(test_cases[i]) != scores[i]) {
	    std::cout << "Test failed: " << test_cases[i][0] << " expected " << scores[i] <<
		" received " << getScoreFromTeam(test_cases[i]) <<"\n";
	} else {
	    std::cout << "Test passed: " << test_cases[i][0] <<"\n";
	}
    }    
}

int main()
{
    testCharacters();
    testGetScoreFromTeam();
    
    std::ifstream infile = std::ifstream("data.dat");
    std::string line;
    int total = 0;
    int counter = 0;
    std::vector<std::string> backpacks{3, ""};
    while (infile.good()) {
	getline(infile, line, '\n');
	backpacks[counter++ % 3] = line;
	if (counter % 3 == 0) {
	    total += getScoreFromTeam(backpacks);
	}
    }
    
    std::cout << "Total score: " << total << "\n";

    return 0;
}
