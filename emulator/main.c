#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DEVMODE         // uncomment this switch to enable debug ROM load and run
#ifdef DEVMODE
#include "debug_rom.h"
#endif

extern struct termios orig_term;

int main() {
    printf("TCPU816 - ALPHA 1 emulator.\n");
    CPU cpu;
    cpu_reset(&cpu);


    printf("# -- DEVMODE ENABLED, RUNNING DEFAULT ROM --\n");
    memcpy(cpu.ram, debug_rom, debug_rom_len);
    cpu.cycle_limit = 100000;

    set_term_raw_io();
    cpu_run(&cpu);
    return 0;
}
