# Philosophers

A C implementation of the classic Dining Philosophers concurrency problem. Multiple philosophers sit around a table and alternate between thinking, eating, and sleeping while competing for shared forks (resources). The challenge is to prevent deadlocks and starvation and to produce correct timing and output under concurrency.

- Threads: POSIX threads (`pthread`)
- Synchronization: mutexes
- Timing: millisecond precision

## Features

- Thread-per-philosopher architecture.
- Mutex-protected forks and printing.
- Proper simulation stop conditions:
  - Death when a philosopher exceeds `time_to_die` since last meal.
  - Optional end when all philosophers have eaten `nb_must_eat` times.
- Special handling for the single philosopher case.
- Clean initialization and teardown of resources.

## Requirements

- C compiler compatible with `-Wall -Wextra -Werror`
- POSIX threads (pthread)
- Make

## Build

- Build: `make`
- Clean objects: `make clean`
- Clean objects and binary: `make fclean`
- Rebuild: `make re`

This produces the executable: `philo`

## Usage

```
./philo nb_philo time_to_die time_to_eat time_to_sleep [nb_must_eat]
```

- `nb_philo`: number of philosophers.
- `time_to_die`: time (ms) a philosopher survives without eating.
- `time_to_eat`: time (ms) spent eating.
- `time_to_sleep`: time (ms) spent sleeping.
- `nb_must_eat` (optional): target number of meals for each philosopher; if provided, simulation stops when all reach this count.

Example:
```
./philo 5 800 200 200
./philo 5 800 200 200 7
```

If arguments are invalid, the program prints an error (e.g., `Error: Invalid arguments`) or a usage line:
```
Usage: ./philo nb_philo time_to_die time_to_eat             time_to_sleep [nb_must_eat]
```

## Output Format

Each event is printed as:
```
<timestamp_ms> <philo_id> <event>
```

Where:
- `timestamp_ms` is milliseconds since simulation start.
- `philo_id` is 1-based.
- `<event>` is one of: `has taken a fork`, `is eating`, `is sleeping`, `is thinking`, `died`.

Printing is mutex-protected to avoid interleaved lines.

Special case when `nb_philo == 1`:
- The program prints:
  - `0 1 has taken a fork`
  - After `time_to_die` ms: `<time_to_die> 1 died`

## Behavior and Rules

- A philosopher must hold two forks (left and right) to eat.
- After eating for `time_to_eat` ms, the philosopher sleeps for `time_to_sleep` ms, then thinks.
- If the time since last meal exceeds `time_to_die`, the philosopher dies and the simulation stops.
- If `nb_must_eat` is provided, the simulation stops when all philosophers reach this count.

## Implementation Overview

- Threads are spawned per philosopher.
- Forks are modeled as an array of mutexes.
- Additional mutexes:
  - `print_mutex` to serialize output.
  - `data_mutex` to protect shared state such as `simulation_end`.
- Timekeeping uses millisecond timestamps; sleeps use `usleep` with appropriate conversions and helpers.
- The monitor/check logic detects death and the completion condition (if `nb_must_eat` is set).
- Single philosopher is handled explicitly to avoid deadlock (cannot acquire two forks).

## Project Structure

- `main.c`: Entry point. Argument validation, initialization, single-philosopher fast path, start/stop, cleanup.
- `init.c`: Parsing and preparing `t_data`, initializing philosophers, mutex setup.
- `simulation.c`: Simulation loop and monitoring, start and join threads, termination logic.
- `actions.c`: Philosopher actions (take forks, eat, sleep, think) and event logging.
- `utils1.c`, `utils2.c`: Helpers (time, safe sleep, parsing, printing, etc.).
- `philo.h`: Types (`t_data`, `t_philo`), function prototypes, includes, constants.
- `Makefile`: Build rules with `-pthread -Wall -Wextra -Werror -g`.
- `LICENSE`: License for this repository.

## Argument Constraints and Validation

- All numeric arguments must be positive integers fitting in standard 32-bit range.
- `nb_philo >= 1`.
- Times are in milliseconds.
- With 1 philosopher, simulation prints fork acquisition then death at `time_to_die`.

If validation fails, the program prints `Error: Invalid arguments`.

## Testing Tips

- Basic run:
  - `./philo 5 800 200 200`
- Death case:
  - `./philo 5 200 200 200` (likely deaths)
- Completion with `nb_must_eat`:
  - `./philo 5 800 200 200 5`
- Single philosopher:
  - `./philo 1 800 200 200`
- Look for:
  - No mixed/interleaved prints.
  - Realistic timing (tolerant to scheduling jitter).
  - No deadlocks (program should terminate or keep producing output).
  - Clean exit with all resources released (e.g., under valgrind on Linux).

## Troubleshooting

- Nothing prints:
  - Check argument order and ranges.
  - Ensure your terminal supports ANSI colors (build messages only).
- Early deaths:
  - Increase `time_to_die` relative to `time_to_eat + time_to_sleep`.
- Interleaved/mangled lines:
  - Ensure single `printf` per event and guarded by `print_mutex`.
- Hangs at end:
  - Verify all threads are joined and all mutexes are destroyed in cleanup paths.

## License

This project is released under the terms of the LICENSE file included in this repository.
