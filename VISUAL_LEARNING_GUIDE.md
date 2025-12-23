# Philosophers Project - Visual Learning Guide

**For Staff Evaluation Preparation**

---

## SECTION 1: VISUAL REPRESENTATION OF THE PROBLEM

### The Setup (5 Philosophers)

```
            Philosopher 1
                 (id=1)
                  
     Fork 0        Fork 1
        \            /
         \          /
          \        /
    P 5 ─ ─ ─ ─ ─ ─ ─ ─ ─ P 2
   (id=5)                 (id=2)
   
    Fork 4        Fork 2
        \            /
         \          /
          \        /
    P 4 ─ ─ ─ ─ ─ ─ P 3
   (id=4)          (id=3)
   
   Fork 3
```

**Key insight:** Each philosopher needs TWO adjacent forks to eat.

### Without Deadlock Prevention

```
T=0: All philosophers hungry

P1: Takes fork 0
P2: Takes fork 1  
P3: Takes fork 2
P4: Takes fork 3
P5: Takes fork 4

Now each philosopher has ONE fork and waits for the next...

P1 waits for fork 1 (P2 has it)
P2 waits for fork 2 (P3 has it)
P3 waits for fork 3 (P4 has it)
P4 waits for fork 4 (P5 has it)
P5 waits for fork 0 (P1 has it)

DEADLOCK! 
Everyone has one fork, no one can proceed.
```

### With Your Lock Ordering

```
Lock Ordering Rule: Always acquire forks in ascending order

P1: l=0, r=1 → Always: lock 0, then lock 1
P2: l=1, r=2 → Always: lock 1, then lock 2
P3: l=2, r=3 → Always: lock 2, then lock 3
P4: l=3, r=4 → Always: lock 3, then lock 4
P5: l=4, r=0 → Always: lock 0, then lock 4

ALL threads follow: 0 ≤ 1 ≤ 2 ≤ 3 ≤ 4

No thread ever acquires backward!
No cycles possible!
NO DEADLOCK POSSIBLE!
```

---

## SECTION 2: SYNCHRONIZATION STRUCTURE

### What Each Mutex Protects

```
PHILOSOPHY'S SHARED DATA:

┌─────────────────────────────────────┐
│        Global Data (t_data)         │
├─────────────────────────────────────┤
│ stop_sim ──────────→ [stop_mutex]   │
│ last_meal ─────────→ [state_mutex]  │
│ printf output ─────→ [print_mutex]  │
│ forks[0..n] ──────→ [fork_mutex]    │
│ forks_st[0..n] ────→ [fork_mutex]   │
└─────────────────────────────────────┘

Each arrow: Protected by that mutex
Only ONE thread can access at a time
Prevents corruption!
```

### Mutex Lifeline

```
Program Start:
  ├─ pthread_mutex_init(&print)
  ├─ pthread_mutex_init(&state)
  ├─ pthread_mutex_init(&stop_mutex)
  └─ pthread_mutex_init(&forks[0..n])
  
Program Running:
  ├─ Philosophers lock/unlock (eating)
  ├─ Monitor reads (checking state)
  └─ Both coordinate via mutexes
  
Program End:
  ├─ pthread_mutex_destroy(&print)
  ├─ pthread_mutex_destroy(&state)
  ├─ pthread_mutex_destroy(&stop_mutex)
  └─ pthread_mutex_destroy(&forks[0..n])
```

---

## SECTION 3: THREAD LIFECYCLE

### Main Thread

```
┌─────────────────────────────────────────┐
│ main()                                  │
├─────────────────────────────────────────┤
│ 1. Parse arguments                      │
│ 2. Initialize data structures           │
│ 3. Call start_routine()                 │
│    ├─ Create N philosopher threads      │
│    ├─ Create 1 monitor thread           │
│    └─ Wait for monitor to finish        │
│ 4. Cleanup                              │
│ 5. Exit                                 │
└─────────────────────────────────────────┘
```

### Philosopher Thread (Each of N)

