#ifndef HASH_OBJECT_H
#define HASH_OBJECT_H

#include <string>
std::string hash_object(const std::string filename, bool verbose = true);

std::string write_object_to_store(const std::string object_data);

#endif
