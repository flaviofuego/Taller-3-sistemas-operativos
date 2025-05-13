# Process Communication and Memory Management Workshop

## Overview

This repository contains the implementation of a comprehensive workshop on process communication and dynamic memory management in Linux. The project demonstrates the practical application of key concepts like process creation (`fork()`), inter-process communication (`pipe()`), and memory management (`malloc()`, `calloc()`) in C programming.

## Educational Purpose

The main objectives of this workshop are:

1. Understanding dynamic memory allocation in C using both `malloc()` and `calloc()`
2. Practicing array handling using both index arithmetic and pointer arithmetic
3. Learning process creation and inter-process communication in Linux
4. Developing good memory management practices (proper allocation and deallocation)

## Problem Statement

The project processes student grades organized by sections. The program:

1. Reads data from a text file
2. Creates child processes for each section using `fork()`
3. Sends section data to children via `pipe()`
4. Each child calculates and returns the average grade for its section
5. The parent process compiles results and identifies the section with the highest average

Four different implementations are provided, each using a different combination of memory allocation functions and array access methods:

- `notas_malloc_indices.c`: Uses `malloc()` with array indexing syntax
- `notas_malloc_punteros.c`: Uses `malloc()` with pointer arithmetic
- `notas_calloc_indices.c`: Uses `calloc()` with array indexing syntax
- `notas_calloc_punteros.c`: Uses `calloc()` with pointer arithmetic

## Repository Structure

```
.
├── Dockerfile                # Docker configuration
├── entrypoint.sh             # Script for automatic compilation
├── README.md                 # This documentation file
├── recompile.sh              # Script to recompile all programs
├── notas.txt                 # Input data file
├── notas_malloc_indices.c    # Implementation using malloc and array indices
├── notas_malloc_punteros.c   # Implementation using malloc and pointer arithmetic
├── notas_calloc_indices.c    # Implementation using calloc and array indices
└── notas_calloc_punteros.c   # Implementation using calloc and pointer arithmetic
```

## Requirements

To run this project, you'll need:

- Docker installed on your system
- Basic knowledge of terminal/command-line operations

## Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/linux-process-memory-workshop.git
cd linux-process-memory-workshop
```

### 2. Build the Docker Image

```bash
docker build -t taller3-procesos-linux .
```

### 3. Run the Docker Container

```bash
# On Linux/macOS
docker run -it --name taller3 -v $(pwd):/app taller3-procesos-linux

# On Windows (PowerShell)
docker run -it --name taller3 -v ${PWD}:/app taller3-procesos-linux
```

This command creates a container with your local directory mounted as a volume, allowing you to edit files on your host system and have the changes reflected inside the container.

## Usage

Once inside the container, the programs are automatically compiled by the `entrypoint.sh` script. You can run each version:

```bash
./notas_malloc_indices
./notas_malloc_punteros
./notas_calloc_indices
./notas_calloc_punteros
```

### Expected Output

Each program will produce output like:

```
--- Sección 1 ---
Notas: 4.50 3.20
--- Sección 2 ---
Notas: 2.80 3.10 3.90
--- Sección 3 ---
Notas: 4.00 4.20
Promedio sección 1: 3.85
Promedio sección 2: 3.27
Promedio sección 3: 4.10
Promedio general: 3.74
Mejor sección: 3 con promedio 4.10
```

### Recompiling After Changes

If you make changes to the source files, use the provided script to recompile:

```bash
./recompile.sh
```

### Memory Leak Testing

The Docker environment includes `valgrind` for memory leak detection:

```bash
valgrind ./notas_malloc_indices
```

## Key Concepts Explained

### Dynamic Memory Allocation

- **malloc()**: Allocates requested memory but does not initialize it
  ```c
  int *arr = (int *)malloc(n * sizeof(int));
  ```

- **calloc()**: Allocates and initializes memory to zero
  ```c
  int *arr = (int *)calloc(n, sizeof(int));
  ```

### Array Access Methods

- **Array indexing**: Uses square bracket notation
  ```c
  arr[i] = 10;  // Accessing the ith element
  ```

- **Pointer arithmetic**: Uses pointer manipulation
  ```c
  *(arr + i) = 10;  // Equivalent to arr[i] = 10
  ```

### Process Communication

- **fork()**: Creates a child process
  ```c
  pid_t pid = fork();  // Returns child PID to parent and 0 to child
  ```

- **pipe()**: Creates a unidirectional communication channel
  ```c
  int pipe_fd[2];  // pipe_fd[0] for reading, pipe_fd[1] for writing
  pipe(pipe_fd);
  ```

- **wait()**: Allows a parent to wait for its child processes to finish
  ```c
  wait(NULL);  // Wait for any child to terminate
  ```

## Docker Management

- **Stop container**:
  ```bash
  docker stop taller3
  ```

- **Restart container**:
  ```bash
  docker start -i taller3
  ```

- **Remove container**:
  ```bash
  docker rm taller3
  ```

## Common Issues and Troubleshooting

1. **Memory leaks**: Check that all dynamically allocated memory is freed using `free()` in both parent and child processes.

2. **Pipe deadlocks**: Ensure unused pipe ends are closed properly in all processes.

3. **File read errors**: Verify the `notas.txt` file is in the correct location and has the expected format.

4. **Compilation errors**: Use the `-Wall` flag during compilation to catch potential issues.

## License

This project is provided for educational purposes under the MIT License.

## Acknowledgments

This workshop was designed as part of a Linux systems programming course to teach fundamental concepts of process management and memory allocation in C.

---

*Note: For the purposes of this academic exercise, error handling has been implemented, but in a production environment, even more robust error checking would be recommended.*
