#include <filesystem>
#include <string>
#include <vector>

struct TreeEntry {
	std::string name;
	std::string hash;
	bool is_tree;
};

class Tree {
  public:
	std::string write();
	std::string serialize();
	void add_entry(const TreeEntry &entry) { entries.push_back(entry); }

	static Tree build_tree(const std::filesystem::path &dir,
	                       bool debug = false);

  private:
	std::vector<TreeEntry> entries;
};
