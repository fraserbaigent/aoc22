#include <iostream>
#include "../../lib/file_reader.hpp"
#include <vector>
#include <regex>

using Coordinate = std::pair<int, int>;

int manhattanDistance(Coordinate const& source,
		      Coordinate const& destination) {
    return std::abs(destination.second - source.second) +
	std::abs(destination.first - source. first);
}

class SensorArray {
private:
    Coordinate const _source;
    int const _manhattan_distance { 0 };
public:
    SensorArray(Coordinate && source,
		Coordinate const& destination)
	: _source{std::move(source)}
	, _manhattan_distance{manhattanDistance(_source, destination)}
    {
    }

    int getX() const {
	return _source.first;
    }
    
    int getY() const {
	return _source.second;
    }
    
    void print() const {
	std::cout << "(" << _source.first <<", "<<_source.second<<") d="<<_manhattan_distance<<"\n";
    }

    int getManhattanDistance() const {
	return _manhattan_distance;
    }
};

std::vector<SensorArray> ingestSourceFile(FileReader & readfile) {
    std::vector<SensorArray> arrays;
    std::regex expression("^Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)$");
    while (readfile.good()) {
	auto line = readfile.nextLine();
	std::smatch match;
	std::regex_match(line, match, expression);
	Coordinate source{
	    std::stoi(match[1]),
	    std::stoi(match[2])};
	Coordinate const destination{
	    std::stoi(match[3]),
	    std::stoi(match[4])};
	arrays.push_back(SensorArray(std::move(source), destination));
    }
    
    return arrays;
};

using Span = Coordinate;

std::vector<Span> getSpansAtRow(std::vector<SensorArray> const& sensors, int const row_number) {
    std::vector<Span> spans;
    for (auto & sensor : sensors) {
	int y_delta = std::abs(sensor.getY() - row_number);
	int cols = std::max(sensor.getManhattanDistance() - y_delta, 0);
	if (cols >= 0) {
	    spans.push_back(Span{sensor.getX() - cols, sensor.getX() + cols});
	}
    }
    return spans;
}

void sortSpans(std::vector<Span> & spans) {
    std::sort(spans.begin(),
	      spans.end());
}

int getDistance(Span const& span) {
    return span.second - span.first;
}

int calculateCoverage(std::vector<Span> const& spans) {
    int count { 0 };
    auto span = spans[0];
    for (int i = 1; i < spans.size(); ++i) {
	auto & new_span = spans[i];
	if (new_span.first > span.second) {
	    count += getDistance(span);
	    span = new_span;
	} else if (new_span.second > span.second) {
	    span.second = new_span.second;
	}
    }
    count += getDistance(span);
    return count;
}

void solvePart1(std::vector<SensorArray> const& sensors, int const row_number) {
    std::vector<Span> spans = getSpansAtRow(sensors, row_number);
    sortSpans(spans);
    int covered_cols = calculateCoverage(spans);
    std::cout << "Answer: " << covered_cols <<"\n";
}

int main()
{
    auto reader = FileReader("data.dat");
    auto sensors = ingestSourceFile(reader);

    solvePart1(sensors, 2000000);
    return 0;
}
