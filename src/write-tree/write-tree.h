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
	void add_entry(const TreeEntry &entry) { entries.push_back(entry); }

	static Tree build_tree(const std::filesystem::path &dir);

  private:
	std::vector<TreeEntry> entries;
};
