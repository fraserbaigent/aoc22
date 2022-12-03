#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

class TopValues {
public:
    TopValues() :
	values_{std::vector<int>{ELF_COUNT,0}}
    {
    }
    
    void addValueIfTop(const int total) {
	int index = -1;
	for (int i = 0; i < (int)values_.size(); ++i) {
	    if (total < values_[i]) {
		break;
	    } else {
		index = i;
	    }
	}
	int insert_index = index;
	while (index > 0) {
	    values_[index-1] = values_[index];
	    index--;
	}
	
	if (insert_index >= 0) {
	    values_[insert_index] = total;
	}	
    }

    void printTotalValue() {
	std::cout <<
	    "Max calories: " <<
	    std::accumulate(values_.begin(), values_.end(), 0) <<
	    "\n";
    }
private:
    static constexpr int ELF_COUNT = 3;
    std::vector<int> values_;
};

int main() {
    TopValues top_vals;
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
	    top_vals.addValueIfTop(total);
	    total = 0;
	}
    }
    
    top_vals.addValueIfTop(total);
    top_vals.printTotalValue();
    return 0;
}
