//
// Created by covariance on 07.05.2020.
//

#include "DFA.h"

void DFA::_calculate_inverse() {
    if (_inverse_calculated) {
        return;
    }
    _inverse = std::vector<std::vector<int>>(n, std::vector<int>());
    for (int i = 0; i < n; i++) {
        for (auto edge : edges[i]) {
            _inverse[edge.second].push_back(i);
        }
    }
    _inverse_calculated = true;
}

void DFA::_significance_dfs_terminal(int v) {
    _significant[v] = true;
    for (int son : _inverse[v]) {
        if (!_significant[son]) {
            _significance_dfs_terminal(son);
        }
    }
}

void DFA::_significance_dfs_starting(std::vector<bool> *tmp, int v) {
    (*tmp)[v] = true;
    for (auto son : edges[v]) {
        if (!(*tmp)[son.second]) {
            _significance_dfs_starting(tmp, son.second);
        }
    }
}

void DFA::_calculate_significant() {
    if (_significant_calculated) {
        return;
    }
    _calculate_inverse();
    _significant = std::vector<bool>(n, false);
    for (int i = 0; i < n; i++) {
        if (terminal[i]) {
            _significance_dfs_terminal(i);
        }
    }
    std::vector<bool> tmp(n, false);
    _significance_dfs_starting(&tmp, 0);
    for (int i = 0; i < n; i++) {
        _significant[i] = _significant[i] & tmp[i];
    }
    _significant_calculated = true;
}

bool DFA::_cyclic_dfs(std::vector<int> *state, int v) {
    (*state)[v] = 1;
    for (auto son : edges[v]) {
        if (_significant[son.second]) {
            if ((*state)[son.second] == 1) {
                return true;
            }
            if ((*state)[son.second] == 0 && _cyclic_dfs(state, son.second)) {
                return true;
            }
        }
    }
    (*state)[v] = 2;
    return false;
}

void DFA::_calculate_cyclic() {
    if (_cyclic_calculated) {
        return;
    }
    _calculate_significant();
    std::vector<int> state(n, 0);
    _cyclic = _cyclic_dfs(&state, 0);
    _cyclic_calculated = true;
}

void DFA::_word_count(std::vector<int> *val, int v, int mod) {
    long long sum = 0;
    for (int son : _inverse[v]) {
        if (_significant[son]) {
            if ((*val)[son] == 0) {
                _word_count(val, son, mod);
            }
            sum = (sum + (*val)[son]) % mod;
        }
    }
    (*val)[v] = (int) sum;
}

bool DFA::_iso_dfs(std::vector<int> *iso, const DFA &that, int v) {
    int that_v = (*iso)[v];
    if (that_v == -1 || terminal[v] != that.terminal[that_v]) {
        return false;
    }
    for (auto edge : edges[v]) {
        auto that_edge = that.edges[that_v].find(edge.first);
        if (that_edge == that.edges[that_v].end()) {
            (*iso)[v] = -1;
            return false;
        }
        if ((*iso)[edge.second] == -1) {
            (*iso)[edge.second] = that_edge->second;
            if (!_iso_dfs(iso, that, edge.second)) {
                return false;
            }
        }
        if ((*iso)[edge.second] != that_edge->second) {
            (*iso)[v] = -1;
            return false;
        }
    }
    return true;
}

[[nodiscard]] int DFA::_from_by_way(int from, char by) const {
    if (from == -1) return -1;
    if (edges[from].find(by) == edges[from].end()) return -1;
    return edges[from].find(by)->second;
}

[[nodiscard]] bool DFA::_terminal(int v) const {
    return (v != -1) && (this->terminal[v]);
}

bool DFA::accepts(const std::string &s) {
    int cur_vert = 0;
    std::unordered_map<char, int>::const_iterator got;
    for (char ch : s) {
        got = this->edges[cur_vert].find(ch);
        if (got == this->edges[cur_vert].end()) {
            return false;
        }
        cur_vert = this->edges[cur_vert].find(ch)->second;
    }
    return this->terminal[cur_vert];
}

int DFA::word_count(int mod) {
    _calculate_cyclic();
    if (_cyclic) return -1;
    std::vector<int> val(n, 0);
    val[0] = 1;
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        if (terminal[i]) {
            _word_count(&val, i, mod);
            sum = (sum + val[i]) % mod;
        }
    }
    for (int i = 0; i < n; i++) {
        if (_significant[i]) {
            return (int) (sum % mod);
        }
    }
    return 0;
}

int DFA::word_count(int l, int mod) {
    std::vector<int> cur(n, 0), tmp;
    cur[0] = 1;
    for (int i = 0; i < l; i++) {
        tmp.clear();
        tmp.resize(n, 0);
        for (int j = 0; j < n; j++) {
            for (auto edge : edges[j]) {
                tmp[edge.second] = (tmp[edge.second] + cur[j]) % mod;
            }
        }
        cur = tmp;
    }
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        if (terminal[i]) {
            sum += cur[i];
        }
    }
    return (int) (sum % mod);
}

std::vector<int> DFA::iso(const DFA &that) {
    std::vector<int> iso(n, -1);
    iso[0] = 0;
    if (!_iso_dfs(&iso, that, 0)) {
        iso[0] = -1;
    }
    return iso;
}

bool DFA::is_iso(const DFA &that) {
    if (this->n != that.n || this->terminal.size() != that.terminal.size()) {
        return false;
    }
    auto to_check = iso(that);
    for (int to : to_check) {
        if (to == -1) {
            return false;
        }
    }
    return true;
}