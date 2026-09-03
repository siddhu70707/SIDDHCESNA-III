# SIDDHCESNA-III Custom 16-Bit Processor Architecture

An emulator and toolchain implementation for the **SIDDHCESNA-III**, a high-performance, keyboard-driven custom 16-bit computer architecture optimized for retro game processing and embedded simulation loops.

---

## Hardware Constraints Specification

* **Word Size:** 16-bit native registers and data paths.
* **Instruction Format:** Fixed-width `[5-bit opcode][11-bit Parameters]`.
* **RAM Size:** 64 KB total addressable byte space.
* **System Clock:** 4.0 MHz.
* **Display Output:** 256 × 192 resolution running at a crisp 60 Hz frame rate.
  * **Raw Bandwidth:** ~2.95 MHz.
  * **Overhead headroom:** ~1.05 million spare processing clock cycles available per frame for active software logic.

### Status Flags Layout (5 Hardware Lines)
* **Zero (Z):** Triggered if an ALU operation equals 0.
* **Greater (G):** Triggered if `src1 > src2` during a comparison.
* **Smaller (S):** Triggered if a calculation drops below 0 or `src1 < src2`.
* **Overflow (V):** Triggered when an operation overflows past 16 bits (`> 0xFFFF`).
* **Vblank (VB):** Hardware vertical blanking signaling window for safe video updates.

---

## Register Configuration Mapping (16 Total)

| Register | Name | Architectural Function Reference |
| :--- | :--- | :--- |
| `r0` – `r11` | **GPR** | General Purpose Registers for application data handling. |
| `r12` | **JTR** | Jump Target Register. Holds destination addresses for branching. |
| `r13` | **ACC** | Accumulator. Default target location for ALU operations. |
| `r14` | **SP** | Stack Pointer. Points to the current top of the system stack. |
| `r15` | **MAR** | Memory Address Register. Holds addresses for RAM load/store cycles. |

---

## 🧠 Memory Allocation Blueprint

```text
+-------------------+ 0xFFFF  (Keyboard Input MMIO)

|                   |
|    System RAM     |
|                   |
+-------------------+ 0x1C00

|   Stack Memory    | 4 KB Dedicated Stack Space
+-------------------+ 0x0C01

|   Unused / Free   | 0x0C00 Single byte split
+-------------------+ 0x0BFF

|   Video Memory    | 3 KB Framebuffer (256x192 @ 1-bit monochrome depth)
+-------------------+ 0x0000
```

---

## 🛠️ Complete Instruction Set Architecture (ISA)

### 1. System Operations
* **`0: NOP`** | `[00000][00000000000]` | Idle the execution core for 1 cycle.
* **`27: HALT`** | `[11011][00000000000]` | Freezes execution state until physical manual reset.

### 2. Arithmetic & Logic Unit (ALU)
*All operations default their calculation destinations into `r13` (ACC) unless noted otherwise.*
* **`1: ADD`** | `[00001][src1_4b][src2_4b][000]` | Adds `src1` to `src2`. Sets Zero/Overflow.
* **`2: SUB`** | `[00010][src1_4b][src2_4b][000]` | Subtracts `src2` from `src1`. Sets Zero/Smaller.
* **`3: MUL`** | `[00011][src1_4b][src2_4b][000]` | Multiplies `src1` by `src2`. Sets Zero/Overflow.
* **`4: DIV`** | `[00100][src1_4b][src2_4b][000]` | Integer division of `src1 / src2`. Protection fallback to `0xFFFF` on divide-by-zero.
* **`5: AND`** | `[00101][src1_4b][src2_4b][000]` | Bitwise AND.
* **`6: OR`** | `[00110][src1_4b][src2_4b][000]` | Bitwise OR.
* **`7: XOR`** | `[00111][src1_4b][src2_4b][000]` | Bitwise XOR.
* **`8: NOT`** | `[01000][src_4b][0000000]` | Bitwise NOT inversion of input source.
* **`9: BSL`** | `[01001][src_4b][amt_4b][000]` | Shifts bits left by value in `amt` register.
* **`10: BSR`**| `[01010][src_4b][amt_4b][000]` | Shifts bits right logically by value in `amt` register.
* **`11: CMP`**| `[01011][src1_4b][src2_4b][000]` | Updates flags (`Z`, `G`, `S`) by evaluating `src1` against `src2`. Discards numerical math.

### 3. Immediate Constants & Movement Data
* **`12: LUI`**| `[01100][dest_4b][imm_7bit]` | Loads literal to middle bits `[13:7]`.
* **`13: LLI`**| `[01101][dest_4b][imm_7bit]` | Loads literal to lower bits `[6:0]`, preserves remaining bits.
* **`22: MOV`**| `[10110][dest_4b][src_4b][000]` | Direct duplication copy from `src` register to `dest`.
* **`28: LRI`**| `[11100][dest_4b][imm_2bit][00000]` | Loads literal to top bits `[15:14]`. Combined with LLI/LUI for a complete 16-bit constant load.

### 4. Control Execution Flow
*All branching structures inspect the target destination jump address from register `r12` (JTR).*
* **`14: JMP`**| Unconditional jump configuration.
* **`15: JZ`** | Jump if **Zero** flag active.
* **`16: JG`** | Jump if **Greater** flag active.
* **`17: JS`** | Jump if **Smaller/Sign** flag active.
* **`18: JO`** | Jump if **Overflow** flag active.
* **`19: JVB`**| Jump if **Vertical Blanking** phase active.

### 5. Memory & Core Stack Bus
* **`20: STR`**| Writes `src` register value into RAM address indexed by `r15` (MAR).
* **`21: LOAD`**| Reads RAM data indexed by `r15` (MAR) into `dest` register.
* **`23: PUSH`**| Pre-increments `r14` (SP), then saves value to Stack memory location.
* **`24: POP`** | Reads Stack memory value into `dest`, then post-decrements `r14` (SP).
* **`25: CALL`**| Pushes current return execution PC onto stack, branches instantly to location in `r15` (MAR).
* **`26: RET`** | Pops address from stack memory directly back into PC register.
