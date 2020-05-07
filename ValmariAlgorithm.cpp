/* Code below reproduced from "Fast brief practical DFA minimization" by Antti Valmari (2011) */

//
// Created by covariance on 06.05.2020.
//

#include "DFA.h"

class valmari_minimizer {

public:
    DFA minimize(const DFA& dfa) {
        return dfa;
    }
};

DFA DFA::minimize() {
    valmari_minimizer A;
    return A.minimize(*this);
}