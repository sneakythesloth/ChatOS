#ifndef CHATOS_SERIAL_H
#define CHATOS_SERIAL_H

void serial_init(void);
void serial_write_char(char character);
void serial_write(const char *text);

#endif
