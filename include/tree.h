// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

class PMTree {
 public:
  struct Node {
    char value;
    std::vector<Node*> children;
    size_t count;
    explicit Node(char v = '\0') : value(v), count(0) {}
  };

  explicit PMTree(const std::vector<char>& symbols);
  ~PMTree();

  Node* getRoot() const { return root_; }

 private:
  Node* root_;

  Node* build(const std::vector<char>& available);
  void deleteNode(Node* node);
  void computeCounts(Node* node);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
