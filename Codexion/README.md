*This project has been created as part of the 42 curriculum by vabad-ro.*

## Description

**Codexion** is a multithreaded simulation of coders competing for a limited set of shared resources (dongles) in order to compile their code. 

- **Coders as threads:** Each coder is represented by its own `pthread` thread and follows a fixed cycle: `compile` -> `debug` -> `refactor` -> try to compile again.
- **Dongles:** There are `n` dongles for `n` coders. If `n == 1`, there is a single dongle. To perform a compile, a coder must acquire **two** dongles (its left and right dongle).
- **Cooldown:** After a dongle is released, it cannot be taken again until `dongle_cooldown` milliseconds have passed.
- **Scheduling:** The arbitration of requests to each dongle is done by a scheduler passed as argument: `fifo` or `edf`. 
  - `fifo`: The request that arrived first is served first.
  - `edf`: The request with the closest burnout deadline is served first. The deadline is calculated as `last_compile_start + time_to_burnout`. When using `edf`, no coder should burn out if the parameters are viable.
- **Burnout detection:** A dedicated monitor thread continuously checks for burnout with high precision. If a coder has not reached the required number of compiles and `gettimeofday()`-based time exceeds their deadline, the simulation stops and a `burned out` log is emitted.
- **Termination:** The simulation ends when **any** coder burns out, or when **all** coders have completed at least `number_of_compiles_required` compiles.

### Program arguments

```text
number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory and validated (no negatives, no non-integers, `scheduler` must be either `fifo` or `edf`).

### Log format (exact)

All logs are timestamped in milliseconds and strictly serialized (one message per line):

```text
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

The `burned out` message must appear within 10 ms of the actual burnout event.

## Instructions

### Compilation

```bash
cd coders
make
```

The project is compiled with `cc -Wall -Wextra -Werror -pthread`. The `Makefile` must not relink and provides the rules `$(NAME)`, `all`, `clean`, `fclean` and `re`. The executable is named `codexion` and must be generated in `coders/`.

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 4 800 200 100 100 3 50 fifo
./codexion 4 1000 200 100 100 5 50 edf
```

### Constraints

- Language: **C**
- Must pass **norminette** on all files in `coders/` (including headers). A single norm error results in `0`.
- No global variables
- No `libft` allowed
- Only authorized functions: `pthread_create`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_destroy`, `pthread_cond_init`, `pthread_cond_wait`, `pthread_cond_timedwait`, `pthread_cond_broadcast`, `pthread_cond_destroy`, `gettimeofday`, `usleep`, `write`, `malloc`, `free`, `printf`, `fprintf`, `strcmp`, `strlen`, `atoi`, `memset`
- No segfault, bus error, double free or unexpected termination
- No memory leaks (every `malloc` must be freed)
- Time is measured with `gettimeofday()` (real time)

## Resources

- [42 Norminette](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf) - Coding standard enforced by the project
- [The Little Book of Semaphores](https://greenteapress.com/semaphores/) - Reference for classical synchronization problems
- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/) - Pthreads concepts and patterns
- [`pthread` man pages](https://man7.org/linux/man-pages/man7/pthreads.7.html) - API reference for mutexes and condition variables
- [`gettimeofday` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) - Time source used for timestamps and deadlines
- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) - Conceptual analogy for shared resources and deadlock avoidance

### Use of AI

AI was used in a limited and controlled way during this project:

- **Norminette compliance:** Assistance with fixing formatting issues (tabs/spaces, alignment, blank lines, function separation) to satisfy the 42 Norm without altering program behavior.
- **Documentation:** Assistance generating this `README.md` to strictly follow the structure and points required by the subject/`AGENTS.md`.
- **Debugging assistance:** Occasional help reasoning about compiler warnings and norm errors.

All algorithmic design decisions (resource ordering, scheduler logic, heap implementation, burnout detection, synchronization strategy) were conceived, reviewed and validated against the subject constraints. AI output was never blindly copied into core logic without manual verification, testing and ensuring full compliance with the project's rules.

## Blocking cases handled

