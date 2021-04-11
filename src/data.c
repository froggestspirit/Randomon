#include "global.h"
#include "malloc.h"
#include "battle.h"
#include "data.h"
#include "graphics.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/trainers.h"
#include "constants/battle_ai.h"

const u16 gMinigameDigits_Pal[] = INCBIN_U16("graphics/link/minigame_digits.gbapal");
const u32 gMinigameDigits_Gfx[] = INCBIN_U32("graphics/link/minigame_digits.4bpp.lz");
static const u32 sMinigameDigitsThin_Gfx[] = INCBIN_U32("graphics/link/minigame_digits2.4bpp.lz"); // Unused

#define LARGE_MON_PIC_SIZE 0xC80
#define BATTLER_OFFSET(i) (gHeap + 0x8000 + LARGE_MON_PIC_SIZE * (i))

const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[] =
{
    BATTLER_OFFSET(0), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(1), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(2), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[] =
{
    BATTLER_OFFSET(4), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(5), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(6), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(7), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_PlayerRight[] =
{
    BATTLER_OFFSET(8),  LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(9),  LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(10), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(11), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gBattlerPicTable_OpponentRight[] =
{
    BATTLER_OFFSET(12), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(13), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(14), LARGE_MON_PIC_SIZE,
    BATTLER_OFFSET(15), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Brendan[] =
{
    gTrainerBackPic_Brendan, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Brendan + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Brendan + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Brendan + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_May[] =
{
    gTrainerBackPic_May, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_May + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_May + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_May + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Red[] =
{
    gTrainerBackPic_Red, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Red + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Red + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Red + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Red + (LARGE_MON_PIC_SIZE * 4), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Leaf[] =
{
    gTrainerBackPic_Leaf, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Leaf + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Leaf + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Leaf + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Leaf + (LARGE_MON_PIC_SIZE * 4), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_RubySapphireBrendan[] =
{
    gTrainerBackPic_RubySapphireBrendan, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireBrendan + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireBrendan + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireBrendan + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_RubySapphireMay[] =
{
    gTrainerBackPic_RubySapphireMay, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireMay + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireMay + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_RubySapphireMay + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Wally[] =
{
    gTrainerBackPic_Wally, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Wally + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Wally + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Wally + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

const struct SpriteFrameImage gTrainerBackPicTable_Steven[] =
{
    gTrainerBackPic_Steven, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Steven + LARGE_MON_PIC_SIZE, LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Steven + (LARGE_MON_PIC_SIZE * 2), LARGE_MON_PIC_SIZE,
    gTrainerBackPic_Steven + (LARGE_MON_PIC_SIZE * 3), LARGE_MON_PIC_SIZE,
};

static const union AnimCmd sAnim_GeneralFrame0[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_GeneralFrame3[] =
{
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF548[] =
{
    AFFINEANIMCMD_FRAME(0x0100, 0x0100, 0x00, 0x00),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF558[] =
{
    AFFINEANIMCMD_FRAME(0xff00, 0x0100, 0x00, 0x00),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF568[] =
{
    AFFINEANIMCMD_FRAME(0x0028, 0x0028, 0x00, 0x00),
    AFFINEANIMCMD_FRAME(0x0012, 0x0012, 0x00, 0x0c),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF580[] =
{
    AFFINEANIMCMD_FRAME(0xfffe, 0xfffe, 0x00, 0x12),
    AFFINEANIMCMD_FRAME(0xfff0, 0xfff0, 0x00, 0x0f),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF598[] =
{
    AFFINEANIMCMD_FRAME(0x00a0, 0x0100, 0x00, 0x00),
    AFFINEANIMCMD_FRAME(0x0004, 0x0000, 0x00, 0x08),
    AFFINEANIMCMD_FRAME(0xfffc, 0x0000, 0x00, 0x08),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd gUnknown_082FF5B8[] =
{
    AFFINEANIMCMD_FRAME(0x0002, 0x0002, 0x00, 0x14),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF5C8[] =
{
    AFFINEANIMCMD_FRAME(0xfffe, 0xfffe, 0x00, 0x14),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF5D8[] =
{
    AFFINEANIMCMD_FRAME(0x0100, 0x0100, 0x00, 0000),
    AFFINEANIMCMD_FRAME(0xfff0, 0xfff0, 0x00, 0x09),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF5F0[] =
{
    AFFINEANIMCMD_FRAME(0x0004, 0x0004, 0x00, 0x3f),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF600[] =
{
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0xfd, 0x05),
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0x03, 0x05),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[] =
{
    gUnknown_082FF548,
    gUnknown_082FF568,
    gUnknown_082FF580,
    gUnknown_082FF598,
    gUnknown_082FF5B8,
    gUnknown_082FF5C8,
    gUnknown_082FF5F0,
    gUnknown_082FF600,
    gUnknown_082FF5D8,
};

static const union AffineAnimCmd gUnknown_082FF63C[] =
{
    AFFINEANIMCMD_FRAME(0xfffc, 0xfffc, 0x04, 0x3f),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF64C[] =
{
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0x03, 0x05),
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0xfd, 0x05),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF664[] =
{
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0xfb, 0x14),
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0x00, 0x14),
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0x05, 0x14),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd gUnknown_082FF684[] =
{
    AFFINEANIMCMD_FRAME(0x0000, 0x0000, 0x09, 0x6e),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[] =
{
    gUnknown_082FF548,
    gUnknown_082FF568,
    gUnknown_082FF580,
    gUnknown_082FF598,
    gUnknown_082FF5B8,
    gUnknown_082FF5C8,
    gUnknown_082FF63C,
    gUnknown_082FF64C,
    gUnknown_082FF664,
    gUnknown_082FF5D8,
    gUnknown_082FF684,
};

const union AffineAnimCmd *const gUnknown_082FF6C0[] =
{
    gUnknown_082FF558,
    gUnknown_082FF568,
    gUnknown_082FF580,
    gUnknown_082FF598,
    gUnknown_082FF5B8,
    gUnknown_082FF5C8,
    gUnknown_082FF63C,
    gUnknown_082FF64C,
    gUnknown_082FF664,
    gUnknown_082FF5D8,
    gUnknown_082FF684,
};

static const union AnimCmd gUnknown_082FF6EC[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd gUnknown_082FF6F4[] =
{
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_END,
};

static const union AnimCmd gUnknown_082FF6FC[] =
{
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_END,
};

static const union AnimCmd gUnknown_082FF704[] =
{
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_082FF70C[] =
{
    gUnknown_082FF6EC,
    gUnknown_082FF6F4,
    gUnknown_082FF6FC,
    gUnknown_082FF704,
};

#define SPECIES_SPRITE(species, sprite) [SPECIES_##species] = {sprite, LARGE_MON_PIC_SIZE, SPECIES_##species}
#define SPECIES_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species}
#define SPECIES_SHINY_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species + SPECIES_SHINY_TAG}

#include "data/pokemon_graphics/unknown_anims.h"
#include "data/pokemon_graphics/front_pic_coordinates.h"
#include "data/pokemon_graphics/still_front_pic_table.h"
#include "data/pokemon_graphics/back_pic_coordinates.h"

#include "data/pokemon_graphics/back_pic_table.h"
#include "data/pokemon_graphics/palette_table.h"
#include "data/pokemon_graphics/shiny_palette_table.h"

#include "data/trainer_graphics/front_pic_anims.h"
#include "data/trainer_graphics/front_pic_tables.h"
#include "data/trainer_graphics/back_pic_anims.h"
#include "data/trainer_graphics/back_pic_tables.h"

#include "data/pokemon_graphics/enemy_mon_elevation.h"
#include "data/pokemon_graphics/front_pic_anims.h"
#include "data/pokemon_graphics/front_pic_table.h"

static const u32 sUnused[] =
{
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000088, 0x00000888, 0x00000888, 0x00000886,
    0x00000888, 0x00000886, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000886, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000886, 0x00000886,
    0x00000888, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000888, 0x00000886, 0x00000888,
    0x00000888, 0x00000888, 0x00000886, 0x00000886,
    0x00000888, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000886, 0x00000886, 0x00000088,
    0x00000886, 0x00000886, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000886, 0x00000888, 0x00000088, 0x00000088,
    0x00000888, 0x00000888, 0x00000888, 0x00000886,
    0x00000888, 0x00000888, 0x00000888, 0x00000886,
    0x00000886, 0x00000886, 0x00000886, 0x00000886,
    0x00000886, 0x00000886, 0x00000888, 0x00000888,
    0x00000886, 0x00000886, 0x00000886, 0x00000886,
    0x00000886, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000886, 0x00000886, 0x00000888,
    0x00000886, 0x00000886, 0x00000888, 0x00000888,
    0x00000088, 0x00000088, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000886, 0x00000886, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000088, 0x00000886,
    0x00000888, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000888, 0x00000886, 0x00000888,
    0x00000088, 0x00000088, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000888, 0x00000886,
    0x00000886, 0x00000888, 0x00000888, 0x00000088,
    0x00000888, 0x00000886, 0x00000886, 0x00000888,
    0x00000886, 0x00000888, 0x00000888, 0x00000886,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000088, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000088, 0x00000888, 0x00000888, 0x00000886,
    0x00000886, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000886,
    0x00000888, 0x00000886, 0x00000088, 0x00000088,
    0x00000088, 0x00000888, 0x00000088, 0x00000888,
    0x00000888, 0x00000088, 0x00000088, 0x00000888,
    0x00000886, 0x00000888, 0x00000886, 0x00000886,
    0x00000886, 0x00000888, 0x00000888, 0x00000888,
    0x00000088, 0x00000888, 0x00000888, 0x00000888,
    0x00000088, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000088, 0x00000088,
    0x00000886, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000088, 0x00000888,
    0x00000886, 0x00000888, 0x00000088, 0x00000088,
    0x00000888, 0x00000888, 0x00000088, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000088,
    0x00000888, 0x00000888, 0x00000088, 0x00000088,
    0x00000088, 0x00000888, 0x00000088, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000886, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000886, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000088, 0x00000886,
    0x00000088, 0x00000886, 0x00000886, 0x00000886,
    0x00000088, 0x00000886, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000886,
    0x00000886, 0x00000886, 0x00000888, 0x00000888,
    0x00000886, 0x00000886, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000886, 0x00000886,
    0x00001882, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000886, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000886, 0x00000088, 0x00000886, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000088, 0x00000886,
    0x00000886, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000886, 0x00000088,
    0x00000088, 0x00000886, 0x00000886, 0x00000886,
    0x00000886, 0x00000886, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000886,
    0x00000886, 0x00000886, 0x00000886, 0x00000088,
    0x00000886, 0x00000088, 0x00000886, 0x00000886,
    0x00000886, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000886, 0x00000886,
    0x00000886, 0x00000888, 0x00000886, 0x00000886,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000886, 0x00000886, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000088, 0x00000088, 0x00000088,
    0x00000088, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
    0x00000888, 0x00000888, 0x00000888, 0x00000888,
};

#include "data/trainer_parties.h"
#include "data/text/trainer_class_names.h"
#include "data/trainers.h"
#include "data/text/species_names.h"
#include "data/text/move_names.h"
