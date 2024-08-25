#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>
#include <stdio.h>
#include "serial.h"

struct termios orig_term;

void reset_term(void) {
    tcsetattr(0, TCSANOW, &orig_term);
}

int stdin_ready() {
	struct pollfd fds;
	fds.fd = 0;                     // 0 = stdin
	fds.events = POLLIN;
	return poll(&fds, 1, 0) == 1;
}

void set_term_raw_io(void) {
    struct termios raw_term;
    tcgetattr(0, &orig_term);
    memcpy(&raw_term, &orig_term, sizeof(orig_term));
    cfmakeraw(&raw_term);
    atexit(reset_term);
    tcsetattr(0, TCSANOW, &raw_term);
}

char get_chr(void) {
    unsigned char c;
    if(stdin_ready()) {
        read(0, &c, 1);
        if (c == 0x18) { // CTRL+X
            printf("\r\n");
            exit(0);
        }
        return c;
    }
    return 0;
}
