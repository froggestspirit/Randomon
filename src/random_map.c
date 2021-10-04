#include "global.h"
#include "fieldmap.h"
#include "overworld.h"
#include "random_map.h"

u32 gCurrentSeed;
u8 mapWidth, mapHeight;
bool8 badMap;
u8 gRandomMapData[MAX_DIM * (MAX_DIM >> 1)];
struct Point{
    s8 x;
    s8 y;
};

extern struct BackupMapLayout gBackupMapLayout;
//EWRAM_DATA static u8 gRandomMapData[MAX_MAP_DATA_SIZE >> 5] = {0};

struct ConnectionFlags
{
    u8 south:1;
    u8 north:1;
    u8 west:1;
    u8 east:1;
};

const u16 randMapBlock[16 * NUM_BLOCKS] = {
EMPTY_TILE, EMPTY_TILE, EMPTY_TILE, EMPTY_TILE,
EMPTY_TILE, EMPTY_TILE, EMPTY_TILE, EMPTY_TILE,
EMPTY_TILE, EMPTY_TILE, EMPTY_TILE, EMPTY_TILE,
EMPTY_TILE, EMPTY_TILE, EMPTY_TILE, EMPTY_TILE,

BORDER_TILE, BORDER_TILE, BORDER_TILE, BORDER_TILE,
BORDER_TILE, BORDER_TILE, BORDER_TILE, BORDER_TILE,
BORDER_TILE, BORDER_TILE, BORDER_TILE, BORDER_TILE,
BORDER_TILE, BORDER_TILE, BORDER_TILE, BORDER_TILE,

PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,

PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,

PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,

PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE,
PATH_TILE, PATH_TILE, PATH_TILE, PATH_TILE
};


u16 map_rand(int a, int b){  // Psuedo Random number generator, using the same formula as the Gen 3/4 games
    gCurrentSeed = ((0x41C64E6D * gCurrentSeed) + 0x00006073) & 0xFFFFFFFF;
    if(a > b)
        return ((gCurrentSeed >> 16) % ((a - b) + 1)) + b;
    return ((gCurrentSeed >> 16) % ((b - a) + 1)) + a;
}