```
┌─────────────────────────────────────────┐
│ philo_routine(philosopher_id)           │
├─────────────────────────────────────────┤
│ LOOP (while !get_stop()):               │
│   ├─ Try to take both forks             │
│   │   ├─ Lock fork1 (smaller #)         │
│   │   ├─ Lock fork2 (larger #)          │
│   │   ├─ Check fairness (already own?)  │
│   │   └─ If ok: eat                     │
│   │       ├─ Update last_meal           │
│   │       ├─ Eat (sleep for X ms)       │
│   │       ├─ Increment meal count       │
│   │       └─ Release both forks         │
│   │                                     │
│   ├─ If failed to eat:                  │
│   │   └─ Sleep 200µs and retry          │
│   │                                     │
│   ├─ Sleep (if eating succeeded)        │
│   └─ Think                              │
│                                         │
│ END LOOP (when monitor stops)           │
│ Exit thread                             │
└─────────────────────────────────────────┘
```

### Monitor Thread

```
┌─────────────────────────────────────────┐
│ monitor_routine()                       │
├─────────────────────────────────────────┤
│ LOOP (while !get_stop()):               │
│   ├─ For each philosopher:              │
│   │   ├─ Check: now - last_meal         │
│   │   │         > time_to_die?          │
│   │   │                                 │
│   │   └─ If YES:                        │
│   │       ├─ Print death message        │
│   │       ├─ Set stop_sim = 1           │
│   │       └─ Exit loop                  │
│   │                                     │
│   ├─ Check: Everyone ate enough meals? │
│   │   └─ If YES:                        │
│   │       ├─ Set stop_sim = 1           │
│   │       └─ Exit loop                  │
│   │                                     │
│   └─ Sleep 1ms (check again)            │
│                                         │
│ END LOOP                                │
│ Exit thread                             │
│ (Main thread was waiting for this)      │
└─────────────────────────────────────────┘
```

---

## SECTION 4: LOCK ACQUISITION FLOWCHART

```
PHILOSOPHER WANTS TO EAT:

        START
          │
          ├─→ get_lock_order(l, r, &f1, &f2)
          │   (Ensure f1 < f2)
          │
          ├─→ lock(forks[f1])  ← First lock (smaller)
          │
          ├─→ lock(forks[f2])  ← Second lock (larger)
          │
          ├─→ Check: Do I already own one fork?
          │   │
          │   ├─ YES: Release both → RETURN FAIL (retry later)
          │   │
          │   └─ NO: Continue
          │
          ├─→ Mark forks as mine (forks_st[l] = my_id)
          │
          ├─→ EAT!
          │
          ├─→ Mark forks as mine (forks_st[r] = my_id)
          │
          ├─→ unlock(forks[f2])  ← Release (reverse order)
          │
          ├─→ unlock(forks[f1])  ← Release (reverse order)
          │
          └─→ SLEEP, THINK
          
KEY: Always acquire in ascending order (f1 < f2)
     Always release in reverse order
```

---

## SECTION 5: EXECUTION TIMELINE (2 Philosophers)

```
T=0ms  ┌─ Both start
       │  Phil 1: THINKING
       │  Phil 2: THINKING (with 200µs delay)
       │  All forks: AVAILABLE
       │
T=10ms ├─ Phil 1 tries eat
       │  ├─ Locks fork 0
       │  ├─ Locks fork 1
       │  └─ EATING (until T=20)
       │  
       │  Phil 2: THINKING
       │  Fork 0: LOCKED (Phil 1)
       │  Fork 1: LOCKED (Phil 1)
       │
T=15ms ├─ Phil 2 tries eat
       │  ├─ Tries to lock fork 1 (Phil 1 has it)
       │  └─ BLOCKED waiting for fork 1
       │
T=20ms ├─ Phil 1 finishes eating
       │  ├─ Unlocks fork 1
       │  ├─ Unlocks fork 0
       │  └─ SLEEPING (until T=30)
       │
       │  Phil 2 unblocks!
       │  ├─ Locks fork 1 (now available)
       │  ├─ Locks fork 0
       │  └─ EATING (until T=30)
       │
T=25ms ├─ Phil 1: SLEEPING
       │  Phil 2: EATING
       │  Fork 0: LOCKED (Phil 2)
       │  Fork 1: LOCKED (Phil 2)
       │
T=30ms ├─ Phil 1 done sleeping
       │  └─ THINKING
       │
       │  Phil 2 done eating
       │  └─ SLEEPING (until T=40)
       │
T=40ms ├─ Phil 1 tries eat again → EATING
       │  Phil 2 done sleeping → THINKING
       │
       ... pattern continues ...
```

