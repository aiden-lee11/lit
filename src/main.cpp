#include "cat_file.h"
#include "hash_object.h"
#include "init.h"
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
	}
}