bool8 check_rect(u8 tile, u8 x, u8 y, u8 w, u8 h){  // Check if a rectangle is free of tiles that are not tile, or 0
    u8 xi, yi;
    u8 xCoord, yCoord;
    if(x + w < x){
        x = x + w;
        w = abs(w);
    }
    if(y + h < y){
        y = y + h;
        h = abs(h);
    }
    for(yi = 0; yi < h; yi++){
        yCoord = y + yi;
        u16 yOff = yCoord * mapWidth;
        for(xi = 0; xi < w; xi++){
            xCoord = x + xi;
            if(gRandomMapData[yOff + xCoord] != tile){
                if(gRandomMapData[yOff + xCoord]){
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void fill_rect(u8 tile, u8 x, u8 y, u8 w, u8 h){  // Fill a rectangle with tile
    u8 xi, yi;
    u8 xCoord, yCoord;
    if(x + w < x){
        x = x + w;
        w = abs(w);
    }
    if(y + h < y){
        y = y + h;
        h = abs(h);
    }
    for(yi = 0; yi < h; yi++){
        yCoord = y + yi;
        u16 yOff = yCoord * mapWidth;
        for(xi = 0; xi < w; xi++){
            xCoord = x + xi;
            if(!gRandomMapData[yOff + xCoord])
                gRandomMapData[yOff + xCoord] = tile;
        }
    }
}

bool8 draw_line(u8 tile, u8 x1, u8 y1, u8 x2, u8 y2){  // Draw a line of tiles (this expects either a horizantal/vertical line, not diagonal)
    u8 count = 0;
    s8 inc[2];
    if(x1 < x2)
        inc[0] = 1;
    if(y1 < y2)
        inc[1] = 1;
    if(x1 > x2)
        inc[0] = -1;
    if(y1 > y2)
        inc[1] = -1;
    while(x1 != x2 || y1 != y2){
        if(gRandomMapData[(y1 * mapWidth) + x1] == tile)
            return TRUE;
        if(gRandomMapData[(y1 * mapWidth) + x1] >= 4){
            badMap = TRUE;
            return TRUE;
        }
        gRandomMapData[(y1 * mapWidth) + x1] = tile;
        if(count){
            if(gRandomMapData[((y1 + inc[0]) * mapWidth) + (x1 + inc[1])] == tile)
                return TRUE;
            if(gRandomMapData[((y1 - inc[0]) * mapWidth) + (x1 - inc[1])] == tile)
                return TRUE;
        }
        x1 += inc[0];
        y1 += inc[1];
        count++;
    }
    if(gRandomMapData[(y1 * mapWidth) + x1] == tile)
        return TRUE;
    return FALSE;
}

void process_path(u8 dir, struct Point *point, struct Point *destPoint){  // Handle drawing the paths
    s8 len;
    u8 xy[2];
    u8 xy2[2];
    while(point->x != destPoint->x || point->y != destPoint->y){
        if(!dir){  // x
            if(abs(destPoint->x - point->x) > PATH_LINEARITY)
                len = map_rand(PATH_LINEARITY, abs(destPoint->x - point->x));
            else
                len = abs(destPoint->x - point->x);
            if(len){
                xy[0] = xy2[0] = point->x;
                xy[1] = xy2[1] = point->y;
                if(destPoint->x < point->x)
                    len = -len;
                xy2[0] += len;
                point->x += len;
                if(draw_line(2, xy[0], xy[1], xy2[0], xy2[1]))
                    point = destPoint;
            }
        }else{  // y
            if(abs(destPoint->y - point->y) > PATH_LINEARITY)
                len = map_rand(PATH_LINEARITY, abs(destPoint->y - point->y));
            else
                len = abs(destPoint->y - point->y);
            if(len){
                xy[0] = xy2[0] = point->x;
                xy[1] = xy2[1] = point->y;
                if(destPoint->y < point->y)
                    len = -len;
                xy2[1] += len;
                point->y += len;
                if(draw_line(2, xy[0], xy[1], xy2[0], xy2[1]))
                    point = destPoint;
            }
        }
        dir ^= 1;
    }
}

u32 RandomMap(u16 seed, u8 mapType, bool8 newGame){
    int i;
    u8 x, y;
    u16 size;
    u32 retSeed;
    u8 exitPos[4];
    u8 exitSize[4];
    u8 mapDim[4];
    gCurrentSeed = seed;
    badMap = TRUE;
    while(badMap){
        retSeed = gCurrentSeed;
        badMap = FALSE;
        // Maps are built in blocks first that represent 4x4 tiles
        // Map size
        mapWidth = map_rand(8,30);
        mapHeight = map_rand(8,30);
        size = ((mapWidth * 4) + 15) * ((mapHeight * 4) + 14);
        // Enforce maximum size (In-game RAM limit)
        while(size >= MAX_MAP_DATA_SIZE){
            if(mapHeight >= mapWidth)
                mapHeight--;
            else
                mapWidth--;
            size = ((mapWidth * 4) + 15) * ((mapHeight * 4) + 14);
        }
        mapDim[0] = mapDim[2] = mapWidth;
        mapDim[1] = mapDim[3] = mapHeight;
        for(i = 0; i < 4; i++){
            if(mapType & (1 << i)){
                exitPos[i] = map_rand(MARGIN_SIZE, (mapDim[i] - 1) - MARGIN_SIZE);
                exitSize[i] = map_rand(1, 4);
                if(exitPos[i] + exitSize[i] >= mapDim[i] - MARGIN_SIZE)
                    exitPos[i] -= (exitPos[i] + exitSize[i]) - (mapDim[i] - MARGIN_SIZE);
            }
        }

        // Create a bare map template
        for(i = 0; i < MAX_MAP_DATA_SIZE >> 6; i++)
            gRandomMapData[i] = 0;

        for(y = 0; y < mapHeight; y++){
            u16 yOff = y * mapWidth;
            for(x = 0; x < mapWidth; x++){
                // Get location type corners will be borders, margins may vary, center has no border
                u8 locType = 0;
                if(y >= MARGIN_SIZE)
                    locType += 3;
                if(y >= (mapHeight - MARGIN_SIZE))
                    locType += 3;
                if(x >= MARGIN_SIZE)
                    locType += 1;
                if(x >= (mapWidth - MARGIN_SIZE))
                    locType += 1;

                switch(locType){
                    case 0:
                    case 2:
                    case 6:
                    case 8:  // corners
                        gRandomMapData[yOff + x] = 1;
                        break;
                    case 4:  // center
                        gRandomMapData[yOff + x] = 0;
                        break;
                    case 1:  // north
                        if((x >= exitPos[0]) && (x < exitPos[0] + exitSize[0]))
                            gRandomMapData[yOff + x] = 0;
                        else
                            gRandomMapData[yOff + x] = 1;
                        break;
                    case 3:  // west
                        if((y >= exitPos[1]) && (y < exitPos[1] + exitSize[1]))
                            gRandomMapData[yOff + x] = 0;
                        else
                            gRandomMapData[yOff + x] = 1;
                        break;
                    case 5:  // east
                        if((y >= exitPos[3]) && (y < exitPos[3] + exitSize[3]))
                            gRandomMapData[yOff + x] = 0;
                        else
                            gRandomMapData[yOff + x] = 1;
                        break;
                    case 7:  // south
                        if((x >= exitPos[2]) && (x < exitPos[2] + exitSize[2]))
                            gRandomMapData[yOff + x] = 0;
                        else
                            gRandomMapData[yOff + x] = 1;
                        break;
                }
            }
        }
        // Set focal points for the path
        // Create one near each exit, and one near the center
        struct Point *pathPointCenter;
        struct Point *xyAvg;
        struct Point *pathPoint[10]; 
        u8 firstDir[10];
        u8 points = 0;
        pathPointCenter->x = mapWidth >> 1;
        pathPointCenter->y = mapHeight >> 1;
        xyAvg->x = 0;
        xyAvg->y = 0;

        if(exitPos[0] > 0){
            pathPoint[points]->x = exitPos[0] + (exitSize[0] >> 1);
            pathPoint[points]->y = MARGIN_SIZE;
            firstDir[points] = 1;
            xyAvg->x -= pathPoint[points]->x;
            xyAvg->y -= pathPoint[points]->y;
            points++;
        }
        if(exitPos[1] > 0){
            pathPoint[points]->x = MARGIN_SIZE;
            pathPoint[points]->y = exitPos[1] + (exitSize[1] >> 1);
            firstDir[points] = 0;
            xyAvg->x -= pathPoint[points]->x;
            xyAvg->y -= pathPoint[points]->y;
            points++;
        }
        if(exitPos[2] > 0){
            pathPoint[points]->x = exitPos[2] + (exitSize[2] >> 1);
            pathPoint[points]->y = mapHeight - (MARGIN_SIZE + 1);
            firstDir[points] = 1;
            xyAvg->x -= pathPoint[points]->x;
            xyAvg->y -= pathPoint[points]->y;
            points++;
        }
        if(exitPos[3] > 0){
            pathPoint[points]->x = mapWidth - (MARGIN_SIZE + 1);
            pathPoint[points]->y = exitPos[3] + (exitSize[3] >> 1);
            firstDir[points] = 0;
            xyAvg->x -= pathPoint[points]->x;
            xyAvg->y -= pathPoint[points]->y;
            points++;
        }

        // Adjust the center point so there's less blank space
        if(points){
            xyAvg->x /= points;
            xyAvg->y /= points;
            xyAvg->x--;
            xyAvg->y--;
            if(xyAvg->x < MARGIN_SIZE)
                xyAvg->x += mapWidth;
            if(xyAvg->y < MARGIN_SIZE)
                xyAvg->y += mapHeight;
            pathPointCenter = xyAvg;
        }

        // Create buildings or entities that should spawn a path point
        #define NUM_ENTITY 2
        u8 entity[NUM_ENTITY];
        entity[0] = 4;  // Pokemon Center
        entity[1] = 5;  // Mart
        struct Point *entityPos[NUM_ENTITY];

        // Draw the path with the path points
        /*for(i = 0; i < points; i++)
            process_path(firstDir[i], pathPoint[i], pathPointCenter);
        gRandomMapData[(pathPointCenter->y * mapWidth) + pathPointCenter->x] = 2;

        // Find space for the entities
        for(i = 0; i < NUM_ENTITY; i++){
            entityPos[i]->x = entityPos[i]->y = 0;
            s8 xy[2];
            xy[0] = xy[1] = 0;
            u8 rerolls = 0;
            while(entityPos[i]->x == 0 && entityPos[i]->y == 0){
                if(rerolls > MAX_REROLLS){ // Too many re-rolls, abort
                    entityPos[i]->x = entityPos[i]->y = 1;
                    badMap = TRUE;
                }else{
                    rerolls++;
                    xy[0] = map_rand(MARGIN_SIZE, (mapWidth - 1) - MARGIN_SIZE);
                    xy[1] = map_rand(MARGIN_SIZE, (mapHeight - 2) - MARGIN_SIZE);
                    if(xy[0] == pathPointCenter->x)
                        xy[0]++;
                    if(xy[1] == pathPointCenter->y)
                        xy[1]++;
                    if(check_rect(0, xy[0], xy[1] - 1, 1, 2)){ // Make sure the area the entity is placed is clear, and the tile above it
                        if(check_rect(2, xy[0], xy[1] + 1, 1, 1)){ // Do a separate check for the tile below the entity, alowing paths, since they are good to lead up to buildings
                            entityPos[i]->x = xy[0];
                            entityPos[i]->y = xy[1];
                            gRandomMapData[(xy[1] * mapWidth) + xy[0]] = entity[i];
                        }
                    }
                }
            }
        }
        // Create paths for the entities
        for(i = 0; i < NUM_ENTITY; i++){
            entityPos[i]->y++;
            process_path(0, entityPos[i], pathPointCenter);
        }*/
    }

    //gBackupMapLayout.width = (mapWidth << 2) + 15;
    //gBackupMapLayout.height = (mapHeight << 2) + 14;

    // Copy over actual tile data
    /*u16 *tilePointer[16];
    for(i = 0; i < 16; i++){
        tilePointer[i] = gBackupMapLayout.map;
        tilePointer[i] += gBackupMapLayout.width * 7 + 7;
        tilePointer[i] += (i >> 2) * gBackupMapLayout.width;
        tilePointer[i] += (i & 3);
    }

    for(y = 0; y < mapHeight; y++){
        u16 yOff = y * mapWidth;
        for(x = 0; x < mapWidth; x++){
            for(i = 0; i < 16; i++){
                *tilePointer[i] = randMapBlock[(gRandomMapData[yOff + x] << 4) + i];
                tilePointer[i] += 4;
            }
        }
        for(i = 0; i < 16; i++)
            tilePointer[i] += (3 * gBackupMapLayout.width) + 15;
    }*/
    return retSeed;
}
