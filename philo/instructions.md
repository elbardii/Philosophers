# Philosophers - Mandatory Part

**I never thought philosophy would be so deadly**

Version: 12.0

## Project Summary

In this project, you will learn the basics of threading a process. You will learn how to create threads and explore the use of mutexes.

---

## Problem Overview

Here are the key things you need to know to succeed in this assignment:

- One or more philosophers sit at a round table. There is a large bowl of spaghetti in the middle of the table.
- The philosophers take turns eating, thinking, and sleeping.
  - While they are eating, they are not thinking nor sleeping;
  - while thinking, they are not eating nor sleeping;
  - and, of course, while sleeping, they are not eating nor thinking.
- There are also forks on the table. There are **as many forks as philosophers**.
- Since eating spaghetti with just one fork is impractical, a philosopher must pick up both the fork to their right and the fork to their left before eating.
- When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.
- Every philosopher needs to eat and should never starve.
- Philosophers do not communicate with each other.
- Philosophers do not know if another philosopher is about to die.
- Needless to say, philosophers should avoid dying!

---

## Global Rules

### Critical Requirements
- **Global variables are forbidden!**
- **Your program must not have any data races.**

### Program Arguments
Your program must take the following arguments:
```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers and also the number of forks.
- `time_to_die` (in milliseconds): If a philosopher has not started eating within `time_to_die` milliseconds since the start of their last meal or the start of the simulation, they die.
- `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
- `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
- `number_of_times_each_philosopher_must_eat` (optional argument): If all philosophers have eaten at least `number_of_times_each_philosopher_must_eat` times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

### Philosopher Numbering
- Each philosopher has a number ranging from 1 to `number_of_philosophers`.
- Philosopher number 1 sits next to philosopher number `number_of_philosophers`.
- Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher N + 1.

### Program Logs
Any state change of a philosopher must be formatted as follows:
- `timestamp_in_ms X has taken a fork`
- `timestamp_in_ms X is eating`
- `timestamp_in_ms X is sleeping`
- `timestamp_in_ms X is thinking`
- `timestamp_in_ms X died`

Replace `timestamp_in_ms` with the current timestamp in milliseconds and X with the philosopher number.

**Important logging rules:**
- A displayed state message should not overlap with another message.
- A message announcing a philosopher's death must be displayed within 10 ms of their actual death.
- Again, philosophers should avoid dying!

---

## Mandatory Part Specifications

| Field | Value |
|-------|-------|
| **Program name** | philo |
| **Turn in files** | Makefile, *.h, *.c, in directory philo/ |
| **Makefile** | NAME, all, clean, fclean, re |
| **Arguments** | number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat] |
| **External functs.** | memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock |
| **Libft authorized** | No |
| **Description** | Philosophers with threads and mutexes |

### Specific Implementation Rules
- **Each philosopher must be represented as a separate thread.**
- There is one fork between each pair of philosophers. Therefore, if there are several philosophers, each philosopher has a fork on their left side and a fork on their right side. If there is only one philosopher, they will have access to just one fork.
- **To prevent philosophers from duplicating forks, you should protect each fork's state with a mutex.**

---

## Technical Requirements

### Threading
- Use `pthread_create()` to create philosopher threads
- Use `pthread_join()` or `pthread_detach()` appropriately
- Each philosopher runs as an independent thread

### Synchronization
- Use mutexes to protect shared resources (forks)
- Use `pthread_mutex_init()`, `pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_mutex_destroy()`
- Prevent data races between threads

### Timing
- Use `gettimeofday()` for timestamps
- Use `usleep()` for sleeping/eating durations
- All times are in milliseconds

### Memory Management
- All heap-allocated memory must be properly freed
- No memory leaks tolerated
- Handle cleanup on program termination

---

## Common Instructions

- Your project must be written in C.
- Your project must be written in accordance with the Norm.
- Your functions should not quit unexpectedly (segmentation fault, bus error, double free, etc.) except for undefined behavior.
- Your Makefile must compile your source files with the flags -Wall, -Wextra, and -Werror, using cc.
- Your Makefile must contain at least the rules $(NAME), all, clean, fclean and re.
- Submit your work to the assigned Git repository in the `philo/` directory.

---

## Key Implementation Challenges

1. **Deadlock Prevention**: Ensure philosophers don't all grab their left fork simultaneously
2. **Starvation Prevention**: Make sure all philosophers get a chance to eat
3. **Race Condition Prevention**: Properly synchronize access to shared resources
4. **Death Detection**: Monitor philosopher states and detect death within 10ms
5. **Clean Termination**: Properly cleanup threads and mutexes when simulation ends

---

## Testing Scenarios

Consider testing with these scenarios:
- `./philo 1 800 200 200` - Single philosopher (should die)
- `./philo 5 800 200 200` - Basic case
- `./philo 5 800 200 200 7` - With eating limit
- `./philo 4 410 200 200` - Tight timing
- `./philo 2 310 200 100` - Two philosophers, tight timing