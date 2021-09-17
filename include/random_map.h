#ifndef GUARD_RANDOM_MAP_H
#define GUARD_RANDOM_MAP_H


#include "main.h"

#define EMPTY_TILE 0x01010101
#define MAX_DIM 30
#define MARGIN_SIZE 2

u16 rand(int, int);
bool8 check_rect(u8, u8, u8, u8, u8);
void fill_rect(u8, u8, u8, u8, u8);
bool8 draw_line(u8, u8, u8, u8, u8);
void process_path(u8, u8, u8);
void RandomMap(u16, u8, bool8);


#endif //GUARD_RANDOM_MAP_H