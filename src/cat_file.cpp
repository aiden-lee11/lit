#include "cat_file.h"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <zlib.h>

void cat_file(const std::string object_id) {
	// todo add better handling for the object id length and stuff
	// for now hardcoding to objects i think this is fine?
	std::filesystem::path filename =
	    ".lit/objects/" + object_id.substr(0, 2) + "/" + object_id.substr(2);

	uLong compressed_size = std::filesystem::file_size(filename);

	std::ifstream inputFile(filename, std::ios::binary);
	if (!inputFile.is_open()) {
		std::cerr << "error couldnt open file " << filename << std::endl;
		return;
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	std::string compressed_data = buffer.str();
	const char *dataPtr = compressed_data.c_str();

	Bytef *compressed_ptr =
	    reinterpret_cast<Bytef *>(const_cast<char *>(dataPtr));

	// maybe change ratio of alloc at some point
	int compress_ratio = 5;

	uLong uncompresed_size = compressed_size * compress_ratio;
	Bytef *uncompressed_data = new Bytef[uncompresed_size];

	int result = uncompress(uncompressed_data, &uncompresed_size,
	                        compressed_ptr, compressed_size);

	if (result == Z_OK) {
		std::string uncompressed_string(
		    reinterpret_cast<const char *>(uncompressed_data),
		    uncompresed_size);

		int end_of_header = uncompressed_string.find('\0');
		uncompressed_string = uncompressed_string.substr(end_of_header);
		std::cout << uncompressed_string << std::endl;
	} else {
		std::cerr << "error uncompressing file" << std::endl;
	}

	delete[] uncompressed_data;
}
