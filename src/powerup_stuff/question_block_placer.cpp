#include <telkin/Hooks.h>
#include <telkin/Print.h>
#include <dynamic_libs/os_functions.h>

// Game functions, called by address.
// Argument types are taken from the decompile; param_1/2/3/4 are 4-byte values.
using GetStringParamFn = int  (*)(const char** out, unsigned int params, const char* key);
using SpawnItemFn      = void (*)(unsigned int a, unsigned int b, unsigned int data,
                                  unsigned int c, unsigned int d, unsigned int zero);

static inline int GetStringParam(const char** out, unsigned int params, const char* key) {
    return reinterpret_cast<GetStringParamFn>(0x0248c4bc)(out, params, key);   // FUN_0248c4bc
}
static inline void SpawnItem(unsigned int a, unsigned int b, unsigned int data,
                             unsigned int c, unsigned int d) {
    reinterpret_cast<SpawnItemFn>(0x024babf0)(a, b, data, c, d, 0);            // FUN_024babf0
}


// String compare
static bool StrEq(const char* a, const char* b) {
    while (*a == *b) {
        if (*a == '\0') return true;
        ++a; ++b;
    }
    return false;
}


// Item table. Flip `unlocked` to false to lock an item from being placed in ?-Blocks.
// name == nullptr means the name string lives in game memory at nameAddr
// (those three names appear in the decompile only as DAT_ addresses).
// `data` is the pointer the game passes to FUN_024babf0 for that item.

struct ItemEntry {
    const char*  name;
    unsigned int nameAddr;
    unsigned int data;
    bool         unlocked;
};

// You can replace the data entry to make it spawn a different item
static ItemEntry sItems[] = {
    { "Coin",                 0x1032afc8, 0x1032b084, true },
    { "Coin10",                0,          0x1032b098, true },
    { "CoinRandom10",          0,          0x1032b0c8, true },
    { "CoinInfinity",          0,          0x1032b098, true },
    { "KinokoOneUp",           0,          0x1032b024, true },
    { "KinokoSuper",           0,          0x1032b0f0, true },
    { "SuperBell",             0,          0x1032b100, true },
    { "FireFlower",            0,          0x1032b11c, true },
    { "SuperLeaf",             0,          0x1032b130, true },
    { "BoomerangFlower",       0,          0x1032b140, true },
    { "SuperStar",             0,          0x1032b154, true },
    { "Ball",                 0x1032afd8, 0x1032afe0, true },    // Those Baseball things
    { nullptr,                 0x1032afe8, 0x1032b164, true },   // Bomb, name not visible from decompiler
    { "CoinBlow",              0,          0x1032b17c, true },
    { "GreenStar",             0,          0x1032b188, true },
    { "DoubleMario",           0,          0x1032b198, true },
    { "KinokoBig",             0,          0x1032b1a8, true },
    { "AssistLeaf",            0,          0x1032b1c0, true },
    { "SuperBellSpecial",      0,          0x1032b1e0, true },
    { "CoinConcentricCircle",  0,          0x1032b208, true },
    { "CoinBlow30",            0,          0x1032b06c, true },
};

static const unsigned int kDefaultData = 0x1032b084;  // what the original used for unknown names
static const unsigned int kLockedData  = 0x1032b084;  // locked items give a Coin instead


// Replacement for the game's FUN_022f94c8, which is hooked at its first
// instruction. It runs when a level object that holds an item is set up
// (not when the player picks the item up).
void powerup_placer(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    // Read the object's "ItemType" setting (e.g. "FireFlower", "Coin10").
    // "Dummy" is the fallback if the parameter is missing, as in the original.
    const char* itemType = "Dummy";
    GetStringParam(&itemType, param_2, "ItemType");

    tk::println("powerup_placer: ItemType=%s", itemType);

    // Start with the game's default item data. This is what the original code
    // used when the item name matched nothing in its list.
    unsigned int data = kDefaultData;

    // Look the item name up in our table (sItems).
    for (const ItemEntry& e : sItems) {
        // Most names are string literals; a few exist only in game memory
        // (name == nullptr), so read the string from its address instead.
        const char* n = e.name ? e.name : reinterpret_cast<const char*>(e.nameAddr);

        if (StrEq(itemType, n)) {
            // Debug: report the result of the lookup
            tk::println("Matched Entry %d, %s", (int)(&e - sItems), e.unlocked ? "unlocked" : "LOCKED -> Coin10");

            // Unlocked: use the item's own data pointer.
            // Locked:   substitute the Coin10 data so the object still spawns
            //           something instead of the real item.
            data = e.unlocked ? e.data : kLockedData;
            break;
        }
    }

    // Hand off to the game's own spawn code, with our data.
    SpawnItem(param_1, param_2, data, param_3, param_4);
}

// Branches must be put in a global scope, not in functions
tBranch(0x022f94c8, powerup_placer, tk::BranchType::b);