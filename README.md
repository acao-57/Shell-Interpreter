# Restricted Shell (rsh) Interpreter

A custom-built, restricted shell interpreter written in C. This project implements a functional shell that limits users to a predefined set of 12 essential commands, simulating a secure environment for interns. It demonstrates core systems programming concepts, including process spawning, built-in command handling, and user input parsing.

## Technical Implementation

The shell operates by:
1.  Displaying the `rsh>` prompt and reading user input.
2.  Parsing the input string into a command and its arguments.
3.  Handling built-in commands (`cd`, `exit`, `help`) directly.
4.  For external commands (`ls`, `cp`, `grep`, etc.), using the `posix_spawnp()` system call to create a new process and execute the program.
5.  Restricting execution to only the 12 allowed commands; any other command results in a `NOT ALLOWED!` error.

### Allowed Commands
1.  `cp`
2.  `touch`
3.  `mkdir`
4.  `ls`
5.  `pwd`
6.  `cat`
7.  `grep`
8.  `chmod`
9.  `diff`
10. `cd`      *(built-in)*
11. `exit`    *(built-in)*
12. `help`    *(built-in)*

### Key Functions and System Calls
- **`posix_spawnp()`:** Creates a new process to execute an external program.
- **`chdir()`:** Changes the current working directory (used for the `cd` built-in).
- **`fgets()`:** Reads a line of input from the user.
- **`strtok()`:** Parses the input string into tokens (command and arguments).

## How to Build & Run

1.  **Compile the program:**
    ```bash
    make
    ```
    This produces the `rsh` executable.

2.  **Run the shell:**
    ```bash
    ./rsh
    ```

3.  **Run with test input (example):**
    ```bash
    # Redirects stdin from a test file and suppresses prompt messages (sent to stderr)
    ./rsh < test_cases/test05.in 2>/dev/null > my_output.out
    ```

4.  **Compare your output to the expected output:**
    ```bash
    diff my_output.out test_cases/test05.out
    ```

## Built-in Command Details

| Command | Description | Implementation |
| :--- | :--- | :--- |
| `cd <dir>` | Changes directory. | Uses `chdir()` system call. Prints an error to **stdout** if too many arguments are given. |
| `exit` | Exits the shell. | Breaks the main loop and returns 0. |
| `help` | Shows allowed commands. | Prints the list of 12 commands to **stdout**. |

## Error Handling

The shell handles the following errors explicitly:

- **Invalid Command:** Prints `NOT ALLOWED!` to **stdout**.
- **`cd` with multiple arguments:** Prints `-rsh: cd: too many arguments` to **stdout**.
- All other errors (e.g., command not found, file not exist) are handled by the spawned processes themselves and their output is not controlled by `rsh`.

## Testing

The `test_cases` directory contains input files (`testXX.in`) and their expected output (`testXX.out`). To ensure correctness, test your implementation against all provided cases.

**Example Test Command:**
```bash
./rsh < test_cases/test02.in 2>/dev/null > mytest2.out
diff mytest2.out test_cases/test02.out
