#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <vector>

class Packet {
private:
    std::vector<Packet> _sub_values;
    int _value { -1 };
public:
    Packet() {
    }
    
    Packet(const int value)
	: _value { value } {
	//std::cout << "Adding packet value " << value << "\n";
    }
    
    void addValue(Packet value) {
	_sub_values.push_back(std::move(value));
    }

    Packet const& getPacket(size_t index) const& {
	return _sub_values[index];
    }

    int getValue() const {
	return _value;
    }

    int size() const {
	return _sub_values.size();
    }
    
    void printValue() const {
	if (_value >= 0) {
	    std::cout << _value;
	} else {
	    std::cout << "[";
	    for (int i = 0; i < _sub_values.size(); ++i) {
		_sub_values[i].printValue();
		if (i < _sub_values.size()-1) {
		    std::cout << ",";
		}
	    }
	    std::cout << "]";
	}
    };

    bool isEmptyList() const {
	return _value < 0 && _sub_values.empty();
    }
};
#endif
