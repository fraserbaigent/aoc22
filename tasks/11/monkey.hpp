#include <string>
#include <vector>
#include <queue>
#include <regex>
#include <iostream>


using ulong = std::uint64_t;

class Monkey {
private:
    enum CallbackType {
	ADD,
	MULTIPLY,
	SQUARE,
    };
    std::queue<ulong> _items;
    ulong _divisor { 0 };
    ulong _true_monkey { 0 };
    ulong _false_monkey { 0 };
    ulong _number { 0 };
    ulong _inspection_count { 0 };
    ulong _callback_type { 0 };
    ulong _callback_value { 0 };
    ulong _modulo { 1 };
public:
    Monkey(std::vector<std::string> const& config) {
	getMonkeyNumber(config[0]);
	getStartingItems(config[1]);
	configureCallback(config[2]);
	configureDivisor(config[3]);
	_true_monkey = getResultMonkey(config[4]);
	_false_monkey = getResultMonkey(config[5]);
    }

    void receiveItem(ulong item) {
	_items.emplace(item);
    }

    ulong throwItem() {
	auto item = _items.front();
	_items.pop();
	return item;
    }

    bool hasItems() const {
	return !_items.empty();
    }
    
    ulong getMonkeyToPass(bool divide_number = true) {
	auto &item = _items.front();
	doCallback(item);
	if (divide_number) {
	    item /= 3;
	} else {
	    item %= _modulo;
	}
	_inspection_count++;
	return getMonkeyToPassTo(item);
    }

    ulong getNumber() const {
	return _number;
    }
    
    ulong getInspectionCount() const {
	return _inspection_count;
    }

    ulong getDivisor() const {
	return _divisor;
    }

    void setModulo(ulong modulo) {
	_modulo = modulo;
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

    ulong getResultMonkey(std::string const& str) {
	std::regex re(".*If \\w+: throw to monkey (\\d+)$");
	std::smatch sm;
	std::regex_match(str, sm, re);
	return std::stoi(sm[1]);
    }
    
    ulong getMonkeyToPassTo(ulong const& worry_level) {
	ulong result =  (worry_level % _divisor) == 0 ? _true_monkey : _false_monkey;
	return result;
    }

    void doCallback(ulong & worry_level) {
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
