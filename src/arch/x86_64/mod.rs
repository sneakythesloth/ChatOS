pub mod serial;

#[inline]
pub fn disable_interrupts() {
    // Keep interrupts disabled until the IDT and interrupt controllers exist.
    unsafe {
        core::arch::asm!("cli", options(nomem, nostack, preserves_flags));
    }
}

#[inline]
pub fn halt() {
    unsafe {
        core::arch::asm!("hlt", options(nomem, nostack));
    }
}
