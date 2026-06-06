// Copyright 2022 NNTU-CS
#include "tree.h"

#include <algorithm>
#include <functional>

PMTree::PMTree(const std::vector<char>& symbols) {
  root_ = build(symbols);
  computeCounts(root_);
}

PMTree::~PMTree() {
  deleteNode(root_);
}

PMTree::Node* PMTree::build(const std::vector<char>& available) {
  if (available.empty()) return nullptr;
  Node* node = new Node();
  for (char ch : available) {
    Node* child = new Node(ch);
    std::vector<char> remaining;
    for (char c : available) {
      if (c != ch) remaining.push_back(c);
    }
    Node* subtree = build(remaining);
    child->children = subtree ? subtree->children : std::vector<Node*>();
    node->children.push_back(child);
  }
  std::sort(node->children.begin(), node->children.end(),
            [](Node* a, Node* b) { return a->value < b->value; });
  return node;
}

void PMTree::deleteNode(Node* node) {
  if (!node) return;
  for (Node* child : node->children) deleteNode(child);
  delete node;
}

void PMTree::computeCounts(Node* node) {
  if (!node) return;
  if (node->children.empty()) {
    node->count = 1;
    return;
  }
  size_t total = 0;
  for (Node* child : node->children) {
    computeCounts(child);
    total += child->count;
  }
  node->count = total;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  std::vector<std::vector<char>> result;
  std::vector<char> path;
  std::function<void(PMTree::Node*)> dfs = [&](PMTree::Node* node) {
    if (!node) return;
    if (node->value != '\0') path.push_back(node->value);
    if (node->children.empty()) {
      result.push_back(path);
    } else {
      for (PMTree::Node* child : node->children) dfs(child);
    }
    if (!path.empty() && path.back() == node->value) path.pop_back();
  };
  dfs(tree.getRoot());
  return result;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  if (num < 1) return {};
  auto all = getAllPerms(tree);
  if (static_cast<size_t>(num) > all.size()) return {};
  return all[num - 1];
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  if (num < 1) return {};
  PMTree::Node* node = tree.getRoot();
  std::vector<char> result;
  size_t remaining = static_cast<size_t>(num);
  while (node && !node->children.empty()) {
    PMTree::Node* chosen = nullptr;
    for (PMTree::Node* child : node->children) {
      if (remaining <= child->count) {
        chosen = child;
        break;
      } else {
        remaining -= child->count;
      }
    }
    if (!chosen) return {};
    result.push_back(chosen->value);
    node = chosen;
  }
  return result;
}
