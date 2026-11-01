#include <telkin/Hooks.h>
#include <telkin/Print.h>
#include <dynamic_libs/os_functions.h>

void main() {
    InitOSFunctionPointers();
    tk::println("Hello from SM3DAPWorld!");
}