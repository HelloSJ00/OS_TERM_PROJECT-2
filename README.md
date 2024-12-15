# OS Term Project #2

# 객체지향적으로 만들어본 Virtual Memory

---

## 실행

| **Action**           | **Details**                                                       |
| -------------------- | ----------------------------------------------------------------- |
| **Clone Repository** | `git clone <repository-url>`                                      |
| **환경**             | - **Operating System:** macOS<br>- **Editor:** Visual Studio Code |
| **컴파일 명령어**    | `g++ -std=c++17 -o main main.cpp \                                |

CLASS/KERNEL/Kernel.cpp \
CLASS/MEMORY/Memory.cpp \
CLASS/PAGETABLE/PageTable.cpp \
CLASS/PTE/PTE.cpp \
CLASS/USER/User.cpp`|
| **실행**             |`./main` |

---

## Project 2: Multi-process execution with Virtual Memory (Paging)

**Programming assignment #2 due by Dec. 15. 2024 (11:59pm) KST**

### Introduction

In the previous project, we simulated scheduling to enable multiple processes to run concurrently. Based on that, we will now add **virtual memory** to each process. Virtual memory is a powerful concept that:

1. Simplifies program development.
2. Protects programs from malicious interference by other programs.

With virtual memory, each process can access memory as if it owns the entire memory, even though it does not in reality. This allows software engineers to avoid concerns about the physical memory location of variables, code, stack, etc. Instead, each process runs with its own **address space**, which represents the set of addresses that the process can use. This abstraction is implemented using architectural support for **paging** and/or **segmentation**.

### Paging Overview

**Paging** implements address translation between physical addresses (PA) and virtual addresses (VA):

- Programs are written using virtual addresses (VA).
- The CPU only sees virtual addresses.
- The operating system (OS) maintains a **page table** to map virtual addresses to physical addresses.

#### Key Concepts

1. **Page and Page Frame**:

   - A page is a small, fixed-size subset of the address space.
   - Physical memory is divided into fixed-size blocks called **page frames**.
   - Typical page size: **4 KB**.

2. **Page Frame Number**:

   - Each page frame is numbered (e.g., Frame 0 for addresses 0 to 0xFFF, Frame 1 for addresses 0x1000 to 0x1FFF, etc.).

3. **MMU (Memory Management Unit)**:

   - A hardware component within the CPU that translates virtual addresses to physical addresses using a page table.
   - It accesses the **page table**, which resides in memory, and retrieves the corresponding physical address.

4. **Demand Paging**:
   - Pages are allocated at runtime.
   - The OS maintains a list of free page frames at boot time.
   - Initially, page table entries are marked as invalid.
   - When a process accesses an invalid page, a **page fault** occurs, and the OS allocates a page frame, updates the page table, and resumes the process.

---

### Implementation Details

#### General Requirements

- **Kernel and User Processes**:

  - One process (parent) acts as the kernel.
  - Ten processes act as user processes.

- **Memory Management**:

  - Physical memory is fragmented into page-sized units.
  - The OS maintains a **free page list**.

- **Page Tables**:

  - The OS allocates an empty page table for each new process, initialized with invalid entries.

- **Process Execution**:

  - When a process gets a time slice, it accesses virtual memory (10 pages per slice).
  - If the page table entry is valid, the physical address is accessed.
  - If invalid, a free page frame is allocated, and the page table is updated.

- **Virtual Address Structure**:

  - Virtual addresses are divided into two parts: **page number** and **page offset**.

- **MMU**:
  - Points to the starting address of the page table.

#### Optional Module: Swapping

- Implement a **swapping mechanism** to handle cases where virtual memory exceeds physical memory.
- Use algorithms such as **LRU** to select pages for eviction.
- Implement additional features such as **copy-on-write** for bonus credit.

#### Reporting Requirements

- Simulate 10,000 ticks of execution.
- Log:
  - Virtual addresses accessed.
  - Physical addresses accessed.
  - Page fault events.
  - Page table changes.
  - Read/write values.
- Include:
  - Scheduling parameters.
  - Physical memory size (chosen based on reasonable assumptions).

#### Submission

- Provide a **PDF-formatted report** summarizing your results.
- Submit a zip file containing:
  - Source code.
  - Logs.

---

### Contacts

- **TA**: Jaeyeol Choi (jaychoi@skku.edu)
- **Professor**: Eun-Seok Ryu (esryu@skku.edu)

---
