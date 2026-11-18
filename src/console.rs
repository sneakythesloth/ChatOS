use core::fmt::{self, Write};

struct SerialWriter;

impl Write for SerialWriter {
    fn write_str(&mut self, value: &str) -> fmt::Result {
        crate::arch::x86_64::serial::write_str(value);
        Ok(())
    }
}

pub fn print(arguments: fmt::Arguments<'_>) {
    let _ = SerialWriter.write_fmt(arguments);
}
