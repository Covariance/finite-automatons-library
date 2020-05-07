//
// Created by covariance on 07.05.2020.
//

#include "DFA.h"

std::ostream &operator<<(std::ostream &out, const DFA &dka) {
    int m = 0, k = 0;
    for (bool term : dka.terminal) {
        if (term) {
            k++;
        }
    }
    for (const auto& edge : dka.edges) {
        m += edge.size();
    }
    out << dka.n << ' ' << m << ' ' << k << '\n';
    for (int i = 0; i < dka.n; i++) {
        if (dka.terminal[i]) {
            out << i + 1 << ' ';
        }
    }
    out << '\n';
    for (int i = 0; i < dka.n; i++) {
        for (auto edge : dka.edges[i]) {
            out << i + 1 << ' ' << edge.second + 1 << ' ' << edge.first;
        }
    }
    return out;
}

std::istream &operator>>(std::istream &in, DFA &dka) {
    int m = 0, k = 0;
    in >> dka.n >> m >> k;
    dka.terminal.clear();
    dka.terminal.resize(dka.n, false);
    for (int i = 0; i < k; i++) {
        int tmp = 0;
        in >> tmp;
        dka.terminal[tmp - 1] = true;
    }
    dka.edges.clear();
    dka.edges.resize(dka.n, std::unordered_map<char, int>());
    for (int i = 0; i < m; i++) {
        int from = 0, to = 0;
        char ch = 0;
        in >> from >> to >> ch;
        dka.edges[from - 1].insert({ch, to - 1});
    }
    return in;
}

struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

bool DFA::operator==(const DFA &that) {
    std::pair<int, int> tmp;
    std::queue<std::pair<int, int>> bfs;
    std::unordered_set<std::pair<int, int>, pair_hash> was;
    was.insert({0, 0});
    bfs.push({0, 0});
    while (!bfs.empty()) {
        std::pair<int, int> cur = bfs.front();
        bfs.pop();
        if (this->_terminal(cur.first) != that._terminal(cur.second)) {
            return false;
        }
        if (cur.first != -1) {
            for (auto edge : this->edges[cur.first]) {
                tmp = {this->_from_by_way(cur.first, edge.first), that._from_by_way(cur.second, edge.first)};
                if (was.find(tmp) == was.end()) {
                    was.insert(tmp);
                    bfs.push(tmp);
                }
            }
        }
        if (cur.second != -1) {
            for (auto edge : that.edges[cur.second]) {
                tmp = {this->_from_by_way(cur.first, edge.first), that._from_by_way(cur.second, edge.first)};
                if (was.find(tmp) == was.end()) {
                    was.insert(tmp);
                    bfs.push(tmp);
                }
            }
        }
    }
    return true;
}