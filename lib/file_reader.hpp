#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <fstream>

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

#endif
