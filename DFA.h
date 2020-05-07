//
// Created by covariance on 06.05.2020.
//

#ifndef FINITE_AUTOMATONS_LIBRARY_DFA_H
#define FINITE_AUTOMATONS_LIBRARY_DFA_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <iostream>

class DFA {
private:
    int n;
    std::vector<bool> terminal;
    std::vector<std::unordered_map<char, int>> edges;

    bool _inverse_calculated;
    std::vector<std::vector<int>> _inverse;

    void _calculate_inverse();

    bool _significant_calculated;
    std::vector<bool> _significant;

    void _significance_dfs_terminal(int v);
    void _significance_dfs_starting(std::vector<bool> *tmp, int v);
    void _calculate_significant();

    bool _cyclic_calculated;
    bool _cyclic;

    bool _cyclic_dfs(std::vector<int> *state, int v);
    void _calculate_cyclic();

    void _word_count(std::vector<int> *val, int v, int mod);

    bool _iso_dfs(std::vector<int> *iso, const DFA& that, int v);

    [[nodiscard]] int _from_by_way(int from, char by) const;
    [[nodiscard]] bool _terminal(int v) const;

public:
    DFA();

    DFA(int n, const std::vector<int> &terminalSet, const std::vector<std::pair<char, std::pair<int, int>>> &edgeset);

    friend std::ostream &operator<<(std::ostream &out, const DFA &dka);

    friend std::istream &operator>>(std::istream &in, DFA &dka);

    bool operator==(const DFA &that);

    bool accepts(const std::string &s);

    int word_count(int mod);

    int word_count(int l, int mod);

    std::vector<int> iso(const DFA& that);

    bool is_iso(const DFA& that);

    DFA minimize();
};

std::ostream &operator<<(std::ostream &out, const DFA &dka);

std::istream &operator>>(std::istream &in, DFA &dka);

#endif //FINITE_AUTOMATONS_LIBRARY_DFA_H
