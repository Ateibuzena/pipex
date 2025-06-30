# Pipex - Process and Pipe Management Project in C

## 📁 Project Structure

The project is organized in a modular and clear way to facilitate both compilation and code understanding:

´´´
├── libft/
│ ├── libft.h
│ ├── Makefile
│ ├── README.md
│ └── src/
│ ├── d_ascii/
│ ├── d_fds/
│ ├── d_get_next_line/
│ ├── d_lists/
│ ├── d_memory/
│ ├── d_printf/
│ └── d_strings/
├── Makefile
├── pipexft.h
├── README.md
└── src/
├── bonus/
│ └── pipex_bonus.c
├── ft_execute.c
├── ft_process.c
├── ft_utils.c
└── pipex.c
´´´

* **libft/**: Custom library with reusable functions organized by category.  
* **src/**: Main Pipex source code, split into files by functionality (execution, processes, utilities).  
* **bonus/**: Extra features and improvements implemented for the extended version.  
* **Makefile and root files**: Manage compilation and define structures and prototypes used throughout the project.

---

## 🚀 Process Division: First, Intermediate, and Last

A key part of this project is how the execution is divided into processes to simulate a chain of pipes like in a real shell.

### First Process

* Handles opening the input file or managing `here_doc` if enabled.  
* Redirects standard input and connects output to the first pipe.  
* Executes the first command in the chain.

### Intermediate Processes

* Manage communication between the first and last command when multiple commands exist in the pipeline.  
* Each process reads input from the previous pipe and writes to the next one.  
* Properly handle opening and closing file descriptors to avoid leaks and conflicts.

### Last Process

* Receives input from the previous pipe and redirects output to the specified output file.  
* Adjusts opening modes (append or truncate) depending on whether `here_doc` is active.  
* Executes the last command and closes the pipeline flow.

---

This organization allows:

* **Clarity:** Each process has a defined and separate role.  
* **Robustness:** Proper error handling and descriptor closing at each stage.  
* **Scalability:** Easy to extend for more commands or extra features.

---

If you want, I can help you add more technical details or prepare examples for the documentation. How do you feel about this structure?
