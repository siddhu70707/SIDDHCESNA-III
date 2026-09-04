#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <iostream>
using namespace std;

class RAM {
private:
    uint16_t data[65536];
public:
    bool we = false;
    bool re = false;

    void update_value(uint16_t val, uint16_t addr, bool clk) {
        if (clk && we) {
            data[addr] = val;
        }
    }

    uint16_t read_value(uint16_t addr) const {
        if (re) {
            return data[addr];
        }
        cerr << "\nRead enable must be enabled";
        return 0;
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

class ROM {
private:
    uint16_t data[65536];
public:
    bool re = false;

    uint16_t read_value(uint16_t addr) const {
        if (re) {
            return data[addr];
        }
        return 0;
    }

    void update_re(bool r, bool clk) {
        if (clk) {
            this->re = r; 
        }
    }

    void reprogram(const uint16_t new_data[65536]) {
        for (int i = 0; i < 65536; ++i) {
            data[i] = new_data[i];
        }
    }
};

#endif