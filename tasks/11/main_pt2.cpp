#include <iostream>
#include <set>
#include "../../lib/file_reader.hpp"
#include "monkey.hpp"

std::vector<std::string> getMonkeyStrings(FileReader & reader) {
    std::vector<std::string> strings;
    while (reader.good()) {
	std::string line = reader.nextLine();
	if (line.empty()) {
	    break;
	}
	strings.push_back(std::move(line));
    }

    return strings;
}

void initialiseCommonModulo(std::vector<Monkey> &monkeys) {
    ulong divisor_product { 1 };
    for (auto & monkey : monkeys) {
	divisor_product *= monkey.getDivisor();
    }
    for (auto & monkey : monkeys) {
	monkey.setModulo(divisor_product);
    }
}    

void playCatch(std::vector<Monkey> &monkeys, ulong number_of_rounds) {    
    for (ulong i = 0; i < number_of_rounds;++i) {
	for (auto & monkey : monkeys) {
	    while (monkey.hasItems()) {
		auto to_pass = monkey.getMonkeyToPass(false);
		auto item = monkey.throwItem();
		monkeys[to_pass].receiveItem(item);
	    }
	}
    }
}

ulong getMonkeyBusiness(std::vector<Monkey> & monkeys) {
    std::sort(monkeys.begin(),
	      monkeys.end(),
	      [&](auto & l, auto & r) {
		  return l.getInspectionCount() > r.getInspectionCount();
	      });
    for (auto & monkey : monkeys) {
	std::cout << "Monkey " << monkey.getNumber() <<
	    " inspected items " << monkey.getInspectionCount() <<
	    " times.\n";
    }
    return monkeys[0].getInspectionCount() * monkeys[1].getInspectionCount();
}

int main()
{
    auto reader = FileReader("data.dat");

    std::vector<Monkey> monkeys;
    while (reader.good()) {
	std::vector<std::string> monkey_strings = getMonkeyStrings(reader);
	monkeys.push_back(Monkey(monkey_strings));
    }
    static constexpr ulong number_of_rounds = 10000;
    initialiseCommonModulo(monkeys);
    playCatch(monkeys, number_of_rounds);
    ulong business = getMonkeyBusiness(monkeys);
    std::cout << "Answer: " << business <<"\n";
    
    return 0;
}
