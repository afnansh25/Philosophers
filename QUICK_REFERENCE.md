# Philosophers Project - Quick Reference Card

**Print this out and study before your evaluation!**

---

## THE PROBLEM IN ONE SENTENCE

**N philosophers need 2 forks each to eat, only N forks exist → potential for deadlock and starvation**

---

## YOUR SOLUTION IN FOUR POINTS

| Solution | Mechanism | Prevents |
|----------|-----------|----------|
| **Lock Ordering** | Always lock forks in numerical order | Deadlock (breaks circular wait) |
| **Fairness Check** | Can't hold one fork and grab another | Starvation (forces rotation) |
| **Separate Locks** | Each fork gets its own mutex | Bottleneck (allows parallelism) |
| **Monitor Thread** | Separate thread watches for death/completion | Infinite loops (ensures termination) |

---

## DEADLOCK = 4 CONDITIONS (ALL must be true)

❌ Break ANY one and NO deadlock:

1. **Mutual Exclusion** - Can't break (need exclusive access)
2. **Hold & Wait** - Can't break (lock-based resources)
3. **No Preemption** - Can't break (mutex behavior)
4. **Circular Wait** ← **YOU BREAK THIS** with lock ordering

**Your approach:** Enforce total lock order → No cycles → No deadlock ✓

---

## YOUR HEADERS (What to Know)

### `philo.h` - Key Structures

```c
s_philo {
    pthread_t thrd;      // Thread running this philosopher
    int id;              // Philosopher number (1-N)
    int meals_count;     // How many meals eaten
    long last_meal;      // Last eating timestamp
    int l_fork, r_fork;  // Fork indices
    t_data *phdata;      // Pointer to global data
}

t_data {
    int num_philo, time_to_die, time_to_eat, time_to_sleep, eat_limit;
    
    pthread_mutex_t forks[N];     // One lock per fork
    pthread_mutex_t print;        // Output sync
    pthread_mutex_t state;        // State data sync
    pthread_mutex_t stop_mutex;   // Stop flag sync
    
    char *forks_st;   // Who owns each fork
    int stop_sim;     // Stop flag
    long start_time;  // Simulation start
}
```

---

## MAKEFILE QUICK REFERENCE

```makefile
NAME = philo              # Target executable
OBJ = $(SRC:.c=.o)       # Substitute .c → .o
%.o: %.c                 # Compile .c files to .o
$(NAME): $(OBJ)          # Link all objects
```

**Commands:**
```bash
make          # Build
make clean    # Remove .o files  
make fclean   # Remove .o AND executable
make re       # Rebuild everything
```

**Critical flag:** `-pthread` ← Links thread library!

---

## FLOW AT A GLANCE

```
PHILOSOPHER (Each thread):
1. Try take two forks (lock ordering!)
2. If success: eat → update last_meal → release forks
3. If fail: wait a bit, retry
4. Sleep
5. Think
6. Loop until monitor stops

MONITOR (Single thread):
1. Every 1ms check each philosopher
2. If anyone: time_since_meal > time_to_die → they're dead!
3. If everyone: meals_count >= eat_limit → done!
4. Set stop flag when done
```

---

## THE 3-PHILOSOPHER DEADLOCK (Without Lock Order)

```
Thread A: Lock 0, waiting for 1
Thread B: Lock 1, waiting for 2
Thread C: Lock 2, waiting for 0

CYCLE: A→0→1→B→1→2→C→2→0→A
= DEADLOCK! (everyone blocked)
```

**Your solution:** Always acquire 0→1→2→3...
No backward lock = No cycle = No deadlock!

---

## COMMON ANSWERS FOR INTERVIEWS

| Q | A |
|---|---|
| "How prevent deadlock?" | "Lock ordering - always acquire in numerical order" |
| "How prevent starvation?" | "Fairness check - can't hold one fork and grab another" |
| "Why separate locks?" | "Multiple threads can proceed concurrently" |
| "Why monitor thread?" | "Separate concern, reliable detection" |
| "What about printf()?" | "Protected by print mutex - only one thread at a time" |
| "Edge case: 1 philosopher?" | "Special case - lock fork, wait to die, release" |
| "The -pthread flag?" | "Links POSIX threads library - required for pthread functions" |
| "Why usleep() not sleep()?" | "Need millisecond precision, sleep() only does seconds" |

---

## MUTEX PROTECTION CHECKLIST

✓ **stop_sim** → stop_mutex  
✓ **last_meal** → state mutex  
✓ **forks_st** → fork mutex (inside locks)  
✓ **printf** → print mutex  
✓ **Each fork** → its own mutex

---

## RED FLAGS (What Could Go Wrong)

