# wait3() Example

This example demonstrates how to use the `wait3()` system call to wait for a child process to finish and collect detailed resource usage information about it.

## What is wait3()?

`wait3()` is a BSD extension to POSIX that allows a parent process to:
- Wait for **any child process** to change state (exit or stop).
- Retrieve the **exit status** of that child.
- Collect **performance/resource statistics** using a `struct rusage`.

## Key Features

- Uses `fork()` to create a child process.
- Child sleeps for 2 seconds and exits with code 42.
- Parent waits with `wait3()`, prints:
  - Child exit status
  - CPU time (user + system)
  - Memory and performance stats (from `struct rusage`)

## How to Compile & Run

In this folder, run:

    make
    ./wait3_test

Sample output:

    Parent process: waiting for child...
    Child process: PID = 90211
    Child exited with status: 42
    Resource usage:
      User CPU time      : 0.000159 sec
      System CPU time    : 0.000179 sec
      Max resident set   : 466944 KB
      Shared memory size : 0
      Unshared data size : 0
      Unshared stack size: 0
      Page reclaims (soft page faults): 130
      Page faults (hard) : 0
      Swaps              : 0
      Block input ops    : 0
      Block output ops   : 0
      Voluntary context switches  : 0
      Involuntary context switches: 2

## What is struct rusage?

This struct contains performance info for the child process:
- `ru_utime`: Time spent in user mode
- `ru_stime`: Time in system (kernel) mode
- `ru_maxrss`: Max memory usage (resident set)
- `ru_minflt`, `ru_majflt`: Page faults
- `ru_nvcsw`, `ru_nivcsw`: Context switches
- And more...

Some fields may be 0 on macOS or depending on system support.

## Related System Calls

- `wait()`, `waitpid()`, `wait4()`
- `fork()`, `getpid()`
- `exit()`

---

Author: Levent Akdogan  
School: 42 Heilbronn  
Date: May 11, 2025
