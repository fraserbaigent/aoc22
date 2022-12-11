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

class Crt {
private:
    int const _width { 0 };
    int const _height { 0 };
    std::vector<std::vector<char>> _pixels;

public:
    Crt(int const width, int const height)
	: _width{width}
	, _height{height}
	, _pixels{std::vector<std::vector<char>>(_height, std::vector<char>(_width, '.'))}
    {
    }

    void drawPixel(int cycle, int cpu_register) {
	cycle = (cycle - 1) % 240;
	int row = cycle / _width ;
	int col = cycle % _width;
	_pixels[row][col] =  cpu_register - 1 <= col && cpu_register + 1 >= col ?
	    '#' : '.';
    }

    void printCrtScreen() {
	for (auto & row : _pixels) {
	    for (auto c : row) {
		std::cout << " " << c;
	    }
	    std::cout <<"\n";
	}
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
    static constexpr int crt_height = 6;
    static constexpr int crt_width = 40;
    Crt crt(crt_width ,crt_height);
    
    for (auto &command : commands) {
	cpu.addCommand(command);
	while (cpu.commandCount() != 0) {
	    crt.drawPixel(cpu.getCycleNumber(), cpu.getRegister());
	    cpu.runCycle();
	}
    }
    crt.printCrtScreen();

    return 0;
}
