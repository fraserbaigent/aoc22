#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> totals;
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
	    totals.push_back(total);
	    total = 0;
	}
    }
    if (total != 0) {
	totals.push_back(total);
    }
    
    if (totals.size() > 3) {
	std::partial_sort(totals.begin(),
			  totals.begin() + 3,
			  totals.end(),
			  [] (auto &l, auto &r) {
			      return l > r;
			  });
    }
    
    std::cout << "\nMax calories:" << std::accumulate(totals.begin(), totals.begin() + 3, 0) << "\n";
    return 0;
}
