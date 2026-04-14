# Lab 3 – NFA to DFA Conversion

## Objective

To implement the Subset Construction algorithm for converting a Non-deterministic Finite Automaton (NFA) into an equivalent Deterministic Finite Automaton (DFA). Students will employ epsilon closure computations and systematic state enumeration to construct a deterministic machine that recognizes the same language as the given NFA.

## Theoretical Background

The Subset Construction (also called the powerset construction method) transforms an NFA into a DFA that accepts the same language (Hopcroft et al., 2006). The algorithm is based on the fundamental theorem of automata: for every NFA, there exists an equivalent DFA.

The key insight is that each DFA state corresponds to a subset of NFA states. Specifically, a DFA state represents the set of NFA states reachable via epsilon transitions and the input symbols processed so far. The algorithm maintains an invariant: after processing input w, the DFA is in state corresponding to the epsilon closure of the NFA's reachable states.

Formal definition: Given NFA N = (Q_N, Σ, δ_N, q_0, F_N), the equivalent DFA D = (Q_D, Σ, δ_D, q_0', F_D) where:
- Q_D ⊆ 2^{Q_N} (states are subsets of NFA states)
- q_0' = ε-closure(q_0) (DFA start state)
- For each subset S of NFA states and symbol a: δ_D(S, a) = ε-closure(move(S, a))
- F_D contains all subsets containing at least one NFA accept state

The algorithm employs two key operations:
- ε-closure(S): Compute all NFA states reachable from S via epsilon transitions
- move(S, a): Compute all NFA states reachable from S via single 'a' transition

A final optimization step can minimize the DFA using equivalence class partitioning (Hopcroft minimization), though the basic subset construction typically produces acceptable results for academic purposes.

## Algorithm

**Subset Construction (DFA from NFA):**

1. Initialize Q_D ← ∅, ε_closure_start ← ε-closure({q_0})
2. q_0' ← new DFA state representing ε_closure_start
3. Add q_0' to unprocessed queue, mark as start state
4. **While unprocessed states remain**:
   - Pop state T from queue
   - **For each symbol a in Σ**:
     - U ← ε-closure(move(T, a))
     - **If U ∉ Q_D**:
       - Add U as new DFA state
       - Enqueue U
     - Add δ_D(T, a) = U
5. **For each DFA state T**:
   - **If T ∩ F_N ≠ ∅**: Mark T as accept
6. **Return** DFA(Q_D, Σ, δ_D, q_0', F_D)

**ε-Closure Computation (Recursive):**
1. closure ← {s}
2. **Repeat**:
   - **For each transition (p, ε, q)** in NFA:
     - **If p ∈ closure and q ∉ closure**:
       - closure ← closure ∪ {q}
3. **Until no change**
4. **Return** closure

## Example Used

```
NFA for regular expression: (a|b)*abb
```

The NFA contains 8 states and accepts strings that end with "abb" after any combination of 'a' and 'b'. The subset construction will generate a DFA with fewer states and all deterministic transitions, suitable for single-pass linear scanning.

## Program Structure

- **`set_create()`**: Initialize empty state set. Time: O(1).
- **`set_add(s, state)`**: Add state to set. Time: O(1).
- **`set_contains(s, state)`**: Test membership. Time: O(1).
- **`set_equal(s1, s2)`**: Equality test. Time: O(1).
- **`epsilon_closure(nfa, states)`**: Compute epsilon closure. Time: O(n·m) where n is state count, m is transition count.
- **`move(nfa, states, symbol)`**: Compute reachable states. Time: O(m).
- **`nfa_init_hardcoded(nfa)`**: Initialize example NFA. Time: O(1).
- **`dfa_init(dfa)`**: Initialize empty DFA. Time: O(1).
- **`add_dfa_state(dfa, nfa_states, is_accept)`**: Add DFA state. Time: O(1).
- **`find_dfa_state(dfa, nfa_states)`**: Locate DFA state. Time: O(d) where d is DFA states.
- **`subset_construction(nfa, dfa)`**: Main conversion algorithm. Time: O(2^n·m) worst-case, O(n·m) average.
- **`display_dfa_table(dfa)`**: Output transition table. Time: O(d·a).
- **`display_dfa_transitions_detailed(dfa)`**: Detailed state diagram. Time: O(d·a).

## How to Compile and Run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o nfa_to_dfa nfa_to_dfa.c
./nfa_to_dfa
```

## Sample Output

```
Regular Expression: (a|b)*abb
Converting NFA to equivalent DFA via Subset Construction...

Input NFA:
  States: 0 - 7
  Start: 0
  Accept: 7
  Transitions: 9

================================================================
            DFA TRANSITION TABLE
================================================================
DFA State    On 'a'          On 'b'          Accept?        
----------------------------------------------------------------
0             →0              →1              NO            
1             →0              →2              NO            
2             →0              →3              NO            
3             →0              →1              YES           
================================================================

================================================================
            DFA TRANSITIONS (DETAILED)
================================================================

State 0 [START]:
  --a--> State 0
  --b--> State 1

State 1:
  --a--> State 0
  --b--> State 2

State 2:
  --a--> State 0
  --b--> State 3

State 3 [ACCEPT]:
  --a--> State 0
  --b--> State 1

================================================================

Status: NFA to DFA conversion completed successfully.
Converted 8 NFA states to 4 DFA states.
```

## Complexity Analysis

| Metric | Complexity |
|--------|-----------|
| Time Complexity | O(2^n · m) worst-case; O(n · m) practical average |
| Space Complexity | O(2^n) for DFA state storage |

Worst-case exponential complexity occurs when all NFA states are reachable via symbol transitions, creating a maximally-sized DFA. However, in practice, many NFA patterns produce DFAs with manageable state counts linear or polynomial in the NFA size. The subset construction is asymptotically optimal among determinization algorithms.

## References

- Aho, A.V., Lam, M.S., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Hopcroft, J.E., Motwani, R., & Ullman, J.D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson Education.
- Rabin, M.O., & Scott, D. (1959). Finite Automata and Their Decision Problems. *IBM Journal of Research and Development*, 3(2), 115-125.

---
