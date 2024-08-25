#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include <termios.h>

void reset_term(void);
void set_term_raw_io(void);
char get_chr(void);


#endif // SERIAL_H_INCLUDED
