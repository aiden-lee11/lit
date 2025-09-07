#include "hash-object.h"
#include "TinySHA1.hpp"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <zlib.h>

// taken from online reference
std::string deflate_string(const std::string &input_string,
                           int compression_level = Z_DEFAULT_COMPRESSION) {
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	int ret = deflateInit(&strm, compression_level);
	if (ret != Z_OK) {
		std::cerr << "deflateInit failed: " << ret << std::endl;
		return "";
	}

	strm.avail_in = input_string.length();
	strm.next_in =
	    reinterpret_cast<Bytef *>(const_cast<char *>(input_string.data()));

	unsigned long compressed_buffer_size =
	    deflateBound(&strm, input_string.length());
	std::vector<char> compressed_data(compressed_buffer_size);

	strm.avail_out = compressed_buffer_size;
	strm.next_out = reinterpret_cast<Bytef *>(compressed_data.data());

	ret = deflate(&strm, Z_FINISH);
	if (ret != Z_STREAM_END) {
		std::cerr << "deflate failed: " << ret << std::endl;
		deflateEnd(&strm);
		return "";
	}

	size_t actual_compressed_size = compressed_buffer_size - strm.avail_out;

	deflateEnd(&strm);

	return std::string(compressed_data.data(), actual_compressed_size);
}

std::string write_object_to_store(const std::string object_data) {
	// add in here the header once we determine if blob or tree etc
	sha1::SHA1 s;
	s.processBytes(object_data.data(), object_data.size());

	uint8_t hash_bytes[20];
	s.getDigestBytes(hash_bytes);

	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (int i = 0; i < 20; i++) {
		oss << std::setw(2) << (int)hash_bytes[i];
	}
	std::string hexStr = oss.str();

	std::string subDir = ".lit/objects/" + hexStr.substr(0, 2) + "/";
	std::string path = subDir + hexStr.substr(2);

	std::filesystem::create_directory(subDir);

	std::ofstream objFile(path);
	std::string compressed_file_content = deflate_string(object_data);
	objFile.write(compressed_file_content.data(),
	              compressed_file_content.size());
	objFile.close();

	return hexStr;
}

std::string hash_object(const std::string filename, bool verbose) {
	std::ifstream inputFile(filename, std::ios::binary | std::ios::ate);
	if (!inputFile.is_open()) {
		std::cerr << "error couldnt open file " << filename << std::endl;
		return "";
	}

	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);

	std::string content(size, '\0');
	if (!inputFile.read(content.data(), size)) {
		std::cerr << "error reading file " << filename << std::endl;
		return "";
	}

	std::string header = "blob " + std::to_string(content.size());
	header.push_back('\0');
	std::string store = header + content;

	std::string hash = write_object_to_store(store);
	if (verbose) {
		std::cout << hash << std::endl;
	}

	return hash;
}
