use core::arch::asm;

const COM1: u16 = 0x3F8;
const DATA: u16 = COM1;
const INTERRUPT_ENABLE: u16 = COM1 + 1;
const FIFO_CONTROL: u16 = COM1 + 2;
const LINE_CONTROL: u16 = COM1 + 3;
const MODEM_CONTROL: u16 = COM1 + 4;
const LINE_STATUS: u16 = COM1 + 5;

#[inline]
unsafe fn outb(port: u16, value: u8) {
    asm!("out dx, al", in("dx") port, in("al") value, options(nomem, nostack, preserves_flags));
}

#[inline]
unsafe fn inb(port: u16) -> u8 {
    let value: u8;
    asm!("in al, dx", in("dx") port, out("al") value, options(nomem, nostack, preserves_flags));
    value
}

/// Initialize the first 16550-compatible UART (COM1) for 115200 8N1.
pub fn init() {
    unsafe {
        outb(INTERRUPT_ENABLE, 0x00);
        outb(LINE_CONTROL, 0x80);
        outb(DATA, 0x01);
        outb(INTERRUPT_ENABLE, 0x00);
        outb(LINE_CONTROL, 0x03);
        outb(FIFO_CONTROL, 0xC7);
        outb(MODEM_CONTROL, 0x0B);
    }
}

/// Write one byte to COM1, waiting until the transmitter is ready.
pub fn write_byte(byte: u8) {
    unsafe {
        while (inb(LINE_STATUS) & 0x20) == 0 {
            core::hint::spin_loop();
        }
        outb(DATA, byte);
    }
}

pub fn write_str(value: &str) {
    for byte in value.bytes() {
        if byte == b'\n' {
            write_byte(b'\r');
        }
        write_byte(byte);
    }
}
