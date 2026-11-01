#include <telkin/Hooks.h>
#include <telkin/Print.h>
#include <dynamic_libs/os_functions.h>
#include "powerup_stuff/question_block_placer.h" // IWYU pragma: keep

void main() {
    InitOSFunctionPointers();
    tk::println("Hello from SM3DAPWorld!");
}