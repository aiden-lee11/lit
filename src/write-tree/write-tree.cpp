#include "write-tree.h"
#include "../hash-object/hash-object.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

Tree Tree::build_tree(const fs::path &dir, bool debug) {
	Tree tree;

	for (const auto &entry : fs::directory_iterator(dir)) {
		if (entry.path().filename().string()[0] == '.') {
			if (debug) {
				std::cout << "skipping hidden file" << entry << std::endl;
			}
			continue;
		}

		if (debug) {
			std::cout << "writing " << entry << std::endl;
		}

		if (entry.is_regular_file()) {
			std::string hash = hash_object(entry.path(), false);
			TreeEntry fileEntry = {entry.path().filename().string(), hash,
			                       false};

			tree.add_entry(fileEntry);
		} else if (entry.is_directory()) {
			Tree subtree = Tree::build_tree(entry.path());
			std::string hash = subtree.write();
			TreeEntry fileEntry = {entry.path().string(), hash, false};

			tree.add_entry(fileEntry);
		}
	}

	return tree;
}

char hex_char_to_val(char c) {
	if ('0' <= c && c <= '9')
		return c - '0';
	if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	if ('A' <= c && c <= 'F')
		return c - 'A' + 10;

	// q is bad return cause why not
	return 'Q';
};

std::string hex_to_bytes(const std::string &hex) {
	std::string res;
	res.reserve(hex.size() / 2);

	for (size_t i = 0; i < hex.size(); i += 2) {
		char upper = hex_char_to_val(hex[i]);
		char lower = hex_char_to_val(hex[i + 1]);

		res.push_back((upper << 4) | lower);
	}

	return res;
}

std::string Tree::serialize() {
	std::string res;

	for (const auto &e : entries) {
		res += e.name;
		res.push_back('\0');
		res += hex_to_bytes(e.hash);
	}

	return res;
}

std::string Tree::write() {
	std::string body = serialize();
	std::string header = "tree" + std::to_string(body.size());
	header.push_back('\0');

	std::string store = header + body;

	return write_object_to_store(store);
}
