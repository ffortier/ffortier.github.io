#![cfg_attr(not(test), no_std)]

pub mod allocator;
pub mod config;
pub mod console;
pub mod error;
pub mod idt;
pub mod io;

extern crate alloc;

use core::fmt::Write;

use alloc::string::String;
use console::Console;
use error::Result;

pub fn run() -> Result<()> {
    let mut console = Console::default();

    crate::idt::init()?;

    console.clear();

    let mut test = String::new();

    writeln!(&mut test, "{} {}", "Francis os", 32).unwrap();

    console.print_string(test.as_str());

    Ok(())
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    pub fn test_something() {}
}
