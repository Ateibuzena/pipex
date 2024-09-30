# Pipex
Pipex is a C project that simulates UNIX pipes, allowing users to connect multiple commands. It reads input from one file and writes output to another, mimicking shell behavior. This project enhances skills in C programming, system calls, and memory management while providing hands-on experience with essential UNIX concepts.

## Summary
This project will allow you to explore the functionality of a UNIX mechanism you already know through the use of your program.

## Version
3

## Table of Contents
- [I. Introduction](#i-introduction)
- [II. General Instructions](#ii-general-instructions)
- [III. Mandatory Part](#iii-mandatory-part)
  - [III.1. Examples](#iii1-examples)
  - [III.2. Requirements](#iii2-requirements)
- [IV. Extra Part](#iv-extra-part)
- [V. Submission and Peer Evaluation](#v-submission-and-peer-evaluation)

---

## I. Introduction
Cristina: “Go somewhere to dance salsa :)”

## II. General Instructions
- Your project must be written in C.
- It must follow the Norm; any errors in additional files or functions will result in a score of 0.
- Your functions must not terminate unexpectedly (segfault, bus error, etc.). Such errors will result in a score of 0 during evaluation.
- All heap-allocated memory must be properly freed when necessary.
- If the prompt requires it, you must submit a Makefile that compiles your source files with the flags `-Wall`, `-Werror`, and `-Wextra`, using `cc`. The Makefile must not relink.
- The Makefile must contain at least the rules: `$(NAME)`, `all`, `clean`, `fclean`, and `re`.
- To submit bonuses, you should include a `bonus` rule in your Makefile.
- If your project allows the use of `libft`, copy its source and Makefile into a `libft` directory.
- It is recommended to create test programs for your project to verify its correct functionality.
- Submit your work in your assigned Git repository; only this work will be evaluated.

## III. Mandatory Part

### Program Name
`pipex`

### Files to Submit
- `Makefile`
- `*.h`
- `*.c`

### Arguments
```c
file1 command1 command2 file2
```

### Authorized Functions
- `open`, `close`, `read`, `write`, `malloc`, `free`, `perror`, `strerror`, `access`, `dup`, `dup2`, `execve`, `exit`, `fork`, `pipe`, `unlink`, `wait`, `waitpid`
- `ft_printf` and any equivalent you coded.

### Description
This project is about handling pipes.

Your program should be executed as follows:
```c
./pipex file1 command1 command2 file2
```
It should take 4 arguments:
- `file1` and `file2` are file names.
- `command1` and `command2` are shell commands with their respective parameters.

The execution of the `pipex` program should mimic the following command:
```
$> < file1 command1 | command2 > file2
```

#### III.1. Examples
```
$> ./pipex infile "ls -l" "wc -l" outfile
```
Should behave the same as:
```
<infile ls -l | wc -l >outfile
```
```
$> ./pipex infile "grep a1" "wc -w" outfile
```
Should behave the same as:
```
<infile grep a1 | wc -w >outfile
```
#### III.2. Requirements
- You must submit a Makefile that compiles your source files without relinking.
- You must manage errors meticulously; the program must not terminate unexpectedly.
- There must be no memory leaks.
- If you have any doubts, manage errors like the shell command does.

## IV. Extra Part
- Manage multiple pipes:
    ```bash
    $> ./pipex file1 command1 command2 command3 ... commandN file2
    ```
  Should behave like:
    ```bash
    < file1 command1 | command2 | command3 ... | commandN > file2
    ```

- Accept `<<` and `>>` when the first parameter is “here_doc”:
    ```bash
    $> ./pipex here_doc LIMITER command command1 file
    ```
  Should behave like:
    ```bash
    command << LIMITER | command1 >> file
    ```

Bonuses will only be evaluated if your mandatory part is perfect.

## V. Submission and Peer Evaluation
As usual, submit your work in your Git repository. Only the work uploaded in your repository will be evaluated. Make sure to double-check the names of your files to ensure they are correct.

