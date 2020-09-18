/* Code below reproduced from "Fast brief practical DFA minimization" by Antti Valmari (2011) */

//
// Created by covariance on 06.05.2020.
//

#include "DFA.h"

int* M, * W, w = 0;
int states, edges, terminal_cnt, starting = 0, reached_states = 0;
int* from, * ch, * to, * A, * F;

struct partition {
  int z, * E, * L, * S, * F, * P;

  void init(int n) {
    z = bool(n);
    E = new int[n];
    L = new int[n];
    S = new int[n];
    F = new int[n];
    P = new int[n];
    for (int i = 0; i < n; ++i) {
      E[i] = L[i] = i;
      S[i] = 0;
    }
    if (z) {
      F[0] = 0;
      P[0] = n;
    }
  }

  void mark(int e) {
    int s = S[e], i = L[e], j = F[s] + M[s];
    E[i] = E[j];
    L[E[i]] = i;
    E[j] = e;
    L[e] = j;
    if (!M[s]++) { W[w++] = s; }
  }

  void split() {
    while (w) {
      int s = W[--w], j = F[s] + M[s];
      if (j == P[s]) {
        M[s] = 0;
        continue;
      }
      if (M[s] <= P[s] - j) {
        F[z] = F[s];
        P[z] = F[s] = j;
      } else {
        P[z] = P[s];
        F[z] = P[s] = j;
      }
      for (int i = F[z]; i < P[z]; ++i) {
        S[E[i]] = z;
      }
      M[s] = M[z++] = 0;
    }
  }
};

partition blocks_partition, cords_partition;


bool cmp(int i, int j) {
  return ch[i] < ch[j];
}

void make_adjacent(const int K[]) {
  for (int i = 0; i <= states; i++) { F[i] = 0; }
  for (int i = 0; i < edges; i++) { F[K[i]]++; }
  for (int i = 0; i < states; i++) { F[i + 1] += F[i]; }
  for (int i = edges; i--;) { A[--F[K[i]]] = i; }
}

inline void reach(int q) {
  int i = blocks_partition.L[q];
  if (i >= reached_states) {
    blocks_partition.E[i] = blocks_partition.E[reached_states];
    blocks_partition.L[blocks_partition.E[i]] = i;
    blocks_partition.E[reached_states] = q;
    blocks_partition.L[q] = reached_states++;
  }
}

void rem_unreachable(int T[], int H[]) {
  make_adjacent(T);
  int tmp;
  for (int i = 0; i < reached_states; i++) {
    for (tmp = F[blocks_partition.E[i]]; tmp < F[blocks_partition.E[i] + 1]; tmp++) {
      reach(H[A[tmp]]);
    }
  }
  tmp = 0;
  for (int i = 0; i < edges; i++) {
    if (blocks_partition.L[T[i]] < reached_states) {
      H[tmp] = H[i];
      ch[tmp] = ch[i];
      T[tmp] = T[i];
      tmp++;
    }
  }
  edges = tmp;
  blocks_partition.P[0] = reached_states;
  reached_states = 0;
}

DFA DFA::minimize() {
  from = new int[this->edges.size()];
  ch = new int[this->edges.size()];
  to = new int[this->edges.size()];
  blocks_partition.init(states);
  A = new int[this->edges.size()];
  F = new int[states + 1];

  std::vector<int> terminal_tmp;

  for (int i = 0; i != this->terminal.size(); ++i) {
    if (this->terminal[i]) {
      terminal_tmp.push_back(i);
    }
  }

  for (int t = 0; t < this->edges.size(); ++t) {
    char tmp;
    std::cin >> from[t] >> to[t] >> tmp;
    ch[t] = (int) (tmp - 'a');
    from[t]--;
    to[t]--;
  }
  reach(starting);
  rem_unreachable(from, to);
  for (int i = 0; i < terminal_cnt; ++i) {
    if (blocks_partition.L[terminal_tmp[i]] < blocks_partition.P[0]) { reach(terminal_tmp[i]); }
  }
  terminal_cnt = reached_states;
  rem_unreachable(to, from);

  // solution
  W = new int[edges + 1];
  M = new int[edges + 1];
  M[0] = terminal_cnt;
  if (terminal_cnt) {
    W[w++] = 0;
    blocks_partition.split();
  }

  cords_partition.init(this->edges.size());
  if (!this->edges.empty()) {
    std::sort(cords_partition.E, cords_partition.E + this->edges.size(), cmp);
    cords_partition.z = M[0] = 0;
    int a = ch[cords_partition.E[0]];
    for (int i = 0; i < this->edges.size(); ++i) {
      int t = cords_partition.E[i];
      if (ch[t] != a) {
        a = ch[t];
        cords_partition.P[cords_partition.z++] = i;
        cords_partition.F[cords_partition.z] = i;
        M[cords_partition.z] = 0;
      }
      cords_partition.S[t] = cords_partition.z;
      cords_partition.L[t] = i;
    }
    cords_partition.P[cords_partition.z++] = this->edges.size();
  }

  make_adjacent(to);
  int b = 1, c = 0, j;
  while (c < cords_partition.z) {
    for (int i = cords_partition.F[c]; i < cords_partition.P[c]; i++) {
      blocks_partition.mark(from[cords_partition.E[i]]);
    }
    blocks_partition.split();
    c++;
    while (b < blocks_partition.z) {
      for (int i = blocks_partition.F[b]; i < blocks_partition.P[b]; i++) {
        for (
          j = F[blocks_partition.E[i]];
          j < F[blocks_partition.E[i] + 1]; ++j
          ) {
          cords_partition.mark(A[j]);
        }
      }
      cords_partition.split();
      b++;
    }
  }
  int mo = 0, fo = 0;
  for (int t = 0; t < this->edges.size(); ++t) {
    if (blocks_partition.L[from[t]] == blocks_partition.F[blocks_partition.S[from[t]]]) {
      mo++;
    }
  }
  for (int i = 0; i != blocks_partition.z; ++i) {
    if (blocks_partition.F[i] < terminal_cnt) { ++fo; }
  }

  // re-ordering in some cases
  int nn = blocks_partition.z;
  int* biection = new int[nn];
  for (int i = 0; i < nn; i++) {
    biection[i] = i;
  }
  biection[0] = blocks_partition.S[starting];
  biection[blocks_partition.S[starting]] = 0;


  if (nn == 1 && fo == 0) {
    return DFA();
  }

  // print
  std::cout << nn << ' ' << mo
            << ' ' << fo << '\n';
  for (int i = 0; i != blocks_partition.z; ++i) {
    if (blocks_partition.F[i] < terminal_cnt) {
      std::cout << biection[i] + 1 << '\n';
    }
  }
  for (int t = 0; t != this->edges.size(); ++t) {
    if (blocks_partition.L[from[t]] == blocks_partition.F[blocks_partition.S[from[t]]]) {
      std::cout << biection[blocks_partition.S[from[t]]] + 1 << ' '
                << biection[blocks_partition.S[to[t]]] + 1
                << ' '
                << (char) (ch[t] + 'a') << '\n';
    }
  }

  return DFA();
}
