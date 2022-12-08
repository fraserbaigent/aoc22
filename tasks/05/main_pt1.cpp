#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <regex>

using CrateStack = std::vector<std::stack<char>>;

class FileReader {
private:
    std::ifstream _infile;
public:
    FileReader(std::string const& path) :
	_infile{std::ifstream(path)} {
    }

    bool good() const {
	return _infile.good();
    }

    std::string nextLine() {
	std::string line;
	getline(_infile, line, '\n');
	return line;
    }
};

CrateStack sizeCrateStack(std::string const& size_line) {
    return CrateStack((size_line.size() + 1)/ 4, std::stack<char>{});
    
}

void addInputsFromLines(CrateStack & crate_stack, std::string const& line) {
    for (size_t i = 0; i < crate_stack.size(); i ++) {
	size_t char_index = (i * 4) + 1;
	if (line[char_index] != ' ') {
	    crate_stack[i].emplace(line[char_index]);
	}
    }
}

CrateStack getStacks(FileReader & reader) {
    std::stack<std::string> inputs;
    while (reader.good()) {
	auto line = reader.nextLine();
	if (line.empty()) {
	    break;
	}
	inputs.emplace(std::move(line));
    }

    CrateStack crate_stack = sizeCrateStack(inputs.top());
    inputs.pop();
    while (!inputs.empty()) {
	addInputsFromLines(crate_stack, inputs.top());
	inputs.pop();
    }
    return crate_stack;
}

void moveCrates(FileReader & reader, CrateStack & crate_stack) {
    static std::regex re("^move (\\d+) from (\\d+) to (\\d+)$");
    std::smatch sm;
    std::string line = reader.nextLine();
    std::regex_match(line, sm, re);
    int number_to_move = std::stoi(sm[1]);
    int from = std::stoi(sm[2]);
    int to = std::stoi(sm[3]);
    for (int i = 0;i < number_to_move; i++) {
	crate_stack[to-1].emplace(crate_stack[from-1].top());
	crate_stack[from-1].pop();
    }
}

void printAnswer(CrateStack const& crate_stack) {
    std::cout << "Answer is: ";
    for (auto & s : crate_stack) {
	if (s.empty()) {
	    continue;
	}
	std::cout << s.top();
    }
    std::cout << "\n";
}

int main()
{
    int total = 0;
    auto reader = FileReader("data.dat");
    CrateStack crateStack = getStacks(reader);
    while (reader.good()) {
	moveCrates(reader, crateStack);
    }
    printAnswer(crateStack);
    while (reader.good()) {
	std::string line = reader.nextLine();
    }

    return 0;
}
