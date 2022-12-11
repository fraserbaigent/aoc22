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
    std::vector<Point> _knots;

    VisitedPoints _visited_points;

    bool _answer_cached { false };
    int _answer { 0 };
public:
    Solver(int knot_count)
	:_knots { std::vector<Point>(knot_count, Point{0,0}) }
    {
	_visited_points[0].insert(0);
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
	auto & head = _knots[0];
	if (direction == 'U') {
	    move = &head.second;
	    sign = -1;
	} else if (direction == 'D') {
	    move = &head.second;
	    sign = 1;
	} else if (direction == 'R') {
	    move = &head.first;
	    sign = 1;
	} else {
	    move = &head.first;
	    sign = -1;
	}
	for (int i = 0; i < count; ++i) {
	    *move += sign;
	    moveWholeTail(0, 1);
	}
    }

    void moveWholeTail(int first, int second) {
	auto const& head = _knots[first];
	if (second >= _knots.size()) {
	    addTailToVisited(head);
	    return;
	}
	auto & tail = _knots[second];
	int dx = head.first - tail.first;
	int dy = head.second - tail.second;	
	if (abs(dx) > 1 && abs(dy) > 1) {
	    tail.first = head.first + (abs(dx) / -dx);
	    tail.second = head.second + (abs(dy) / -dy);
	} else if (abs(dx) > 1) {
	    tail.second = head.second;
	    tail.first = head.first + (abs(dx) / -dx);
	} else if (abs(dy) > 1) {
	    tail.first = head.first;
	    tail.second = head.second + (abs(dy) / -dy);
	} else {
	    return;
	}
	moveWholeTail(first + 1, second + 1);
    }
    
    void addTailToVisited(Point const& tail) {
	auto & x_data = _visited_points[tail.first];
	x_data.insert(tail.second);
    }
	    };

int main()
{
    auto reader = FileReader("data.dat");
    static constexpr int knot_count { 10 };
    Solver solver(knot_count);
    while (reader.good()) {
	    std::string line = reader.nextLine();
	    solver.solveLine(line);
    }
    std::cout << "Answer : " << solver.getAnswer() <<"\n";

    return 0;
}
