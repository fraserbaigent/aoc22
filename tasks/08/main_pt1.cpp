#include "../../lib/file_reader.hpp"
#include "../../lib/tests.hpp"

#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

using Grid = std::vector<std::string>;
using SeenGrid = std::vector<std::vector<bool>>;

int toInt(char c) {
    return c - '0';
}

std::string getColumnWord(Grid const& grid, int col) {
    std::stringstream ss;
    for (int i = 0; i < grid.size(); ++i) {
	ss << grid[i][col];
    }
    return ss.str();
}

int calculateVisibleTrees(std::string const& word, SeenGrid * seen, int x, bool is_row) {
    int visible = 0;
    int seen_l = -1;
    int seen_r = -1;
    for (int i = 0, j = word.size() - 1; i <= j;) {
	int l = toInt(word[i]);
	int r = toInt(word[j]);
	if (l > seen_l) {
	    if (!seen ||
		(is_row && !(*seen)[x][i]) ||
		(!is_row && !(*seen)[i][x])) {
		++visible;
	    }
	    seen_l = l;
	    if (seen) {
		if (is_row) {
		    (*seen)[x][i] = true;
		} else {
		    (*seen)[i][x] = true;
		}
	    }
	    i++;
	} else if (r > seen_r) {
	    if (!seen ||
		(is_row && !(*seen)[x][j]) ||
		(!is_row && !(*seen)[j][x])) {
		++visible;
	    }
	    seen_r = r;
	    if (seen) {
		if (is_row) {
		    (*seen)[x][j] = true;
		} else {
		    (*seen)[j][x] = true;
		}
	    }
	    --j;
	} else if (seen_l < seen_r) {
	    i++;
	} else if (seen_r < seen_l) {
	    j--;
	} else {
	    ++i;
	    --j;
	}
    };
    return visible;
}					     

void testCalculateVisibleTrees() {
    std::unordered_map<std::string, int> cases {
	{"30373",3},
	{"25512",4},
	{"65332",4},
	{"33549",3},
	{"35390",4},
    };

    Tests::doTest(cases, [&] (auto & cs) {
	return calculateVisibleTrees(cs, nullptr, 0, false);
    });
}

int do3DSolve(Grid const& grid) {
    int visible = 0;
    SeenGrid seen_grid = {grid.size(), std::vector<bool>(grid[0].size(), false)};
    for (int row = 0; row < 2; ++row) {
	for (int i = 0; i < grid.size(); ++i) {
	    bool is_row = row == 0;
	    std::string adjusted_word = row ? grid[i] :
		getColumnWord(grid,i);
	    visible += calculateVisibleTrees(adjusted_word, &seen_grid, i, is_row);
	}
    }
    
    return visible;
}

int main()
{
    testCalculateVisibleTrees();
    FileReader reader("data.dat");
    
    Grid grid;
    
    while (reader.good()) {
    	grid.push_back(reader.nextLine());
    }    
    
    int result = do3DSolve(grid);
    
    std::cout << "Answer: " << result <<"\n";
    
    return 0;
}
