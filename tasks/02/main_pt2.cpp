#include <fstream>
#include <iostream>
#include <map>
#include <cassert>

enum Weapon {
    ROCK,
    PAPER,
    SCISSORS,
};

enum Result {
    LOSS,
    DRAW,
    WIN
};

int battleScore(int result) {
    switch (result) {
    case WIN:
	return 6;
    case LOSS:
	return 0;
    case DRAW:
	return 3;
    }
    return -100000;
}


int getWeapon(int other_weapon, int result) {
    if (result == DRAW) {
	return other_weapon;
    } else if (result == WIN) {
	return (other_weapon + 1) % 3;
    } else {
	return other_weapon == ROCK ? SCISSORS : other_weapon -1;
    }
}

int weaponScore(int weapon) {
    return weapon + 1;
}

int doBattle(char weapon, char result_char) {
    const int left_weapon = weapon - 'A';
    const int result = result_char - 'X';
    const int required_weapon = getWeapon(left_weapon, result);
    return battleScore(result) +
	weaponScore(required_weapon);
}

void test() {
    std::map<std::string, int> test_cases {
	{"A X", 3},
	{"A Y", 4},
        {"A Z", 8},
	{"B X", 1},
	{"B Y", 5},
	{"B Z", 9},
	{"C X", 2},
	{"C Y", 6},
	{"C Z", 7},
    };

    for (auto & test_case : test_cases) {
	assert(test_case.second == doBattle(test_case.first[0], test_case.first[2]));	
    };
}

int main()
{
    //test();
    std::ifstream infile = std::ifstream("data.dat");
    std::string line;
    int total = 0;
    while (infile.good()) {
    	getline(infile, line, '\n');
    	total += doBattle(line[0], line[2]);
    }
    
    std::cout << "Total score: " << total << "\n";
    return 0;
}