---

## SECTION 6: MUTEX TIMELINE (Zoomed)

```
Time  Fork 0 Mutex      Fork 1 Mutex        Event
────────────────────────────────────────────────────
0ms   UNLOCKED          UNLOCKED            Start
5ms   LOCK              -                   P1 locks f0
      LOCKED            UNLOCKED
10ms  LOCKED            LOCKED              P1 locks f1
                                            P1 eating
15ms  LOCKED            LOCKED              P2 blocked on f1
                        (WAITING...)
20ms  UNLOCKED          UNLOCKED            P1 releases
      (P2 proceeds!)
      
      LOCKED            LOCKED              P2 locks f1, then f0
                                            P2 eating
25ms  LOCKED            LOCKED              P2 eating
30ms  UNLOCKED          UNLOCKED            P2 releases
      (P1 proceeds if waiting)
```

---

## SECTION 7: DEADLOCK DETECTION (What NOT to Do)

### Scenario: Random Lock Order

```
P1: l=0, r=1
P2: l=1, r=2

DIFFERENT THREADS, DIFFERENT ORDER:

P1 (T=0):     lock(0) ✓
P2 (T=1):     lock(1) ✓
P1 (T=2):     lock(1) ✗ BLOCKED (P2 has it)
P2 (T=3):     lock(2) ✓
P2 (T=4):     unlock(2)
P2 (T=5):     unlock(1)
P1 (T=6):     lock(1) ✓ (now available)
              lock(2) ...

Some philosophers might:
- P1: locks 0, waits for 1
- P2: locks 1, waits for 2
- P3: locks 2, waits for 0 (if exists)

This creates cycle: 0→1→2→0

DEADLOCK POSSIBLE!
```

### YOUR Approach: Strict Ordering

```
SAME ORDER for ALL THREADS:

P1: Always 0 → 1
P2: Always 1 → 2
P3: Always 2 → 3 (if exists, otherwise just 2)

Dependency graph:
  0 ← 1 ← 2 ← 3

Always ascending! No cycles!
DEADLOCK IMPOSSIBLE!
```

---

## SECTION 8: MAKEFILE BUILD GRAPH

```
make command
     │
     ├─ Check target: all
     │  └─ Depends on: philo
     │
     ├─ Check target: philo
     │  └─ Depends on: main.o, parse.o, init.o, ...
     │
     ├─ For each .o:
     │  Check: .c timestamp vs .o timestamp
     │  
     ├─ If .c newer:
     │  └─ Compile: cc -c .c → .o
     │
     ├─ All .o files ready
     │  └─ Link: cc -o philo *.o
     │
     └─ Done! philo executable ready
```

### Build System: Before and After

```
WITHOUT Makefile:
$ cc -Wall -Wextra -Werror -pthread -c main.c -o main.o
$ cc -Wall -Wextra -Werror -pthread -c parse.c -o parse.o
$ cc -Wall -Wextra -Werror -pthread -c init.c -o init.o
$ cc -Wall -Wextra -Werror -pthread -c monitor.c -o monitor.o
$ cc -Wall -Wextra -Werror -pthread -c start.c -o start.o
$ cc -Wall -Wextra -Werror -pthread -c routine.c -o routine.o
$ cc -Wall -Wextra -Werror -pthread -c time_utils.c -o time_utils.o
$ cc -Wall -Wextra -Werror -pthread -c free_error.c -o free_error.o
$ cc -Wall -Wextra -Werror -pthread -o philo *.o

Tedious! Error-prone!

WITH Makefile:
$ make

Much better!
And if you edit one file:
$ make
(only that file recompiled)
```

