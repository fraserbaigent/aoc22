#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "../../lib/file_reader.hpp"


class Command {
private:
    int _type { NOOP };
    int _value { -1 };
public:
    enum CommandType {
	NOOP,
	ADDX,
    };

    Command(CommandType type, int value)
	: _type{type}
	, _value{value}
    {
    }

    int addAfterValue() const {
	return _value;
    }

    int getType() const {
	return _type;
    }
};

class Cpu {
private:
    int _register { 1 };
    int _cycle_number { 1 };
    std::queue<Command> _commands;
public:
    void addCommand(Command const& cmd) {
	_commands.push(cmd);
    }

    void runCycle() {
	_cycle_number++;
	executeCommand(_commands.front());
	_commands.pop();
    }

    int commandCount() const {
	return _commands.size();
    }
    
    void executeCommand(Command const& command) {
	using CT = Command::CommandType;
	switch(command.getType()) {
	case CT::NOOP: {
	    _register += command.addAfterValue();
	    break;
	}
	case CT::ADDX: {
	    _commands.push(Command(CT::NOOP,command.addAfterValue()));
	    break;
	}
	}
    }
    
    int getCycleNumber() const {
	return _cycle_number;
    }

    int getRegister() const {
	return _register;
    }
};

Command getCommandFrom(std::string const& line) {
    std::string cmd = line.substr(0,4);
    if (cmd == "noop") {
	return Command(Command::CommandType::NOOP, 0);
    } else if (cmd == "addx") {
	return Command(Command::CommandType::ADDX,std::stoi(line.substr(5, line.size() - 5)));
    }
    return Command(Command::CommandType::NOOP, 0);
}

std::vector<Command> getCommandsFrom(FileReader & reader) {
    std::vector<Command> commands;
    while (reader.good()) {
	std::string line = reader.nextLine();
	commands.push_back(getCommandFrom(line));
    }
    return commands;
}

int main()
{
    auto reader = FileReader("data.dat");
    Cpu cpu;
    std::vector<Command> commands = getCommandsFrom(reader);
    std::vector<int> cycles_of_interest { 20, 60, 100, 140, 180, 220};
    int i { 0 };
    int sum_of_signal_strengths { 0 };
    for (auto &command : commands) {
	cpu.addCommand(command);
	while (cpu.commandCount() != 0) {
	    if (cpu.getCycleNumber() == cycles_of_interest[i]) {
		sum_of_signal_strengths += cpu.getCycleNumber() * cpu.getRegister();
		++i;
	    }

	    cpu.runCycle();
	}
    }
    
    std::cout << "Answer : " << sum_of_signal_strengths <<"\n";

    return 0;
}
