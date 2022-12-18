#include <iostream>
#include "../../lib/file_reader.hpp"
#include <vector>
#include <bits/stdc++.h>
#include <queue>

using HeightMap = std::vector<std::vector<int>>;
using Coordinate = std::pair<int, int>;

void printGrid(HeightMap const& grid) {
    for (auto & row : grid) {
	for (auto c : row) {
	    std::cout << c << " ";
	}
	std::cout << std::endl;
    }
}

HeightMap generateHeightMapFrom(FileReader & reader,
				Coordinate & start_coordinate,
				Coordinate & end_coordinate) {
    HeightMap height_map;
    while (reader.good()) {
	auto line = reader.nextLine();
	std::vector<int> heights(line.size(), 0);
	for (size_t i = 0; i < line.size(); ++i) {
	    auto c = line[i];
	    if (c == 'S') {
		start_coordinate = {i, height_map.size()};
		c = 'a';
	    } else if (c == 'E') {
		end_coordinate = {i, height_map.size()};
		c = 'z';
	    }
	    heights[i] = c - 'a';
	}
	height_map.push_back(std::move(heights));
    }
    return height_map;
}

class Solver {
private:
    struct SolveStep {
	int _x;
	int _y;
	int _from_direction;
	int _from_height;
	int _steps_to;
    };
    
    using VisitedMap = HeightMap;
    HeightMap const& _height_map;
    Coordinate const& _start_coordinate;
    Coordinate const& _end_coordinate;
    VisitedMap _min_steps;
    std::queue<SolveStep> _steps;

    enum DirectionFrom {
	NORTH,
	SOUTH,
	EAST,
	WEST,
	START
    };
    int _min_steps_to_end { INT_MAX };
    
public:
    explicit Solver(HeightMap const& height_map,
	   Coordinate const& start_coordinate,
	   Coordinate const& end_coordinate)
	: _height_map{height_map}
	, _start_coordinate{start_coordinate}  
	, _end_coordinate{end_coordinate}  
	, _min_steps{_height_map.size(),
		     std::vector<int>((_height_map[0]).size(), INT_MAX)
	    } {
    }

    void solve() {

	_steps.emplace(SolveStep{_start_coordinate.first,
				 _start_coordinate.second,
				 START,
				 0,
				 0});
	while (!_steps.empty()) {
	    auto & step = _steps.front();
	    solveStep(step._x,
		      step._y,
		      step._from_direction,
		      step._from_height,
		      step._steps_to);
	    _steps.pop();
	}
    };

    void solveStep(int x, int y, int direction_from, int height_from, int steps) {
	int & this_steps = _min_steps[y][x];
	int const this_height = _height_map[y][x];
	if (direction_from != START &&
	    (this_steps <= steps ||
	     this_height < height_from - 1  ||
	     (x == _start_coordinate.first && y == _start_coordinate.second))) {
	    return;
	}
	this_steps = steps;
	if (direction_from != START && this_height == 0 && steps < _min_steps_to_end){
	    _min_steps_to_end = steps;
	}
	    
	if (x == _end_coordinate.first && y == _end_coordinate.second) {
	    return;
	}
	int next_steps = steps + 1;
	//north
	int y_n = y + 1;
	if (y_n < static_cast<int>(_min_steps.size()) &&
	    direction_from != NORTH) {
	    _steps.emplace(SolveStep{x,
				     y_n,
				     SOUTH,
				     this_height,
				     next_steps});
	}
	//south
	int y_s = y - 1;
	if (y_s >= 0 &&
	    direction_from != SOUTH) {
	    _steps.emplace(SolveStep{x,
				     y_s,
				     NORTH,
				     this_height,
				     next_steps});
	}
	//east
	int x_e = x + 1;
	if (x_e < static_cast<int>(_min_steps[y].size()) &&
	    direction_from != EAST) {
	    _steps.emplace(SolveStep{x_e,
				     y,
				     WEST,
				     this_height,
				     next_steps});
	}
	//west
	int x_w = x - 1;
	if (x_w >= 0 && direction_from != WEST) {
	    _steps.emplace(SolveStep{x_w,
				     y,
				     EAST,
				     this_height,
				     next_steps});
	}
    }

    void printAnswer() {
	std::cout << "Answer is :"<< _min_steps_to_end << "\n";
    }
};


int main()
{
    auto reader = FileReader("data.dat");
    Coordinate start_coordinate, end_coordinate;
    HeightMap height_map = generateHeightMapFrom(reader,
						 end_coordinate,
						 start_coordinate);
    printGrid(height_map);
    Solver solver(height_map, start_coordinate, end_coordinate);
    solver.solve();
    solver.printAnswer();
    
    return 0;
}
