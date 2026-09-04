#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
#include <cstdint>

class Register {
public:
    uint16_t value = 0;
    bool we = false; 
    bool re = false; 

    void update_value(uint16_t val, bool clk) {
        if (clk && we) {
            this->value = val;
        }
    }

    void update_re(bool r, bool clk) {
        if (clk) {
            this->re = r; 
        }
    }

    void update_we(bool w, bool clk) { 
        if (clk) {
            this->we = w; 
        }
    }
};

class RegisterFile {
private:
    Register regs[16];

public:
    
    // General Purpose Register
    Register& r0  = regs[0];
    Register& r1  = regs[1];
    Register& r2  = regs[2];
    Register& r3  = regs[3];
    Register& r4  = regs[4];
    Register& r5  = regs[5];
    Register& r6  = regs[6];
    Register& r7  = regs[7];
    Register& r8  = regs[8];
    Register& r9  = regs[9];
    Register& r10 = regs[10];
    Register& r11 = regs[11];
    
    Register& r12 = regs[12]; // JTR (Jump Target Register)
    Register& r13 = regs[13]; // ACC (Accumulator)
    Register& r14 = regs[14]; // SP  (Stack Pointer)
    Register& r15 = regs[15]; // MAR (Memory Address Register)

    uint16_t read(uint8_t reg_id) {
        if (reg_id < 16) {
            return regs[reg_id].value;
        }
        std::cerr << "\nError: Invalid register ID " << (int)reg_id;
        return 0;
    }

    void write(uint8_t reg_id, uint16_t val, bool clk) {
        if (reg_id < 16) {
            regs[reg_id].update_value(val, clk);
        } else {
            std::cerr << "\nError: Invalid register ID " << (int)reg_id;
        }
    }
};

#endif
