use core::panic::PanicInfo;

#[panic_handler]
fn panic(info: &PanicInfo<'_>) -> ! {
    crate::kprintln!();
    crate::kprintln!("!!! CHATOS KERNEL PANIC !!!");
    crate::kprintln!("{}", info);
    crate::kprintln!("System halted.");

    loop {
        crate::arch::x86_64::disable_interrupts();
        crate::arch::x86_64::halt();
    }
}
