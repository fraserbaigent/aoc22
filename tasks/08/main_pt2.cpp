#include "../../lib/file_reader.hpp"
#include "../../lib/tests.hpp"

#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using Grid = std::vector<std::string>;
using SeenGrid = std::vector<std::vector<bool>>;

struct Tree {
public:
    int _north {0};
    int _south {0};
    int _east {0};
    int _west {0};
    int const _height;

    Tree(int height)
	:_height{height}
    {
    }

    int getCanSee() const {
	return _north * _south * _east * _west;
    }

    int getTestCanSee() const {
	return
	    _north +
	    _south +
	    _east +
	    _west;
    }

};

int toInt(char c) {
    return c - '0';
}

class Forest {
private:
    std::vector<std::vector<Tree>> _trees;
public:
    Forest(Grid const& grid)
    {
	_trees.reserve(grid.size());
	for (auto const& row : grid) {
	    std::vector<Tree> tree_row;
	    tree_row.reserve(row.size());
	    for (auto & c : row) {
		tree_row.push_back(Tree(toInt(c)));
	    }
	    _trees.push_back(std::move(tree_row));
	}
    }

    void solve() {
	for (int i = 0; i < _trees.size();++i) {
	    traverseRow(i);
	}
	for (int j = 0; j < _trees[0].size(); ++j) {
	    traverseColumn(j);
	}
    }

    int getResult() const {
	int max_val { -1 };
	for (int x = 0; x < _trees.size(); ++x) {
	    for (int y = 0; y < _trees[0].size(); ++y) {
		int can_see = _trees[x][y].getCanSee();
		if (can_see > max_val) {
		    max_val = can_see;
		}
	    }
	}
	return max_val;
    }

    int getTestResult() const {
	int max_val { -1 };
	for (int x = 0; x < _trees.size(); ++x) {
	    for (int y = 0; y < _trees[0].size(); ++y) {
		int can_see = _trees[x][y].getTestCanSee();
		if (can_see > max_val) {
		    max_val = can_see;
		}
	    }
	}
	return max_val;
    }


private:
    void traverseRow(int row) {
	for (int i = 1; i < _trees[row].size(); ++i) {
	    _trees[row][i]._west = compareTree(i, -1, row, true);
	    int east_index = _trees[row].size() - 1 - i;
	    _trees[row][east_index]._east =  compareTree(east_index, 1, row, true);
	}
    }

    void traverseColumn(int column) {
	for (int i = 1; i < _trees.size(); ++i) {
	    _trees[i][column]._north = compareTree(i, -1, column, false);
	    int south_index = _trees.size() - 1 - i;
	    _trees[south_index][column]._south = compareTree(south_index, 1, column, false);
	}
    }
    
    Tree & getTree(int i, int row, bool is_row) {
	return is_row ?
	    _trees[row][i] : _trees[i][row];
    }
    
    int compareTree(int tree_index, int offset, int row, bool is_row) {
	int other_tree_index = tree_index + offset;
	int max_index = is_row ? _trees[row].size() - 1 : _trees.size() -1;
	if (other_tree_index <= 0 ||
	    other_tree_index >= max_index) {
	    return std::abs(offset);
	}
	Tree & tree  = getTree(tree_index, row, is_row);
	Tree & other_tree = getTree(other_tree_index, row, is_row);
	if (other_tree._height < tree._height) {
	    int new_offset = offset;
	    if (is_row) {
		if (offset < 0) {
		    new_offset -= other_tree._west > 0 ? other_tree._west : 1;
		} else {
		    new_offset += other_tree._east > 0 ? other_tree._east : 1;
		}
	    } else {
		if (offset < 0) {
		    new_offset -= other_tree._north > 0 ? other_tree._north : 1;
		} else {
		    new_offset += other_tree._south > 0 ? other_tree._south : 1;
		}
	    }
	    return compareTree(tree_index, new_offset, row, is_row);
	} else {
	    return std::abs(offset);
	}
    }

   
};

void testSolver() {
    std::vector<Grid> test_cases {
	{"0","1","2","3","4","5",},
	{"0000000", "0123210", "0000000", },
	{"0000000", "0123456", "0000000", },
	{"0000000", "5432345", "0000000", },	
	{"0000000", "1533521", "0000000", },
	{"25512", "65332", "33549", "35390",},
	};

    std::vector<int > addition_results {
	5,8,8,8,7,7,};

    std::vector<int> multiply_results {
	0, 9, 5, 4, 6 ,8};
    
    for(int i = 0; i < test_cases.size(); ++i) {
	std::cout << "TEST:\n";
	Forest f(test_cases[i]);
	f.solve();
	auto res = f.getTestResult();
	     
	if (res != addition_results[i]) {
	    std::cout << "Test failed: ";
	    for (auto & t : test_cases[i]) {
		std::cout << t << "\n             ";
	    }
	    std::cout <<
		"expected " << addition_results[i] <<
		" received " << res <<"\n";
	    } else {
	    std::cout << "Test passed: ";
	    for (int j = 0; j < test_cases[i].size(); ++j) {
		std::cout << test_cases[i][j] <<"\n";
		if (j != test_cases[i].size() -1) {
		    std::cout << "             ";
		}
	    }
	}

	auto real_res = f.getResult();
	if (real_res != multiply_results[i]) {
	    std::cout << "Test failed: ";
	    for (auto & t : test_cases[i]) {
		std::cout << t << "\n             ";
	    }
	    std::cout <<
		"expected " << multiply_results[i] <<
		" received " << real_res <<"\n";
	    } else {
	    std::cout << "Test passed: ";
	    for (int j = 0; j < test_cases[i].size(); ++j) {
		std::cout << test_cases[i][j] <<"\n";
		if (j != test_cases[i].size() -1) {
		    std::cout << "             ";
		}
	    }
	}

    }

    

    
}

int main()
{
    testSolver();
    FileReader reader("data.dat");
    
    Grid grid;
    
    while (reader.good()) {
    	grid.push_back(reader.nextLine());
    }    
    
    Forest forest(grid);
    forest.solve();
    std::cout << "Answer: " << forest.getResult() <<"\n";
    return 0;
}
