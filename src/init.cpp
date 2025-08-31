#include "init.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

void run_init() {
  std::array<std::filesystem::path, 3> paths = {".lit", ".lit/refs",
                                                ".lit/objects"};

  std::string head_file = ".lit/HEAD";

  try {
    for (auto dir_path : paths) {
      if (std::filesystem::create_directories(dir_path)) {
        std::cout << "directory created successfully" << dir_path << std::endl;
      } else {
        std::cout << "directory failed to init" << dir_path << std::endl;
      }
    }

    std::ofstream HeadFile(head_file);

    HeadFile << "ref: refs/heads/master\n";
    HeadFile.close();

  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "error creating dir" << e.what() << std::endl;
  }
}
