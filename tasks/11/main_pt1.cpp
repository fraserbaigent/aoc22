#include <iostream>
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

void playCatch(std::vector<Monkey> &monkeys, int number_of_rounds) {
    for (int i = 0; i < number_of_rounds;++i) {
	for (auto & monkey : monkeys) {
	    while (monkey.hasItems()) {
		int to_pass = monkey.getMonkeyToPass();
		int item = monkey.throwItem();
		monkeys[to_pass].receiveItem(item);
	    }
	}
    }
}

long int getMonkeyBusiness(std::vector<Monkey> & monkeys) {
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
    static constexpr int number_of_rounds = 20;
    playCatch(monkeys, number_of_rounds);
    long int business = getMonkeyBusiness(monkeys);
    std::cout << "Answer: " << business <<"\n";
    
    return 0;
}
