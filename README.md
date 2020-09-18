# finite-automatons-library
### Fast and optimized library for work with determined and non-determined automatons.
 
FA is a finite-state automaton, sometimes known as finite-state machine,
is a finite automaton that accepts or declines a string based on its structure. This library is dedicated
to working with those automatons - simplification, transformation and other actions.

#### Supported functions
- Reading and writing automaton representations from/to IO streams
- Checking equality of the automatons by structure (different numeration of nodes doesn't matter)
- Checking if automaton accepts the string
- Counting number of words that automaton accepts
- Counting number of words of specific length that automaton accepts
- Checking if automatons are isomorphic to each other and finding that isomorphism
- Minimization algorithm, reproduced from 
["Fast brief practical DFA minimization"](https://www.cs.cmu.edu/~sutner/CDM/resources/Valmari12.pdf)
 by Antti Valmari (2011)
- Transformation from NFA to DFA (from non-deterministic to deterministic form)
