#include <iostream>
#include <fstream>

int main() {
    int max_val = 0;
    int total = 0;
    std::ifstream infile = std::ifstream("data.dat");
    std::string line;
    while (infile.good()) {
	getline(infile, line, '\n');
	std::cout << line <<"\n";
	if (!line.empty()) {
	    int val = std::stoi(line);
	    total += val;
	} else {
	    if (total > max_val) {
		max_val = total;
	    }
	    total = 0;
	}
    }
    if (total > max_val) {
	max_val = total;
    }

    std::cout << "Max value found : " << max_val << std::endl;
    return 0;
}