- **Deadlocks:** Dongles are always acquired in a fixed, deterministic order based on pointer addresses (`first > second` swap) so that all coders attempt to take resources in the same global order, eliminating circular wait.
- **Coffman conditions:** Circular wait is prevented by ordered acquisition. Hold-and-wait is avoided by acquiring both dongles as part of the protocol before entering the critical compile section. Mutual exclusion is enforced by per-dongle mutexes.
- **Starvation:** Each dongle maintains its own priority queue (custom binary heap). Requests are ordered by `key` (sequence number for `fifo`, deadline `last_compile + time_to_burnout` for `edf`) with a stable tiebreaker (`seq`). This guarantees fair arbitration per resource according to the selected scheduler.
- **Cooldown management:** When releasing a dongle, `cooldown_until = get_time_ms() + dongle_cooldown` is set and the dongle is marked not immediately free. Waiting threads are blocked until the cooldown has expired, using condition variables with timed waits (`wait_until_ms`).
- **Precise burnout detection:** A separate `monitor_loop` thread polls coder deadlines independently from worker threads. On detection, it sets the global stop flag under `stop_mutex`, prints the exact `burned out` log under `log_mutex` and terminates the simulation. The logging happens at the moment of detection to satisfy the "within 10 ms" requirement.
- **Log serialization:** All user-visible messages go through `print_log()` protected by `log_mutex`, ensuring no two log lines are ever interleaved.
- **Race conditions:** Shared state is protected explicitly: `stop_mutex` guards `sim->stop`, `log_mutex` guards output, and each dongle's `mutex` guards its heap, `is_free` flag, `cooldown_until`, and request queue. Condition variables coordinate wake-ups on state changes.
- **Stop propagation:** Coders check `sim_stopped()` at critical points (after acquiring dongles, between phases) and release held dongles before exiting to avoid leaving resources in an inconsistent state during termination.

## Thread synchronization mechanisms

### Primitives used

- **`pthread_mutex_t`**
  - `stop_mutex` (in `t_sim`): Protects the global `stop` flag read/written by coders and the monitor.
  - `log_mutex` (in `t_sim`): Serializes all log output to stdout.
  - Per-dongle `mutex` (in `t_dongle`): Protects the priority heap, `is_free`, `cooldown_until`, `arrival` sequence and all request queue operations.
- **`pthread_cond_t`**
  - Per-dongle `cond` (in `t_dongle`): Used to block threads waiting for their turn, for the dongle to become free, or for the cooldown period to expire. Signaled/broadcast on state changes (`release_dongle` does `pthread_cond_broadcast`).
- **Custom synchronization events**
  - **Priority heap per dongle:** A hand-rolled binary heap (`heap_push`/`heap_pop`) ordered by `(key, seq)` enforces FIFO or EDF. This acts as a fair queueing mechanism integrated with the per-dongle lock.
  - **Timed wait helper:** `wait_until_ms()` combines condition variable waiting with deadline-based checks using `gettimeofday()` to wait until `cooldown_until` without busy spinning.
  - **Ordered resource acquisition:** `acquire_dongles()` determines `first`/`second` by pointer comparison to enforce a total order on dongles.

### How they coordinate shared resources

- **Dongle access:** A coder enqueues a `t_request` (with its computed `key` depending on scheduler and `last_compile`) into the target dongle's heap under the dongle's mutex. It then waits until it is at the top of the heap, the dongle is free, and cooldown has elapsed. Once granted, it removes itself from the heap, marks the dongle busy, logs `has taken a dongle`, and unlocks the mutex.
- **Cooldown enforcement:** On `release_dongle()`, the mutex is taken, `cooldown_until = now + cooldown_ms` is set, `is_free = 1`, `pthread_cond_broadcast(&cond)` wakes all waiters, then the mutex is released. Waiters re-evaluate the conditions and only proceed when `get_time_ms() >= cooldown_until`.
- **Logging:** `print_log()` locks `log_mutex`, computes `get_time_ms() - start_time` (or current time as formatted by implementation), prints the exact required string, and unlocks. This prevents interleaved output across all threads.
- **Monitor coordination:** The monitor thread reads coder state and deadlines without modifying coder execution flow except by setting `sim->stop` under `stop_mutex`. Coders test `sim_stopped()` frequently (and also handle stop immediately after taking dongles) to exit cleanly and release any held dongles.
- **Thread lifecycle:** `run_sim()` spawns one thread per coder and one monitor thread, then joins all of them. All threads are joined before cleanup to ensure proper synchronization on exit.
- **Race avoidance examples:** 
  - **Heap corruption:** All heap mutations are serialized by the owning dongle's mutex.
  - **Double-grant / use-after-release:** Grant is only given when heap top matches the requesting coder AND `is_free == 1` AND cooldown passed; state changes are all under the same mutex.
  - **Spurious wakeups:** The while-loop condition re-checks all predicates after every `pthread_cond_wait`/timed wait.
  - **Stop/start races:** Stop flag is mutex-protected and checked after blocking operations.
  - **Logging races:** Every `printf`-based log is inside `log_mutex` critical section.
