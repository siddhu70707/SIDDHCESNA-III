#include <raylib.h>
#include <iostream>
#include <cstdint> 

#include "Includes/alu.h"
#include "Includes/registers.h"
#include "Includes/Memory.h"

using namespace std;

const int emulation_screen_width = 256;
const int emulation_screen_height = 192;

// Emulation Globals
bool global_clock;
ALU global_alu;
Register program_counter;
RegisterFile global_registerfile;
RAM data_memory;
ROM instruction_memory;

int main() {
    const int pixel_scaling_factor = 4;

    InitWindow(emulation_screen_width * pixel_scaling_factor, 
               emulation_screen_height * pixel_scaling_factor,
               "Siddhcesna - III");

    SetTargetFPS(60);

    Image screen_image = GenImageColor(emulation_screen_width, emulation_screen_height, BLACK);
    
    for (int y = 0; y < emulation_screen_height; y++) {
        for (int x = 0; x < emulation_screen_width; x++) {
            ImageDrawPixel(&screen_image, x, y, BLACK); 
        }
    }

    Texture2D screen_texture = LoadTextureFromImage(screen_image);
    
    UnloadImage(screen_image);

    while (!WindowShouldClose()) {
        

        
        BeginDrawing();
            ClearBackground(BLACK); 
            
            DrawTextureEx(screen_texture, Vector2{0, 0}, 0.0f, pixel_scaling_factor, WHITE);
            
        EndDrawing();
    }
    
    // Clean up GPU assets cleanly on exit
    UnloadTexture(screen_texture);
    CloseWindow();
    return 0;
}

/* 
g++ Emulator/main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game && ./game
*/
