// Copyright 2022 NNTU-CS
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "tree.h"

int main() {
  std::vector<char> start = {'1', '2', '3'};
  PMTree demo(start);

  auto all = getAllPerms(demo);
  for (const auto& seq : all) {
    for (char c : seq) std::cout << c;
    std::cout << ' ';
  }
  std::cout << "\n\n";

  auto p1 = getPerm1(demo, 1);
  std::cout << "getPerm1(1) = ";
  for (char c : p1) std::cout << c;
  std::cout << std::endl;

  auto p2 = getPerm2(demo, 2);
  std::cout << "getPerm2(2) = ";
  for (char c : p2) std::cout << c;
  std::cout << "\n\n";

  std::ofstream out("result/times.csv");
  out << "n,getAllPerms_s,getPerm1_s,getPerm2_s\n";

  std::random_device rd;
  std::mt19937 gen(rd());

  for (int n = 3; n <= 8; ++n) {
    std::vector<char> symbols;
    for (int i = 0; i < n; ++i) symbols.push_back('a' + i);
    PMTree tree(symbols);

    size_t maxPerms = 1;
    for (int i = 2; i <= n; ++i) maxPerms *= i;

    std::uniform_int_distribution<size_t> dist(1, maxPerms);
    size_t target = dist(gen);

    auto t0 = std::chrono::steady_clock::now();
    volatile auto allPerms = getAllPerms(tree);
    (void)allPerms;
    auto t1 = std::chrono::steady_clock::now();

    auto t2 = std::chrono::steady_clock::now();
    volatile auto perm1 = getPerm1(tree, target);
    (void)perm1;
    auto t3 = std::chrono::steady_clock::now();

    auto t4 = std::chrono::steady_clock::now();
    volatile auto perm2 = getPerm2(tree, target);
    (void)perm2;
    auto t5 = std::chrono::steady_clock::now();

    double secAll = std::chrono::duration<double>(t1 - t0).count();
    double sec1 = std::chrono::duration<double>(t3 - t2).count();
    double sec2 = std::chrono::duration<double>(t5 - t4).count();

    std::cout << "n = " << n << " (maxPerms=" << maxPerms << ")\n"
              << "  getAllPerms: " << secAll << " s\n"
              << "  getPerm1:    " << sec1 << " s\n"
              << "  getPerm2:    " << sec2 << " s\n\n";

    out << n << "," << secAll << "," << sec1 << "," << sec2 << "\n";
  }
  out.close();
  return 0;
}
