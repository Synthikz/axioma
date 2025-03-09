#include <stdint.h>
#include "../include/main.h"

enum SyscallNumber {
    SYS_PRINT = 0,
    SYS_READ_CHAR = 1,
    SYS_EXIT = 2,
    SYS_CLEAR_SCREEN = 3
};

extern "C" uint32_t syscall(uint32_t number, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    uint32_t result;
    asm volatile(
        "int $0x80"
        : "=a" (result)
        : "a" (number), "b" (arg1), "c" (arg2), "d" (arg3)
        : "memory"
    );
    return result;
}

void print(const char* message, uint8_t foreground, uint8_t background) {
    uint32_t color = foreground | (background << 8);
    syscall(SYS_PRINT, (uint32_t)message, color, 0);
}

char wait_for_key() {
    char key = 0;
    while (!(key = (char)syscall(SYS_READ_CHAR, 0, 0, 0))) {
        
    }
    return key;
}

void exit_program() {
    syscall(SYS_EXIT, 0, 0, 0);
}

void clear_screen(uint8_t foreground, uint8_t background) {
    uint32_t color = foreground | (background << 8);
    syscall(SYS_CLEAR_SCREEN, color, 0, 0);
}

extern "C" void _start() {
    clear_screen(YELLOW, BLUE);
    print("=== Sample Axioma Program ===\n", LIGHT_GREEN, BLUE);
    print("This program runs in user mode and uses syscalls.\n", WHITE, BLUE);
    print("\nPress any key to continue...\n", YELLOW, BLUE);
    
    wait_for_key();
    
    print("\nKey pressed! Now I will finish and return to the kernel.\n", LIGHT_CYAN, BLUE);
    print("Goodbye!\n", LIGHT_PURPLE, BLUE);
    
    print("\nPress any key to exit...\n", YELLOW, BLUE);
    wait_for_key();
    
    exit_program();
}
