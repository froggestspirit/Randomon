#include "global.h"
#include "battle_pyramid.h"
#include "bg.h"
#include "day_night.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "fldeff_misc.h"
#include "frontier_util.h"
#include "menu.h"
#include "mirage_tower.h"
#include "overworld.h"
#include "palette.h"
#include "pokenav.h"
#include "random_map.h"
#include "script.h"
#include "secret_base.h"
#include "trainer_hill.h"
#include "tv.h"
#include "wild_encounter.h"
#include "constants/rgb.h"
#include "constants/metatile_behaviors.h"

u16 currentSeed;

extern struct BackupMapLayout gBackupMapLayout;
EWRAM_DATA static u16 gRandomMapData[MAX_MAP_DATA_SIZE >> 8] = {0};

struct ConnectionFlags
{
    u8 south:1;
    u8 north:1;
    u8 west:1;
    u8 east:1;
};

u16 rand(int a, int b){  // Psuedo Random number generator, using the same formula as the Gen 3/4 games
    currentSeed = ((0x41C64E6D * currentSeed) + 0x00006073) & 0xFFFFFFFF;
    if(a > b)
        return ((currentSeed >> 16) % ((a - b) + 1)) + b;
    return ((currentSeed >> 16) % ((b - a) + 1)) + a;
}

/*bool8 check_rect(u8 tile, u8 x, u8 y, u8 w, u8 h):  # Check if a rectangle is free of tiles that are not tile, or 0
    if x + w < x:
        x = x + w
        w = abs(w)
    if y + h < y:
        y = y + h
        h = abs(h)
    for yi in range(h):
        yCoord = y + yi
        for xi in range(w):
            xCoord = x + xi
            if(mapData[(yCoord * width) + xCoord] != tile):
                if(mapData[(yCoord * width) + xCoord]):
                    return False
    return True


void fill_rect(u8 tile, u8 x, u8 y, u8 w, u8 h):  # Fill a rectangle with tile
    if x + w < x:
        x = x + w
        w = abs(w)
    if y + h < y:
        y = y + h
        h = abs(h)
    for yi in range(h):
        yCoord = y + yi
        for xi in range(w):
            xCoord = x + xi
            if(not mapData[(yCoord * width) + xCoord]):
                mapData[(yCoord * width) + xCoord] = tile


bool8 draw_line(u8 tile, u8 x1, u8 y1, u8 x2, u8 y2):  # Draw a line of tiles (this expects either a horizantal/vertical line, not diagonal)
    count = 0
    inc = [1,1]
    if x1 == x2:
        inc[0] = 0
    if y1 == y2:
        inc[1] = 0
    if x1 > x2:
        inc[0] = -1
    if y1 > y2:
        inc[1] = -1
    while (x1 != x2) or (y1 != y2):
        dprint("Draw path")
        if(mapData[(y1 * width) + x1] == tile):
            return True
        if(mapData[(y1 * width) + x1] >= 4):
            badMap = True
            return True
        mapData[(y1 * width) + x1] = tile
        if(count):
            if(mapData[((y1 + inc[0]) * width) + (x1 + inc[1])] == tile):
                return True
            if(mapData[((y1 - inc[0]) * width) + (x1 - inc[1])] == tile):
                return True
        x1 += inc[0]
        y1 += inc[1]
        count += 1
    if(mapData[(y1 * width) + x1] == tile):
        return True
    return False


void process_path(u8 dir, u8 point, u8 destPoint):  # Handle drawing the paths
    PATH_LINEARITY = 4 # Paths will move a minimum of this number of tiles before turning (unless it's destination is closer)
    while (point[0] != destPoint[0]) or (point[1] != destPoint[1]):
        dprint("Process path")
        if(abs(destPoint[dir] - point[dir]) > PATH_LINEARITY):
            len = rand(PATH_LINEARITY, abs(destPoint[dir] - point[dir]))
        else:
            len = abs(destPoint[dir] - point[dir])
        if(len == 0): # If len is 0, that means the path is aligned on one axis, so switch to the opposite axis and finish the path
            dir ^= 1
            len = abs(destPoint[dir] - point[dir])
        xy = [point[0], point[1]]
        xy2 = [point[0], point[1]]
        if(destPoint[dir] < point[dir]):
            len = -len
        xy2[dir] += len 
        point[dir] += len
        if(draw_line(2, xy[0], xy[1], xy2[0], xy2[1])):
            point = destPoint
        dir ^= 1*/

