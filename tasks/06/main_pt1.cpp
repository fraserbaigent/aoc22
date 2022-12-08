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

int findStartIndex(std::string const& buffer) {
    int start = 0;
    int end = 0;

    static constexpr int WIDTH = 4;

    while (end < buffer.size() - 1) {	
	if (end - start == WIDTH-1) {
	    break;
	}

	end++;
	for (int j = end - 1; j >= start && j >= 0 && j > end - 4; --j) {
	    if (buffer[end] == buffer[j]) {
		start = j+1;
		break;
	    }
	}

    }
    return end + 1;
}

void testFindStart() {
    std::unordered_map<std::string, int> test_cases {
	{"bvwbjplbgvbhsrlpgdmjqwftvncz" , 5},
	{"nppdvjthqldpwncqszvftbrmjlhg", 6},
	{"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10},
	{"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11},
    };

    for (auto & test : test_cases) {
	int result = findStartIndex(test.first);
	if (result  != test.second) {
	    std::cout << "Test failed: " << test.first << " expected " << test.second <<
		" received " << result <<"\n";
	} else {
	    std::cout << "Test passed: " << test.first <<"\n";
	}
    }    
}

int main()
{
    testFindStart();
    auto reader = FileReader("data.dat");
    std::string data_line = reader.nextLine();
    int index = findStartIndex(data_line);
    std::cout << "Answer : " << index <<"\n";

    return 0;
}
