// Main.cpp

#include <telkin/Hooks.h>
#include <telkin/Print.h>
#include <dynamic_libs/os_functions.h>
#include "powerup_stuff/question_block_placer.h" // IWYU pragma: keep

// Branches must be put in a global scope, not in functions
tBranch(0x022f94c8, SM3DW::PowerUp::QBlock::powerup_placer, tk::BranchType::b);

void main() {
    InitOSFunctionPointers();
    tk::println("Hello from SM3DAPWorld!");
}