void RandomMap(u16 seed, u8 mapType, bool8 newGame){
    int i;
    u8 x, y;
    u16 width, height, size;
    u8 exitPos[4];
    u8 exitSize[4];
    u8 mapDim[4];

    currentSeed = seed;
    // Maps are built in blocks first that represent 4x4 tiles
    // Map size
    width = rand(8,30);
    height = rand(8,30);
    size = ((width * 4) + 15) * ((height * 4) + 14);
    // Enforce maximum size (In-game RAM limit)
    while(size >= MAX_MAP_DATA_SIZE){
        if(height >= width)
            height--;
        else
            width--;
        size = ((width * 4) + 15) * ((height * 4) + 14);
    }
    mapDim[0] = mapDim[2] = width;
    mapDim[1] = mapDim[3] = height;
    for(i = 0; i < 4; i++){
        if(mapType & (1 << i)){
            exitPos[i] = rand(MARGIN_SIZE, (mapDim[i] - 1) - MARGIN_SIZE);
            exitSize[i] = rand(1, 4);
            if(exitPos[i] + exitSize[i] >= mapDim[i] - MARGIN_SIZE)
                exitPos[i] -= (exitPos[i] + exitSize[i]) - (mapDim[i] - MARGIN_SIZE);
        }
    }

    // Create a bare map template
    for(y = 0; y < height; y++){
        u16 yOff = y * width;
        for(x = 0; x < width; x++){
            // Get location type corners will be borders, margins may vary, center has no border
            u8 locType = 0;
            if(y >= MARGIN_SIZE)
                locType += 3;
            if(y >= (height - MARGIN_SIZE))
                locType += 3;
            if(x >= MARGIN_SIZE)
                locType += 1;
            if(x >= (width - MARGIN_SIZE))
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
                case 1:
                    if((x >= exitPos[0]) && (x < exitPos[0] + exitSize[0]))
                        gRandomMapData[yOff + x] = 0;
                    else
                        gRandomMapData[yOff + x] = 1;
                    break;
                case 3:
                    if((y >= exitPos[1]) && (y < exitPos[1] + exitSize[1]))
                        gRandomMapData[yOff + x] = 0;
                    else
                        gRandomMapData[yOff + x] = 1;
                    break;
                case 5:
                    if((y >= exitPos[3]) && (y < exitPos[3] + exitSize[3]))
                        gRandomMapData[yOff + x] = 0;
                    else
                        gRandomMapData[yOff + x] = 1;
                    break;
                case 7:
                    if((x >= exitPos[2]) && (x < exitPos[2] + exitSize[2]))
                        gRandomMapData[yOff + x] = 0;
                    else
                        gRandomMapData[yOff + x] = 1;
                    break;
            }
        }
    }
    /*# Set focal points for the path
    # Create one near each exit, and one near the center
    pathPointCenter = [int(width / 2), int(height / 2)]
    pathPoint = []
    firstDir = []
    xyAvg = [0, 0]
    points = 0

    if exitPos[0] > 0:
        pathPoint.append([exitPos[0] + int(exitSize[0] / 2), MARGIN_SIZE])
        firstDir.append(1)
        xyAvg[0] -= pathPoint[points][0]
        xyAvg[1] -= pathPoint[points][1]
        points += 1
    if exitPos[1] > 0:
        pathPoint.append([MARGIN_SIZE, exitPos[1] + int(exitSize[1] / 2)])
        firstDir.append(0)
        xyAvg[0] -= pathPoint[points][0]
        xyAvg[1] -= pathPoint[points][1]
        points += 1
    if exitPos[2] > 0:
        pathPoint.append([exitPos[2] + int(exitSize[2] / 2), height - (MARGIN_SIZE + 1)])
        firstDir.append(1)
        xyAvg[0] -= pathPoint[points][0]
        xyAvg[1] -= pathPoint[points][1]
        points += 1
    if exitPos[3] > 0:
        pathPoint.append([width - (MARGIN_SIZE + 1), exitPos[3] + int(exitSize[3] / 2)])
        firstDir.append(0)
        xyAvg[0] -= pathPoint[points][0]
        xyAvg[1] -= pathPoint[points][1]
        points += 1

    # Adjust the center point so there's less blank space
    if(points):
        xyAvg[0] = int(xyAvg[0] / points) - 1
        xyAvg[1] = int(xyAvg[1] / points) - 1
        if(xyAvg[0] < MARGIN_SIZE):
            xyAvg[0] += width
        if(xyAvg[1] < MARGIN_SIZE):
            xyAvg[1] += height
        pathPointCenter = [xyAvg[0], xyAvg[1]]

    # Create buildings or entities that should spawn a path point
    entity = [4, 5] # Example Pokemon Center, then Mart
    entityPos = []

    # Draw the path with the path points
    for i, point in enumerate(pathPoint):
        process_path(firstDir[i], point, pathPointCenter)
    mapData[(pathPointCenter[1] * width) + pathPointCenter[0]] = 2

    # Find space for the entities
    for i, obj in enumerate(entity):
        entityPos.append([0,0])
        xy = [0,0]
        rerolls = 0
        while entityPos[i] == [0,0]:
            if(rerolls > MAX_REROLLS): # Too many re-rolls, abort
                entityPos[i] = [1,1]
                badMap = True
            else:
                rerolls += 1
                xy[0] = rand(MARGIN_SIZE, (width - 1) - MARGIN_SIZE)
                xy[1] = rand(MARGIN_SIZE, (height - 2) - MARGIN_SIZE)
                dprint(f"Re-roll entity pos:{seed} ({MARGIN_SIZE}, {(width - 1) - MARGIN_SIZE}) x ({MARGIN_SIZE}, {(height - 2) - MARGIN_SIZE}) : {xy}")
                if(xy[0] == pathPointCenter[0]):
                    xy[0] += 1
                if(xy[1] == pathPointCenter[1]):
                    xy[1] += 1
                if(check_rect(0, xy[0], xy[1] - 1, 1, 2)): # Make sure the area the entity is placed is clear, and the tile above it
                    if(check_rect(2, xy[0], xy[1] + 1, 1, 1)): # Do a separate check for the tile below the entity, alowing paths, since they are good to lead up to buildings
                        entityPos[i] = [xy[0], xy[1]]
                        mapData[(xy[1] * width) + xy[0]] = obj
    
    # Create paths for the entities
    for i, point in enumerate(entityPos):
        point[1] += 1
        process_path(0, point, pathPointCenter)*/



}