---

## SECTION 9: MEMORY LAYOUT

```
STACK (each thread):
┌─────────────────────┐
│ Local variables     │
│ (each thread copy)  │
└─────────────────────┘

HEAP (shared):
┌─────────────────────────────────┐
│ t_data structure (global)       │
│  ├─ philo[] array               │
│  ├─ forks[] mutex array         │
│  ├─ forks_st[] ownership array  │
│  └─ Various mutexes             │
├─────────────────────────────────┤
│ Other allocations               │
│ (if needed)                     │
└─────────────────────────────────┘

CODE SECTION:
┌─────────────────────────────────┐
│ main(), philo_routine(),        │
│ monitor_routine(), etc.         │
└─────────────────────────────────┘

All threads share HEAP and CODE
Each has own STACK
```

---

## SECTION 10: KEY DECISION MATRIX

```
DESIGN CHOICE          WHY NOT ALTERNATIVE?              YOUR CHOICE
────────────────────────────────────────────────────────────────────
Lock Order      Random would cause deadlock       Numerical order (safe)
                                                  
Fairness        None → some starve               Force release & retry
                                                  
Mutex Style     One global → no parallelism      Per-resource (parallel)
                                                  
Timing          sleep() → too coarse            usleep() (precise)
                                                  
Monitoring      Each philosopher checks         Separate thread
                others → complex & slow          
                                                  
Printf          Unprotected → scrambled output  Protected by mutex
                                                  
Stop Flag       Direct access → race condition  Protected by mutex
```

---

## SECTION 11: COMPLEXITY ANALYSIS

### Time Complexity

```
Eating cycles: O(n) per philosopher
Total eating: O(n²) (each does ~n cycles)
Monitor checks: O(n) every 1ms

Bottleneck: Monitor checking (but negligible)
Result: Linear scaling possible
```

### Space Complexity

```
Philosophers: O(n)
Forks: O(n)
Mutexes: O(n) (one per fork + global)
Total: O(n)

Example for 500 philosophers:
≈ 500 * (struct + thread) + 500 mutexes
≈ 50-100KB probably
```

---

## SECTION 12: TESTING MENTAL MODEL

### Run This Example in Your Head

```
3 philosophers
time_to_die = 100ms
time_to_eat = 30ms
time_to_sleep = 20ms

T=0ms:  All think, all forks free
T=10ms: P1 eats (locked 0,1 until T=40)
T=15ms: P2 blocked (waiting for fork 1)
T=20ms: P3 thinks
T=40ms: P1 releases, P2 now can grab 1,2
        P1 sleeps until T=60
T=50ms: P2 releases, P1 thinks, P3 can eat
T=60ms: P1 can eat again, P2 sleeps
...
```

**Question:** Does anyone die? Everyone eats enough?

**Answer:** Monitor checks timing constantly
           As long as eating/sleeping cycles complete
           Before time_to_die expires, everyone survives!

---

## FINAL VISUAL: The Concept

```
PROBLEM:                    SOLUTION:
┌─────────┐                ┌──────────┐
│ Deadlock├──────→ ┌─────→ │ Ordering │
└─────────┘        │       └──────────┘
┌─────────┐        │       ┌──────────┐
│Starvation├─────→ ┤─────→ │ Fairness │
└─────────┘        │       └──────────┘
┌─────────┐        │       ┌──────────┐
│ No Speed├────────┤─────→ │Per-locks │
└─────────┘        │       └──────────┘
┌─────────┐        │       ┌──────────┐
│ Messy   ├────────┤─────→ │ Monitor  │
└─────────┘        │       └──────────┘
                   │       ┌──────────┐
                   └─────→ │ Mutexes  │
                           └──────────┘

Each problem solved by specific mechanism!
Combined: Complete, correct solution!
```

---

**Use these visuals to study and explain your project!**

