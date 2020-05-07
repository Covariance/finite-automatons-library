//
// Created by covariance on 07.05.2020.
//

#include "DFA.h"

DFA::DFA() : n(),
             terminal(),
             edges(),
             _significant_calculated(false),
             _cyclic_calculated(false),
             _cyclic(false),
             _inverse_calculated(false) {}

DFA::DFA(int n, const std::vector<int> &terminalSet, const std::vector<std::pair<char, std::pair<int, int>>> &edgeset)
        : n(n),
          terminal(n, false),
          edges(n, std::unordered_map<char, int>()),
          _significant_calculated(false),
          _cyclic_calculated(false),
          _cyclic(false),
          _inverse_calculated(false) {
    for (int term : terminalSet) {
        terminal[term - 1] = true;
    }
    for (auto edge : edgeset) {
        edges[edge.second.first - 1].insert({edge.first, edge.second.second - 1});
    }
}

