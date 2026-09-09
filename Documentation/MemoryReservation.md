================================================================================
SIDDHCESNA-III ARCHITECTURE: MEMORY RESERVATION MAP

Total Addressable RAM: 64 KB (16-bit word-addressable space)
Word Size: 16-bit

MEMORY SEGMENT ALLOCATIONS

[0] Video Memory (VRAM)
* Address Range: 0x0000 - 0x0BFF
* Decimal Range: 0 - 3071
* Size: 3 KB
* Color Depth: 1-bit per pixel
* Display Mapping: 256 * 192 pixels (49,152 total pixels)
* Purpose: Direct pixel-mapped framebuffer for graphical rendering.

[1] Reserved / Unassigned Gap
* Address Range: 0x0C00
* Decimal Range: 3072
* Size: 1 byte/word boundary spacer
* Purpose: Alignment boundary separating VRAM and Stack segments.

[2] Stack Memory
* Address Range: 0x0C01 - 0x1C00
* Decimal Range: 3073 - 7168
* Size: 4 KB
* Pointer Register: r14 (SP - Stack Pointer)
* Purpose: Dynamic subroutine return address tracking, local variable storage,
and register preservation via PUSH/POP instructions.

[3] General Program & Data Space (Free Space)
* Address Range: 0x1C01 - 0xFFFE
* Decimal Range: 7169 - 65534
* Size: ~60.25 KB
* Purpose: Application instructions, static variables, and constants.

[4] Keyboard Input Memory-Mapped I/O (MMIO)
* Address Range: 0xFFFF
* Decimal Range: 65535
* Size: 1 word
* Purpose: Hardware port mapped for reading active keyboard input states.

ARCHITECTURAL REGISTER MAP REFERENCE

r0 - r11 : General Purpose Registers (GPR)

r12      : Jump Target Register (JTR) - Target addresses for JMP/Branching ops

r13      : Accumulator (ACC) - Default destination for ALU operations

r14      : Stack Pointer (SP) - Tracks top of Stack Memory segment

r15      : Memory Address Register (MAR) - Pointer for LOAD/STR/CALL operations

================================================================================