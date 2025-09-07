#include "cat-file/cat-file.h"
#include "hash-object/hash-object.h"
#include "init/init.h"
#include "write-tree/write-tree.h"

#include <filesystem>
#include <iostream>
#include <string>
int main(int argc, char **argv) {
	std::string cmd = argv[1];

	if (cmd == "init") {
		run_init();
	} else if (cmd == "hash-object") {
		// lit hash-object <filename>
		hash_object(argv[2]);
	} else if (cmd == "cat-file") {
		// lit cat-file <filename>
		cat_file(argv[2]);
	} else if (cmd == "write-tree") {
		// default to writing the staging area
		// for now we'll just do the cwd but after add is implemented we should
		// do the staging area
		if (argc == 2) {
			std::filesystem::path cwd = std::filesystem::current_path();

			Tree root = Tree::build_tree(cwd);

			std::string hash = root.write();
			std::cout << hash << std::endl;
		}
	};
}
