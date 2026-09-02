# SIDDHCESNA-III: Project Overview & Architecture Specification

## Architectural Constraints Reference

* **Word Size:** 16-bit

* **Instruction Format:** `[5-bit opcode][11-bit Parameters]`

* **RAM Size:** 64 KB

* **Flags (5 status lines):** Zero, Greater, Smaller, Overflow, Vblank

### Register Map (16 Total)

* `r0` - `r11` -> General Purpose Registers (GPR)

* `r12` -> Jump Target Register (JTR) [Holds target addresses for all JMP/Branching ops]

* `r13` -> Accumulator (ACC) [Default destination for ALU operations]

* `r14` -> Stack Pointer (SP)

* `r15` -> Memory Address Register (MAR)

### Memory Allocation

* `0x0000` - `0x0BFF` (0 - 3071) -> Video Memory (3 KB, 1-bit color depth for 256 * 192 display)

* `0x0C01` - `0x1C00` (3073 - 7168)-> Stack Memory (4 KB)

* `0xFFFF` -> Keyboard Input MMIO

### Display & Clock Configuration

* **Display Resolution:** 256 * 192 = 49,152 pixels

* **Refresh Rate:** 60 Hz

* **Raw Display Bandwidth:** 49,152 * 60 = 2,949,120 Hz (~2.95 MHz)

* **System Clock Speed:** 4.0 MHz (~1.05 million spare cycles per frame available for processing logic)

---

## 1. System Operations

### 0: NOP (No Operation)

* **Description:** Do nothing for one clock cycle.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[00000][00000000000]`

### 27: HALT (Halt Execution)

* **Description:** Stops processor execution loops until external reset.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[11011][00000000000]`

---

## 2. ALU-Based Operations

*All arithmetic/logic results are implicitly saved into the Accumulator (`r13`) unless stated otherwise.*

### 1: ADD (Addition)

* **Description:** Adds contents of `src1` and `src2`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00001][reg_id1][reg_id2][000]`

### 2: SUB (Subtraction)

* **Description:** Subtracts `src2` from `src1`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00010][reg_id1][reg_id2][000]`

### 3: MUL (Multiplication)

* **Description:** Multiplies `src1` by `src2`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00011][reg_id1][reg_id2][000]`

### 4: DIV (Division)

* **Description:** Divides `src1` by `src2`. Stores the integer quotient.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00100][reg_id1][reg_id2][000]`

### 5: AND (Bitwise AND)

* **Description:** Bitwise AND between `src1` and `src2`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00101][reg_id1][reg_id2][000]`

### 6: OR (Bitwise OR)

* **Description:** Bitwise OR between `src1` and `src2`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00110][reg_id1][reg_id2][000]`

### 7: XOR (Bitwise XOR)

