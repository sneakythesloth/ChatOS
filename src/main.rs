#![no_std]
#![no_main]

mod arch;
mod console;
mod panic;

#[used]
#[link_section = ".limine_requests_start"]
static LIMINE_REQUESTS_START: [u64; 4] = [
    0xf6b8_f4b3_9de7_d1ae,
    0xfab9_1a69_40fc_b9cf,
    0x785c_6ed0_15d3_e316,
    0x181e_920a_7852_b9d9,
];

#[used]
#[link_section = ".limine_requests"]
static LIMINE_BASE_REVISION: [u64; 3] = [
    0xf956_2b2d_5c95_a6c8,
    3,
    0,
];

#[used]
#[link_section = ".limine_requests_end"]
static LIMINE_REQUESTS_END: [u64; 2] = [
    0xadc0_e053_1bb1_0d03,
    0x9572_709f_3176_4c62,
];

#[macro_export]
macro_rules! kprint {
    ($($arg:tt)*) => {
        $crate::console::print(format_args!($($arg)*))
    };
}

#[macro_export]
macro_rules! kprintln {
    () => {
        $crate::console::print(format_args!("\r\n"))
    };
    ($fmt:expr $(, $arg:expr)* $(,)?) => {
        $crate::console::print(format_args!(concat!($fmt, "\r\n") $(, $arg)*))
    };
}

#[no_mangle]
#[link_section = ".text.entry"]
pub extern "C" fn _start() -> ! {
    arch::x86_64::disable_interrupts();
    arch::x86_64::serial::init();

    kprintln!("");
    kprintln!("ChatOS kernel starting...");
    kprintln!("Architecture : x86_64");
    kprintln!("Boot protocol: Limine");
    kprintln!("Build mode   : freestanding / no_std");
    kprintln!("");
    kprintln!("Milestone 0: kernel entry reached.");
    kprintln!("Next: boot responses, memory map, and physical page allocator.");

    loop {
        arch::x86_64::halt();
    }
}
