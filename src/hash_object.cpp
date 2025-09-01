#include "hash_object.h"
#include "TinySHA1.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

void hash_object(const std::string filename) {
	std::ifstream inputFile(filename);

	if (!inputFile.is_open()) {
		std::cerr << "error couldnt open file" << filename << std::endl;
		return;
	}

	std::stringstream buffer;
	buffer << inputFile.rdbuf();
	std::string content = buffer.str();

	// add in here the header once we determine if blob or tree etc
	sha1::SHA1 s;
	s.processBytes(content.c_str(), content.size());

	uint32_t digest[5];
	s.getDigest(digest);
	char tmp[48];
	snprintf(tmp, 45, "%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2],
	         digest[3], digest[4]);
	std::string hexStr(tmp);
	std::string subDir = ".lit/objects/" + hexStr.substr(0, 2) + "/";
	std::string path = subDir + hexStr.substr(2);

	std::filesystem::create_directory(subDir);

	std::ofstream objFile(path);
	objFile << "hashed content will be here eventually :D" << std::endl;
	objFile.close();
}