* **Description:** Bitwise XOR between `src1` and `src2`.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[00111][reg_id1][reg_id2][000]`

### 8: NOT (Bitwise NOT)

* **Description:** Inverts all bits of the `src` register.

* **Format:** `[5-bit opcode][4-bit src][7-bit unused]`

* **Binary:** `[01000][reg_id][0000000]`

### 9: BSL (Bitshift Left)

* **Description:** Shifts bits of `src` left by the value inside `amt` register.

* **Format:** `[5-bit opcode][4-bit src][4-bit amt][3-bit unused]`

* **Binary:** `[01001][reg_id1][reg_id2][000]`

### 10: BSR (Bitshift Right)

* **Description:** Shifts bits of `src` right by the value inside `amt` register.

* **Format:** `[5-bit opcode][4-bit src][4-bit amt][3-bit unused]`

* **Binary:** `[01010][reg_id1][reg_id2][000]`

### 11: CMP (Compare)

* **Description:** Subtracts `src2` from `src1` to update internal flags. Result is discarded.

* **Format:** `[5-bit opcode][4-bit src1][4-bit src2][3-bit unused]`

* **Binary:** `[01011][reg_id1][reg_id2][000]`

---

## 3. Immediate & Data Movement

### 12: LUI (Load Upper Immediate)

* **Description:** Loads a 7-bit literal into middle bits `[13:7]` of `dest`.

* **Format:** `[5-bit opcode][4-bit dest][7-bit immediate]`

* **Binary:** `[01100][reg_id][imm_7bit]`

### 13: LLI (Load Lower Immediate)

* **Description:** Loads a 7-bit literal into lower bits `[6:0]` of `dest`. Preserves higher bits.

* **Format:** `[5-bit opcode][4-bit dest][7-bit immediate]`

* **Binary:** `[01101][reg_id][imm_7bit]`

### 22: MOV (Move Data)

* **Description:** Copies content from `src` register to `dest` register.

* **Format:** `[5-bit opcode][4-bit dest][4-bit src][3-bit unused]`

* **Binary:** `[10110][dest_id][src_id][000]`

### 28: LRI (Load Remaining Immediate)

* **Description:** Loads a 2-bit literal into top bits `[15:14]` of `dest`. Combines with LLI and LUI for complete 16-bit constants.

* **Format:** `[5-bit opcode][4-bit dest][2-bit immediate][5-bit unused]`

* **Binary:** `[11100][reg_id][imm_2bit][00000]`

---

## 4. Control Flow & Branching

*All jump operations implicitly look up the 16-bit destination address inside register `r12`.*

### 14: JMP (Unconditional Jump)

* **Description:** Unconditionally copies the address from `r12` into the Program Counter (PC).

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[01110][00000000000]`

### 15: JZ (Jump if Zero)

* **Description:** Copies address from `r12` into PC if Zero flag is active.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[01111][00000000000]`

### 16: JG (Jump if Greater)

* **Description:** Copies address from `r12` into PC if Greater flag is active.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[10000][00000000000]`

### 17: JS (Jump if Sign / Negative)

* **Description:** Copies address from `r12` into PC if Smaller (Sign) flag is active.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[10001][00000000000]`

### 18: JO (Jump if Overflow)

* **Description:** Copies address from `r12` into PC if Overflow flag is active.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[10010][00000000000]`

### 19: JVB (Jump if Vblank)

* **Description:** Copies address from `r12` into PC if vertical blanking hardware signal is active.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[10011][00000000000]`

---

## 5. Memory & Stack Operations

### 20: STR (Store to Memory)

* **Description:** Writes content of `src` register into RAM address stored in `r15` (MAR).

* **Format:** `[5-bit opcode][4-bit src][7-bit unused]`

* **Binary:** `[10100][reg_id][0000000]`

### 21: LOAD (Load from Memory)

* **Description:** Reads RAM content from address stored in `r15` (MAR) into `dest` register.

* **Format:** `[5-bit opcode][4-bit dest][7-bit unused]`

* **Binary:** `[10101][reg_id][0000000]`

### 23: PUSH (Push onto Stack)

* **Description:** increments stack pointer `r14`, then saves `src` value at that RAM address.

* **Format:** `[5-bit opcode][4-bit src][7-bit unused]`

* **Binary:** `[10111][reg_id][0000000]`

### 24: POP (Pop from Stack)

* **Description:** Reads RAM value from stack pointer `r14` address into `dest`, then decrements `r14`.

* **Format:** `[5-bit opcode][4-bit dest][7-bit unused]`

* **Binary:** `[11000][reg_id][0000000]`

### 25: CALL (Call MAR)

* **Description:** Pushes the next sequential PC address onto the stack. Unconditionally branches to address inside `r15` (MAR).

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[11001][00000000000]`

### 26: RET (Return)

* **Description:** Pops the top address value from stack (`r14`) and updates the PC.

* **Format:** `[5-bit opcode][11-bit unused]`

* **Binary:** `[11010][00000000000]`


