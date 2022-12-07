
#include <fstream>
#include <iostream>

enum Weapon {
    ROCK,
    PAPER,
    SCISSORS,
};

int battleScore(int left_weapon, int right_weapon) {
    int score = right_weapon - left_weapon;
    if (score == 0) {
	return 3;
    } else if (score == -2 || score == 1) {
	return 6;
    } else {
	return 0;
    }
}

int weaponScore(int weapon) {
    return weapon + 1;
}

int doBattle(char left, char right) {
    const int left_weapon = left - 'A';
    const int right_weapon = right - 'X';

    return battleScore(left_weapon, right_weapon) +
	weaponScore(right_weapon);
}

int main()
{
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
