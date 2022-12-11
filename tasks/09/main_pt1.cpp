#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../lib/file_reader.hpp"

using Point = std::pair<int, int>;

using VisitedPoints = std::unordered_map<int, std::unordered_set<int>>;

class Solver {
private:
    Point _head { 0, 0 };
    Point _tail { 0, 0 };

    VisitedPoints _visited_points;

    bool _answer_cached { false };
    int _answer { 0 };
public:
    Solver() {
	addTailToVisited();
    }
    
    void solveLine(std::string const& line) {
	moveRope(line);
    }

    int getAnswer() {
	if (!_answer_cached) {
	    for (auto & x : _visited_points) {
		_answer += x.second.size();
	    }
	    
	    _answer_cached = true;
	}
	return _answer;
    }

private:
    void moveRope(std::string const& line) {
	int count = std::stoi(line.substr(2, line.size() - 2));
	char direction = line[0];

	int * move { nullptr };
	int sign = 0;
	if (direction == 'U') {
	    move = &_head.second;
	    sign = 1;
	} else if (direction == 'D') {
	    move = &_head.second;
	    sign = -1;
	} else if (direction == 'R') {
	    move = &_head.first;
	    sign = 1;
	} else {
	    move = &_head.first;
	    sign = -1;
	}
	std::cout << line <<"\n";
	for (int i = 0; i < count; ++i) {
	    std::cout << *move << " + " << sign <<"\n";
	    *move += sign;
	    moveTail();
	    addTailToVisited();
	}
    }

    void moveTail() {
	int dx = _head.first - _tail.first;
	int dy = _head.second - _tail.second;

	if (abs(dx) > 1) {
	    _tail.second = _head.second;
	    _tail.first = _head.first + (abs(dx) / -dx);
	} else if (abs(dy) > 1) {
	    _tail.first = _head.first;
	    _tail.second = _head.second + (abs(dy) / -dy);
	}
    }

    void addTailToVisited() {
	auto & x_data = _visited_points[_tail.first];
	x_data.insert(_tail.second);
    }
};

int main()
{
    auto reader = FileReader("data.dat");
    Solver solver;
    while (reader.good()) {
	    std::string line = reader.nextLine();
	    solver.solveLine(line);
    }
    std::cout << "Answer : " << solver.getAnswer() <<"\n";

    return 0;
}
