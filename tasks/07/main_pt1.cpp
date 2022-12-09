#include "file_handle.hpp"
#include "../../lib/file_reader.hpp"

#include <iostream>
#include <vector>
#include <regex>

class Solver {
private:
    int level { 0 };
    using Command = std::pair<std::string, std::string>;
    std::unordered_map<std::string, std::shared_ptr<FileHandle>> _directories;

    std::shared_ptr<FileHandle> _current_directory;

    using LineData = std::pair<std::string, std::string>;
protected:
    std::shared_ptr<FileHandle> findDirectory(std::string const& name) const {
	auto it = _directories.find(name);
	return it != _directories.cend() ? it->second : nullptr;
    }

    std::string buildPath(std::shared_ptr<FileHandle> const& directory, std::string const& file_or_dir) const {
	return directory->getPath() + "/" + file_or_dir;
    }
    
    void handleCd(std::string const& arg) {
	if (arg == "/") {
	    level = 0;
	    auto dir = findDirectory(arg);
	    if (dir != nullptr) {
		_current_directory = dir;
	    } else {
		_current_directory = std::make_shared<FileHandle>(nullptr, "/");
		_directories[_current_directory->getPath()] = _current_directory;
	    }
	} else if (arg == "..") {
	    level--;
	    if (_current_directory->getParent()) {
		_current_directory = _current_directory->getParent();
	    }
	} else {
	    _current_directory = _directories[buildPath(_current_directory, arg)];
	    level++;
	}
    }

    LineData splitLine(std::string const& line) {
	LineData data;
	auto space = line.find(' ');
	data.first = line.substr(0,space);
	data.second = line.substr(space + 1, line.size() - space);
	return data;
    }

    bool isDirectory(LineData const& data) const {
	return data.first == "dir";
    }

    void handleLs(std::vector<std::string> const& listed) {
	for (auto & line : listed) {
	    LineData line_data = splitLine(line);
	    std::string path = buildPath(_current_directory, line_data.second);
	    if (isDirectory(line_data)) {
		auto dir = findDirectory(line_data.second);
		if (dir == nullptr) {
		    dir = std::make_shared<FileHandle>(_current_directory, path);
		    _directories[path] = dir;
		}
		_current_directory->addChild(dir);
	    } else {
		_current_directory->addFile(path, std::stoi(line_data.first));
	    }
	}
    }

    int getRequiredSpaceForUpdate() const {
	int amount_used = _directories.find("/")->second->getTotalSize();
	static constexpr int TOTAL_REQUIRED = 30000000;
	static constexpr int TOTAL_AVAILABLE = 70000000 ;
	return std::max(TOTAL_REQUIRED - (TOTAL_AVAILABLE - amount_used), 0);
    }
    
    void printResult() const {
	int total = 0;
	int lowest = -1;
	int required_removal = getRequiredSpaceForUpdate();
	static constexpr int max_size_cutoff = 100000;
	std::cout << "Summary of directories:\n";
	for (auto &d : _directories) {
	    std::cout << d.first << " - " << d.second->getTotalSize() << "\n";
	    int total_size = d.second->getTotalSize();
	    if (total_size < max_size_cutoff) {
		total += total_size;
	    }
	    if ((total_size > required_removal) &&
		(lowest <= 0 || 
		 lowest > total_size)) {
		lowest = total_size;
	    }
	    
	}
	std::cout << "Part 1: " << total <<"\n";
	std::cout << "Part 2: " << lowest <<"\n";
    }

    std::shared_ptr<Command> parseCommand(std::string const& line) {
	static std::regex re("^\\$ (\\w+)\\ ?(.*)$");
	std::smatch sm;
	bool result = std::regex_match(line, sm, re);
	return result ?
	    std::make_shared<Command>(sm[1], sm[2]) : nullptr;
    }
public:
    void solve() {
	auto reader = FileReader("data.dat");

	bool ls_ing = false;
	std::vector<std::string> ls_strings;
	while (reader.good()) {
	    std::string line = reader.nextLine();
	    auto command = parseCommand(line);
	    if (command && ls_ing) {
		handleLs(ls_strings);
		ls_strings.clear();
		ls_ing = false;
	    }

	    if (ls_ing) {
		ls_strings.push_back(line);
	    } else if (command) {
		if (command->first == "cd") {
		    handleCd(command->second);
		} else if (command->first == "ls") {
		    ls_ing = true;
		}
	    }
		
	}
	if (ls_ing) {
	    handleLs(ls_strings);
	}       
	printResult();
    };
};

int main()
{
    Solver solver;
    solver.solve();
    return 0;
}
