#ifndef ALU_H
#define ALU_H

#include <iostream>
#include <cstdint>
using namespace std;

class ALU {
    public:
        uint16_t src1, src2, output;
        uint8_t opcode;
        bool zero = false;
        bool overflow = false;
        bool greater = false;
        bool smaller = false;

        void update(uint16_t s1, uint16_t s2, uint8_t op) {
            if (op > 31) {
                std::cerr << "\nopcode must be in between 0-31"; 
                return;
            }

            this->src1 = s1;
            this->src2 = s2;
            this->opcode = op;

            switch(op) {

                case 1: { 
                    uint32_t result = (uint32_t)src1 + src2;
                    output = (uint16_t)(result & 0xFFFF);
                    
                    zero = (output == 0);
                    overflow = (result > 0xFFFF);
                    break;
                }

                case 2: { 
                    int32_t result = (int32_t)src1 - src2;
                    output = (uint16_t)(result & 0xFFFF);
                    
                    zero = (output == 0);
                    smaller = (result < 0);
                    break;
                }

                case 3: {
                    uint32_t result = (uint32_t)src1 * src2;
                    output = (uint16_t)(result & 0xFFFF);
                    
                    zero = (output == 0);
                    overflow = (result > 0xFFFF);
                    break;
                }

                case 4: { 
                    if (src2 == 0) {
                        output = 0xFFFF; // huh
                        overflow = true;
                    } else {
                        output = src1 / src2;
                        if(output == 0) zero = true;
                        overflow = false;
                    }
                    break;
                }

                case 5: 
                    output = src1 & src2;
                    if(output == 0) zero = true;
                    break;

                case 6: 
                    output = src1 | src2;
                    if(output == 0) zero = true;
                    break;

                case 7: 
                    output = src1 ^ src2;
                    if(output == 0) zero = true;
                    break;

                case 8: 
                    output = ~src1;
                    if(output == 0) zero = true;
                    break;

                case 9: 
                    output = src1 << src2;
                    if(output == 0) zero = true;
                    break;

                case 10: 
                    output = src1 >> src2;
                    if(output == 0) zero = true;
                    break;

                case 11: 
                    output = 0; 
                    if(src1 == src2) zero = true;
                    if(src1 > src2) greater = true;
                    if(src1 < src2) smaller = true;
                    break;

                default:
                    cerr << op << "\n is not an ALU operation";
                    break;
            }
        }
};

#endif