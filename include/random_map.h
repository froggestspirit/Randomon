#ifndef GUARD_RANDOM_MAP_H
#define GUARD_RANDOM_MAP_H


#include "main.h"

#define BORDER_TILE 0x3023
#define EMPTY_TILE 0x3001
#define PATH_TILE 0x305E

#define MAX_DIM 30
#define RAND_GEN_SIZE (MAX_DIM * (MAX_DIM >> 1))
#define MARGIN_SIZE 2
#define PATH_LINEARITY 4 // Paths will move a minimum of this number of tiles before turning (unless it's destination is closer)
#define MAX_REROLLS 10
#define NUM_BLOCKS 6

u16 map_rand(int, int);
bool8 check_rect(u8, u8, u8, u8, u8);
void fill_rect(u8, u8, u8, u8, u8);
bool8 draw_line(u8, u8, u8, u8, u8);
u32 RandomMap(u16, u8, bool8);


#endif //GUARD_RANDOM_MAP_H