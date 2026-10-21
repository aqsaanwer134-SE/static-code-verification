# Review Checklist - `game.cpp`

| Category | Result | Defect |
|----------|--------|--------|
| Initialization | Issue | `sum` is used without an initial value (L77). `health` stays uninitialized if input fails (L103). `monsters[]` pointers are not initialized (L27). |
| Pointers (null / delete) | Issue | `findMonster` result used without null check (L54-56, L71-72, L84-86). Use after delete (L85-86). Dangling pointer left in array and deleted again in destructor (L85, L37). `delete` used for array (L109). Memory leak of `bonus` (L121). |
| Array / Buffer | Issue | No bounds check in `spawnMonster`, 6th monster overflows the array (L41, L119). `cin >> name` can overflow `char[10]` (L102, L105). |
| Loops | Issue | Loop uses `<=` and reads one element past the last monster (L64). |
| Arithmetic | Issue | Signed/unsigned mix makes the health check always false (L55-56). Division by zero when no monsters (L80). Integer division loses decimals (L91). `double` returned as `int` (L80). |
| Control flow | Issue | `=` used instead of `==` in `if` (L47). `playerRank` has no return for score <= 100 (L94-99). |
| Function call | Issue | `attackMonster(99, 20)` called with an id that does not exist (L123). Function returns reference to a local variable (L70-73). `spawnMonster` result/limit not checked in `readBoss` (L108). |
| Class | Issue | No copy constructor or assignment operator, so `Game copy = game;` makes a shallow copy and causes double delete (L133, L35-38). Constructor could use initializer list and const reference (L17-22). |
