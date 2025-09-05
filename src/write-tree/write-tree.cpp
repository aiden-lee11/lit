#include "write-tree.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Tree Tree::build_tree(const fs::path &dir) {

	for (const auto &entry : fs::directory_iterator(dir)) {
		std::cout << entry << std::endl;
	}

	return {};
}
