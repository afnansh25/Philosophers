# 🧪 MAKEFILE TEST SCENARIOS
## Test Your Understanding!

---

## SCENARIO 1: Pattern Rule Missing
```makefile
NAME = philo
SRC = main.c parse.c
OBJ = $(SRC:.c=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
    $(CC) $(FLAGS) -o $(NAME) $(OBJ)

# Note: No %.o: %.c rule!
```

**Question:** What happens when you run `make`?

**Your Answer:**
_________________________

**Correct Answer:** 
<details>
<summary>Click to reveal</summary>

Make will try to find `main.o` and `parse.o`, but since there's no rule to create them from `.c` files, it will either:
- Use a built-in implicit rule (if available)
- OR give an error: "No rule to make target 'main.o'"

**Result: Likely fails** (depending on Make's built-in rules)
</details>

---

## SCENARIO 2: Wrong Target Order
```makefile
NAME = philo
OBJ = main.o parse.o

fclean:
    rm -rf $(OBJ) $(NAME)

all: $(NAME)

$(NAME): $(OBJ)
    cc -o $(NAME) $(OBJ)

%.o: %.c
    cc -c $< -o $@
```

**Question:** What happens when you type just `make`?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

It runs `fclean` (the first target), which DELETES all compiled files!

**Result: Everything gets deleted** ❌

**Fix:** Move `all:` to the top.
</details>

---

## SCENARIO 3: Missing Dependency
```makefile
NAME = philo
SRC = main.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
    cc -o $(NAME) $(OBJ)

%.o: %.c
    cc -c $< -o $@
```

**Timeline:**
1. Run `make` → compiles successfully, creates `philo`
2. Edit `utils.h` (change a function signature)
3. Run `make` again

**Question:** Will it recompile?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**NO!** Make says "philo is up to date" because:
- `main.c` timestamp unchanged
- `utils.c` timestamp unchanged
- `.o` files still newer than `.c` files

**Result: Bug! Old code with new header** ❌

**Fix:** Add header dependency:
```makefile
%.o: %.c utils.h
    cc -c $< -o $@
```
</details>

---

## SCENARIO 4: Variable Expansion
```makefile
NAME = my_program
CC = gcc
FLAGS = -Wall

all: $(NAME)

$(NAME):
    $(CC) $(FLAGS) -o $(NAME) main.c
```

**Question:** This rule doesn't list any dependencies. What's wrong? Will it work?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**It compiles BUT has a serious problem:**
1. **Never recompiles after first build** - No dependencies means Make can't detect source changes
2. If you edit `main.c`, running `make` will say "Nothing to be done"
3. Your executable contains OLD code, but Make thinks it's "up to date"
4. This creates **silent bugs** - code and executable don't match!

**Result: Dangerous! Can cause mysterious bugs** ❌

**How Make decides to rebuild:**
- Compares target timestamp vs dependency timestamps
- No dependencies = No comparison = Assumes up to date (if target exists)

**Fix:** Add dependency:
```makefile
$(NAME): main.c
    $(CC) $(FLAGS) -o $(NAME) main.c
```
Now Make checks: Is `main.c` newer than `my_program`? If yes → rebuild!
</details>

---

## SCENARIO 5: TAB vs Spaces
```makefile
NAME = philo
OBJ = main.o

all: $(NAME)

$(NAME): $(OBJ)
    cc -o $(NAME) $(OBJ)
```

**Question:** What if the line "cc -o $(NAME) $(OBJ)" uses 4 SPACES instead of a TAB?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**ERROR!** Make will fail with:
```
Makefile:7: *** missing separator. Stop.
```

**Result: Won't compile at all** ❌

**Why:** Make REQUIRES a TAB character for recipes, not spaces.

**Fix:** Use actual TAB character (press Tab key, not spaces).
</details>

---

## SCENARIO 6: Multiple Header Dependencies
```makefile
%.o: %.c philo.h utils.h constants.h
    $(CC) $(FLAGS) -c $< -o $@
```

**Question:** What happens if you change ONLY `constants.h`?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**All .o files recompile!** Because:
- Every `.o` depends on all three headers
- When any header changes, Make rebuilds all objects

**Result: Works correctly** ✅

**Downside:** Maybe inefficient if not all files include all headers.

**Better solution:** Use automatic dependency generation with `-MMD -MP` flags.
</details>

---

## SCENARIO 7: Circular Dependency
```makefile
all: clean $(NAME)

$(NAME): $(OBJ)
    cc -o $(NAME) $(OBJ)

clean: all
    rm -rf $(OBJ)

%.o: %.c
    cc -c $< -o $@
```

**Question:** What happens when you run `make`?

**Your Answer:**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**Circular dependency error!**
```
Makefile:3: Circular clean <- all dependency dropped.
```

**Result: Unpredictable behavior** ❌

**Why:** `all` depends on `clean`, `clean` depends on `all` → infinite loop

**Fix:** Remove circular dependency. `clean` should never depend on `all`.
</details>

---

## SCENARIO 8: Testing .PHONY
**Setup:** Create a real file named `clean`:
```bash
touch clean
echo "dummy file" > clean
```

**Makefile:**
```makefile
# Version A: Without .PHONY
clean:
    rm -rf *.o

# Version B: With .PHONY
.PHONY: clean
clean:
    rm -rf *.o
```

**Question:** What's the difference when you run `make clean`?

**Your Answer (Version A):**
_________________________

**Your Answer (Version B):**
_________________________

**Correct Answer:**
<details>
<summary>Click to reveal</summary>

**Version A (without .PHONY):**
```
make: 'clean' is up to date.
```
Make sees the file `clean` exists and has no prerequisites, so it's "up to date". **Won't run the command!** ❌

**Version B (with .PHONY):**
```
rm -rf *.o
```
Make knows `clean` is not a real file, so it **always runs the command** ✅

**Lesson:** Always use `.PHONY` for targets that aren't files!
</details>

---

## SCENARIO 9: Adding Header Dependency
**Current Makefile:**
```makefile
NAME = philo
SRC = main.c parse.c init.c
OBJ = $(SRC:.c=.o)
CC = cc
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
    $(CC) $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.c
    $(CC) $(FLAGS) -c $< -o $@
```

**Question:** Write THREE different ways to add `philo.h` as a dependency so changing the header triggers recompilation.

**Your Answer 1:**
```makefile
_________________________
```

**Your Answer 2:**
```makefile
_________________________
```

**Your Answer 3:**
```makefile
_________________________
```

**Correct Answers:**
<details>
<summary>Click to reveal</summary>

**Method 1: Pattern Rule**
```makefile
%.o: %.c philo.h
    $(CC) $(FLAGS) -c $< -o $@
```

**Method 2: Variable + Dependency**
```makefile
DEPS = philo.h

%.o: %.c $(DEPS)
    $(CC) $(FLAGS) -c $< -o $@
```

**Method 3: Direct Dependency**
```makefile
$(OBJ): philo.h

%.o: %.c
    $(CC) $(FLAGS) -c $< -o $@
```

**All three work!** ✅
</details>

---

## SCENARIO 10: Writing a New Rule

**Challenge:** Add a `run` rule that:
1. Builds the program (if needed)
2. Runs it with arguments: `5 800 200 200`

**Your Answer:**
```makefile
_________________________
```

**Question:** Should `run` be in `.PHONY`? Why?

**Your Answer:**
_________________________

**Correct Answers:**
<details>
<summary>Click to reveal</summary>

**The Rule:**
```makefile
run: $(NAME)
    ./$(NAME) 5 800 200 200

.PHONY: all clean fclean re run
```

**Why .PHONY?**
YES! Because:
- `run` is an action, not a file
- If a file named `run` existed, Make would think the target is "up to date"
- We always want `make run` to execute the program

**Result:** ✅
```bash
make run
# Builds if needed, then runs: ./philo 5 800 200 200
```
</details>

---

## 🎯 BONUS CHALLENGE: Fix This Broken Makefile

```makefile
PROGRAM = test
FILES = one.c two.c three.c
OBJECTS = one.o two.o three.o

$(PROGRAM): $(OBJECTS)
    gcc -o PROGRAM OBJECTS

%.o: %.c
    gcc -c $< -o %.o

all: $(PROGRAM)

clean:
rm -rf $(OBJECTS)
```

**Find and list ALL the bugs:**

1. _________________________
2. _________________________
3. _________________________
4. _________________________

**Correct Answers:**
<details>
<summary>Click to reveal</summary>

**Bug 1:** Line 6: `gcc -o PROGRAM OBJECTS`
- Should be: `gcc -o $(PROGRAM) $(OBJECTS)`
- Missing `$()` for variable expansion

**Bug 2:** Line 9: `-o %.o`
- Should be: `-o $@`
- `%.o` is pattern, not actual filename. Use `$@` for target

**Bug 3:** Line 11: `all:` should be FIRST target
- Should be before `$(PROGRAM):`

**Bug 4:** Line 14: Missing TAB before `rm`
- Must use TAB character, not spaces

**Bug 5:** (Bonus) No `.PHONY:` declaration for `all` and `clean`

**Fixed version:**
```makefile
PROGRAM = test
FILES = one.c two.c three.c
OBJECTS = one.o two.o three.o

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
    gcc -o $(PROGRAM) $(OBJECTS)

%.o: %.c
    gcc -c $< -o $@

clean:
    rm -rf $(OBJECTS)

.PHONY: all clean
```
</details>

---

## 📊 SCORE YOURSELF

- **0-3 correct:** Review the main guide again
- **4-6 correct:** You're getting there! Practice more
- **7-8 correct:** Good understanding! Almost ready
- **9-10 correct:** Excellent! You're evaluation-ready! 🏆

---

## 💡 KEY TAKEAWAYS

1. **Substitution creates lists**, **pattern rules create recipes**
2. **First target is default** - always put `all:` first
3. **TABs are mandatory** for recipes (not spaces!)
4. **.PHONY protects** action targets from file conflicts
5. **Header dependencies** prevent subtle bugs
6. **Dependencies matter** - they control when Make rebuilds
7. **Order matters** in target definitions
8. **Variable expansion** needs `$()`

Good luck with your evaluation! 🚀
