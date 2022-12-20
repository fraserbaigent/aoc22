#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <iomanip>
#include "../../lib/file_reader.hpp"
#include <vector>
#include <string>
#include "packet.hpp"
#include <algorithm>

namespace Solver {
enum Comparisons {
    TRUE,
    FALSE,
    EQUAL
};
    
std::pair<int, int> findNextSquareBraces(std::string const& line, size_t offset = 0) {
    std::pair<int, int> positions { -1, -1 };
    auto index = line.find('[', offset);
    if (index == std::string::npos) {
	return positions;
    } else {
	positions.first = index;
    }
    auto open_count { 1 };
    auto close_count { 0 };
    while (open_count > close_count && ++index < line.size()) {
	if (line[index] == '[') {
	    open_count++;
	} else if (line[index] == ']') {
	    close_count++;
	}
    }
    positions.second = index;
    return positions;
}

std::string strippedArrayStringFrom(std::string const& base_string,
				    std::pair<int,int> const& indices) {
    return base_string.substr(indices.first + 1, indices.second - indices.first - 1);
}
    

std::vector<int> getValuesFromCommaListString(std::string const& values) {
    auto start { 0 };
    std::vector<int> result;
    while (start != std::string::npos) {
	auto comma = values.find(',', start + 1);
	auto end_index = std::min(comma, values.size());
	result.push_back(std::stoi(values.substr(start, end_index - start)));
	if (comma == std::string::npos) {
	    break;
	}
	start = comma + 1;
    }
    return result;
}

    Packet getPacket(std::string const& line, bool is_divider = false) {
    Packet root;
    if (is_divider) {
	root.setDivider();
    }
    
    auto index { 0 };
    while (index < line.size()) {
	while (line[index] == ',' || line[index] == ' ') {
	    index++;
	}
	auto next_comma = line.find(',', index);
	auto next_braces = findNextSquareBraces(line, index);
	if (next_braces.first < 0 || next_comma < next_braces.first) {
	    // handle a list element
	    auto end_index = next_braces.first < 0 ? line.size() - 1 : next_braces.first -2;
	    auto array_elements = line.substr(index, end_index - index + 1);
	    std::vector<int> values = getValuesFromCommaListString(array_elements);
	    for (auto v : values) {
		root.addValue(Packet(v));
	    }
	    index = end_index + 1;
	} else if (next_braces.first >= 0 && next_braces.second >= 0) {
	    // handle another list
	    auto next_line = strippedArrayStringFrom(line, next_braces);
	    root.addValue(getPacket(next_line));
	    index = next_braces.second + 1;
	} else {
	    break;
	}
					      
    }
    return root;
}

int inCorrectOrder(Packet const& left, Packet const& right) {

    //std::cout << "Comparing:\n";
    //left.printValue();
    //std::cout << std::endl;
    //right.printValue();
    //std::cout << std::endl;
	
    if (left.isEmptyList() && !right.isEmptyList()) {
	return TRUE;
    } else if (right.isEmptyList() && !left.isEmptyList()) {
	return FALSE;
    } else if (left.size() == 0 && right.size() == 0) {
	if (left.getValue() == right.getValue()) {
	    return EQUAL;
	} else if (left.getValue() < right.getValue()) {
	    return TRUE;
	} else {
	    return FALSE;
	}
    } else if (left.size() > 0 && right.size() > 0) {
	for (int i = 0; i < std::min(left.size(), right.size()); i++) {
	    auto result = inCorrectOrder(left.getPacket(i), right.getPacket(i));
	    if (result != EQUAL) {
		return result;
	    }
	}
	return left.size() == right.size() ? EQUAL :
	    left.size() < right.size() ? TRUE : FALSE;
    } else if (left.size() == 0) {
	Packet wrapper;
	wrapper.addValue(left);
	return inCorrectOrder(wrapper, right);
    } else if (right.size() == 0) {
	Packet wrapper;
	wrapper.addValue(right);
	return inCorrectOrder(left, wrapper);
    }
    return FALSE;
}
bool inCorrectOrder(std::string const& packet_l,
                  std::string const& packet_r) {
    auto packet_1 = getPacket(packet_l.substr(1, packet_l.size() - 2));
    auto packet_2 = getPacket(packet_r.substr(1, packet_r.size() - 2));
    
    return inCorrectOrder(packet_1, packet_2) != FALSE;
}

int solveMain(std::string const& file_name) {
    auto reader = FileReader(file_name);
    int pair_index { 1 };
    int answer { 0 };
    while (reader.good()) {
    	auto line_1 = reader.nextLine();
    	auto line_2 = reader.nextLine();
    	if (reader.good()) {
    	    reader.nextLine();
    	}
    	if (inCorrectOrder(line_1, line_2)) {
    	    std::cout << "Adding " << pair_index << " to list\n";
    	    answer += pair_index;
    	}
    	pair_index++;
    }
    return answer;
}

void printResult(std::vector<Packet> const& packets) {
    int i { 0 };
    for (auto &p : packets) {
	std::cout << std::setw(3) << ++i << " | ";
	p.printValue();
	if (p.isDivider()) {
	    std::cout << "<-------------------------- DIVIDER";
	}
	std::cout << std::endl;
    }
}
    
int solveMain2(std::string const& file_name) {
    auto reader = FileReader(file_name);
    std::vector<Packet> packets { getPacket("[[2]]", true), getPacket("[[6]]", true)};
    while (reader.good()) {
	auto line = reader.nextLine();
	if (line.empty()) {
	    continue;
	}
	packets.push_back(getPacket(line.substr(1, line.size() - 2)));
    }
    std::sort(packets.begin(),
	      packets.end(),
	      [](auto const& l, auto const& r) {
		  return !inCorrectOrder(l, r);
	      });
    auto first = std::find_if(packets.begin(),
			      packets.end(),
			      [](auto const& p) {
				  return p.isDivider();
			      });
    auto second = std::find_if(first + 1,
			      packets.end(),
			      [](auto const& p) {
				  return p.isDivider();
			      });
    printResult(packets);
    
    return
	(std::distance(packets.begin(), first) + 1) *
	(std::distance(packets.begin(), second) + 1);
}
    
};

#endif
