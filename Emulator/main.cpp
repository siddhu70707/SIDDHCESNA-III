#include <raylib.h>
#include <iostream>
#include <cstdint> 
#include "alu.h"
#include "registers.h"

using namespace std;

const int emulation_screen_width = 256;
const int emulation_screen_height = 192;

typedef struct {
    uint16_t x;
    uint16_t y;
    bool pixel_color; 
} pixel;

// Emulation Globals

bool global_clock;
ALU global_alu;
RegisterFile global_registerfile;


int main() {
    const int pixel_scaling_factor = 4;

    InitWindow(emulation_screen_width * pixel_scaling_factor, 
               emulation_screen_height * pixel_scaling_factor,
               "Siddhcesna - III");

    pixel screen[emulation_screen_height][emulation_screen_width];
    for(int y = 0; y < emulation_screen_height; y++){
        for(int x = 0; x < emulation_screen_width; x++){
            screen[y][x].x = x;
            screen[y][x].y = y;
            screen[y][x].pixel_color = true; 
        }
    }

    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK); 
        for(int y = 0; y < emulation_screen_height; y++){
            for(int x = 0; x < emulation_screen_width; x++){
                
                Color pix_color = screen[y][x].pixel_color ? WHITE : BLACK;
                DrawRectangle(
                    x * pixel_scaling_factor, 
                    y * pixel_scaling_factor, 
                    pixel_scaling_factor, 
                    pixel_scaling_factor, 
                    pix_color
                );
            }
        }

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

/* 
g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game && ./game
*/ 