| Issue | Your Prevention |
|-------|-----------------|
| Deadlock | Lock ordering (always f1 < f2) |
| Starvation | Fairness check (release if already own) |
| Race on stop_sim | stop_mutex (lock before read/write) |
| Corrupted output | print mutex (lock around printf) |
| Stale last_meal reads | state mutex (lock around access) |
| Single thread bottleneck | Per-fork locks (not global) |

---

## TIMESTAMP FORMULA

```
DEAD if: (now_ms() - get_last_meal(philo)) > time_to_die

Example:
- now = 5000ms
- last_meal = 4900ms
- time_to_die = 50ms
- difference = 100ms
- 100 > 50 → DEAD!
```

---

## THREAD CREATION & CLEANUP

```c
// CREATE:
pthread_create(&thread, NULL, function, arg);

// DO WORK...

// CLEANUP:
pthread_join(thread, NULL);       // Wait for thread
pthread_mutex_destroy(&mutex);    // Cleanup mutex
free(ptr);                        // Free memory
```

**Order matters:** Join threads BEFORE destroying mutexes!

---

## WHAT EACH FILE DOES

| File | Purpose |
|------|---------|
| main.c | Parse args, call init, start, cleanup |
| parse.c | Convert string args to numbers |
| init.c | Allocate memory, init mutexes, setup philosophers |
| routine.c | Philosopher's eat/sleep/think logic |
| monitor.c | Check death/completion, set stop flag |
| start.c | Create threads, launch simulation |
| time_utils.c | Time functions: now, sleep, get_stop |
| free_error.c | Cleanup and deallocation |
| philo.h | Header: struct definitions, prototypes |
| Makefile | Build configuration |

---

## THE FAIRNESS CHECK (WHY IT MATTERS)

```c
if (d->forks_st[p->l_fork] == (char)p->id
    || d->forks_st[p->r_fork] == (char)p->id)
{
    // Already own one fork → RELEASE and RETRY
    pthread_mutex_unlock(&d->forks[f2]);
    pthread_mutex_unlock(&d->forks[f1]);
    return (0);
}
```

**Why?** Prevents monopoly. Forces turn-taking. Everyone gets chances.

---

## LOCK ORDERING FUNCTION

```c
static void get_lock_order(int l, int r, int *f1, int *f2)
{
    if (l < r)
    {
        *f1 = l;  // Smaller index first
        *f2 = r;
    }
    else
    {
        *f1 = r;  // Smaller index first
        *f2 = l;
    }
}
```

**Guarantee:** Always f1 < f2 → No cycles possible!

---

## MONITOR LOGIC

```
LOOP:
  FOR each philosopher:
    if (now - last_meal > time_to_die):
      Print death
      Set stop flag
      Break
  
  IF not dead AND everyone ate limit:
    Set stop flag
  
  Sleep 1ms
```

---

## USAGE EXAMPLE

```bash
./philo 4 410 200 200 10
        │  │   │   │   │
        │  │   │   │   └─ Eat limit (optional)
        │  │   │   └─ Time to sleep (ms)
        │  │   └─ Time to eat (ms)
        │  └─ Time to die (ms)
        └─ Number of philosophers

Output:
0 1 has taken a fork
1 1 has taken a fork
2 1 is eating
...
```

---

## MEMORY LAYOUT (3 Philosophers)

```
t_data:
  - num_philo = 3
  - philo[3] array
    - philo[0]: id=1, l=0, r=1
    - philo[1]: id=2, l=1, r=2
    - philo[2]: id=3, l=2, r=0
  - forks[3] mutex array
  - forks_st[3] ownership array
  - Various mutexes (print, state, stop)
  - Threads array (3 + 1 monitor)
```

---

## WHY YOUR DESIGN WORKS

1. **Lock Ordering** → Breaks circular wait → No deadlock
2. **Fairness** → Forces release → No starvation  
3. **Per-lock** → Multiple can proceed → Concurrency
4. **Monitor** → Detects termination → Clean shutdown
5. **Mutexes** → Protects shared state → No races

**Result:** Correct, fair, concurrent solution! ✓

---

## FINAL TEST QUESTIONS (Ask Yourself)

- [ ] Why does numerical lock order prevent deadlock?
- [ ] What 4 conditions cause deadlock? Which do you break?
- [ ] What does the fairness check do? Why?
- [ ] What if you used one global mutex instead of per-fork?
- [ ] How does the monitor detect death?
- [ ] What does the -pthread flag do?
- [ ] Why separate monitor thread instead of philosophers monitoring?
- [ ] What happens to a philosopher with 1 fork?
- [ ] How is stop_sim made thread-safe?
- [ ] What would happen if you used sleep() instead of usleep()?

**If you can answer all of these → You're ready! 🎓**

---

**Save this card and review it daily until your evaluation!**

