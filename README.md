# Static Verification of C++ Code

## 1. Objective

Perform static verification of `game.cpp`: find defects without running the program, record them in a review checklist and a code review report, and fix them.

## 2. Tools Used

- Manual code review (checklist based)
- cppcheck 2.22.0
- g++ 13.3.0 with `-Wall -Wextra`
- AddressSanitizer and UndefinedBehaviorSanitizer (`-fsanitize=address,undefined`), used to confirm some defects and to test the fixed code

## 3. Deliverables

- [Table No. 1: Review Checklist](review_checklist.md)
- [Table No. 2: Code Review Report](code_review_report.md)
- [Original code: src/game.cpp](src/game.cpp)
- [Fixed code: src/game_fixed.cpp](src/game_fixed.cpp)

## 4. How to Run Static Analysis

```
cppcheck --enable=all --inconclusive --std=c++17 src/game.cpp
g++ -Wall -Wextra -O2 -c src/game.cpp
g++ -Wall -Wextra -fsanitize=address,undefined -g src/game.cpp -o game
```

The first two commands are pure static analysis, nothing is executed. The third one builds the program with sanitizers so the defects can be confirmed at run time (the original program crashes on it).

To check the fixed version:

```
cppcheck --enable=all --inconclusive --std=c++17 src/game_fixed.cpp
g++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined -g src/game_fixed.cpp -o game_fixed
./game_fixed
```

## 5. Results

| Defect | Line | g++ | cppcheck | Manual |
|--------|------|-----|----------|--------|
| `=` instead of `==` | 47 | Found | Not found | Found |
| Unsigned comparison always false | 56 | Found | Found | Found |
| Reference to local variable returned | 73 | Found | Found | Found |
| Missing return in `playerRank` | 99 | Found | Found | Found |
| `new[]` released with `delete` | 109 | Found | Found | Found |
| Uninitialized `sum` | 77-80 | Found | Found | Found |
| Use after delete | 86 | Not found | Found | Found |
| No copy constructor / `operator=` | 133 | Not found | Found | Found |
| Array overflow in `spawnMonster` | 41 | Not found | Not found | Found (ASan also confirms) |
| Loop `<=` out of bounds | 64 | Not found | Not found | Found |
| Null pointer dereference | 54, 71, 84 | Not found | Not found | Found |
| Buffer overflow with `cin >> name` | 105 | Not found | Not found | Found |
| Memory leak of `bonus` | 121 | Not found | Not found | Found |
| Dangling pointer / double delete | 85 | Not found | Not found | Found |

## 6. Conclusion

The original `game.cpp` had defects in all 8 checklist categories (21 defects in total: 11 High, 7 Medium, 3 Low). The most severe were null pointer dereferences, out of bounds access, buffer overflow, use after delete, double delete and uninitialized variables.

The fixed version (`src/game_fixed.cpp`) addresses all of them. It compiles without warnings under `-Wall -Wextra`, runs clean with AddressSanitizer and UndefinedBehaviorSanitizer (including with a very long boss name and invalid health input), and cppcheck only reports one inconclusive performance hint about a local copy.

Static tools alone were not enough: many serious defects were only found by the manual review, so the best approach is to use both.
