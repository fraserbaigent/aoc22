#include <string>
#include <vector>
#include <queue>
#include <regex>
#include <iostream>

class Monkey {
private:
    enum CallbackType {
	ADD,
	MULTIPLY,
	SQUARE,
    };
    std::queue<int> _items;
    int _divisor { 0 };
    int _true_monkey { 0 };
    int _false_monkey { 0 };
    int _number { 0 };
    int _inspection_count { 0 };
    int _callback_type { 0 };
    int _callback_value { 0 };
public:
    Monkey(std::vector<std::string> const& config) {
	getMonkeyNumber(config[0]);
	getStartingItems(config[1]);
	configureCallback(config[2]);
	configureDivisor(config[3]);
	_true_monkey = getResultMonkey(config[4]);
	_false_monkey = getResultMonkey(config[5]);
    }

    void receiveItem(int item) {
	_items.emplace(item);
    }

    int throwItem() {
	int item = _items.front();
	_items.pop();
	return item;
    }

    bool hasItems() const {
	return !_items.empty();
    }
    
    int getMonkeyToPass() {
	int &item = _items.front();
	doCallback(item);
	item /= 3;
	_inspection_count++;
	return getMonkeyToPassTo(item);
    }

    int getNumber() const {
	return _number;
    }
    
    int getInspectionCount() const {
	return _inspection_count;
    }
private:
    void getMonkeyNumber(std::string const& str) {
	std::regex re("^Monkey (\\d+)\\:$");
	std::smatch sm;
	std::regex_match(str, sm, re);
	_number = std::stoi(sm[1]);
    }

    void getStartingItems(std::string const& str) {
	std::regex re(".*Starting items: (.*)");
	std::smatch sm;
	std::regex_match(str, sm, re);
	auto index = 0;
	std::string list = sm[1];
	while (index < list.size()) {
	    auto next_index = list.find(',', index);
	    if (next_index == std::string::npos) {
		next_index = list.size();
	    }
	    _items.emplace(std::stoi(list.substr(index, next_index - index)));
	    index = next_index + 1;
	}
    }

    void configureCallback(std::string const& str) {
	std::regex re(".*Operation: new = old ([\\*\\+]) (old|\\d+)$");
	std::smatch sm;
	std::regex_match(str, sm, re);
	if (sm[2] == "old") {
	    _callback_type = SQUARE;
	} else {
	    if (sm[1] == "*") {
		_callback_type = MULTIPLY;
	    } else {
		_callback_type = ADD;
	    }
	    _callback_value = std::stoi(sm[2]);
	}
    }

    void configureDivisor(std::string const& str) {
	std::regex re(".*Test: divisible by (\\d+)$");
	std::smatch sm;
	std::regex_match(str, sm, re);
	_divisor = std::stoi(sm[1]);
    }

    int getResultMonkey(std::string const& str) {
	std::regex re(".*If \\w+: throw to monkey (\\d+)$");
	std::smatch sm;
	std::regex_match(str, sm, re);
	return std::stoi(sm[1]);
    }
    
    int getMonkeyToPassTo(int const& worry_level) {
	int result =  (worry_level % _divisor) == 0 ? _true_monkey : _false_monkey;
	return result;
    }

    void doCallback(int & worry_level) {
	switch (_callback_type) {
	case ADD:
	    worry_level += _callback_value;
	    break;
	case MULTIPLY:
	    worry_level *= _callback_value;
	    break;
	case SQUARE:
	    worry_level *= worry_level;
	    break;
	}
    }
};
