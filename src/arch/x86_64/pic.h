#ifndef CHATOS_X86_64_PIC_H
#define CHATOS_X86_64_PIC_H

#include <stdint.h>

void pic_initialize(void);
void pic_enable_irq(uint8_t irq);
void pic_send_eoi(uint8_t irq);

#endif
