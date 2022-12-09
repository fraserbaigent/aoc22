#ifndef FILE_HANDLE_HPP
#define FILE_HANDLE_HPP

#include <memory>
#include <unordered_map>
#include <string>

class FileHandle {
private:
    std::shared_ptr<FileHandle> _parent;
    std::unordered_map<std::string, std::shared_ptr<const FileHandle>> _children;
    std::string _path;
    mutable int _size { 0 };
    mutable bool _size_cached { false };
public:
    FileHandle(std::shared_ptr<FileHandle> const& parent, std::string const& path)
	: _parent{parent}
	, _path{path}
    {
    };

    FileHandle(int size, std::string const& path)
	: _path{path}
	, _size{size}
	, _size_cached{true}
    {
    }

    void addFile(std::string const& file_name, int file_size) {
	auto it = _children.find(file_name);
	if (it == _children.cend()) {
	    _children[file_name] = std::make_shared<FileHandle>(file_size, file_name);
	}
    }

    void addChild(std::shared_ptr<FileHandle> const& child) {
	_children[child->getPath()] = child;
    }
    
    std::string getPath() const {
	return _path;
    }
    
    int getTotalSize() const {
	if (!_size_cached) {
	    _size_cached = true;
	    _size = 0;
	    for (auto && child : _children) {
		_size += child.second->getTotalSize();
	    }
	}
	return _size;
    }
    
    std::shared_ptr<FileHandle> getParent() const {
	return _parent;
    }
};


#endif
