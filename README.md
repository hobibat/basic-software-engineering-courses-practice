## Environment & System Architecture

This project explores low-level operating system concepts (such as process creation and memory isolation) using POSIX standard C functions like `fork()` and `wait()`. 

### The Engineering Challenge
* **POSIX vs. Windows:** POSIX (Portable Operating System Interface) is a standard used by Unix/Linux kernels to handle privileged operations (like process management) via system calls. Standard Windows environments use a different proprietary kernel API.
* **Compilation Constraint:** When compiling on a native Windows toolchain (such as MSVC or standard MinGW), compilers throw header resolution errors (`<unistd.h>`, `<sys/wait.h>`) because Windows lack native POSIX system call definitions.

### The Solution: WSL & Tooling Setup
To execute and compile low-level Linux code on a Windows host without heavy traditional virtualization:
1. **Windows Subsystem for Linux (WSL 2):** Utilizes an authentic, lightweight Linux kernel running natively on Windows to handle POSIX system calls.
2. **VS Code Remote-WSL:** Connects the editor workspace directly to the Linux container namespace, mapping system headers and the GNU Compiler Collection (`gcc` via `build-essential`) accurately.

### Quick Start
```bash
# Ensure you are inside the WSL terminal environment
cd 01_os_lowlevel_c
gcc 01_fork_tree.c -o 01_fork_tree
./01_fork_tree
