#![no_std]

mod config;
pub mod console;
pub mod error;
mod idt;

use console::Console;
use error::Result;

pub fn run() -> Result<()> {
    let mut console = Console::default();

    crate::idt::init();

    console.clear();
    console.print_string("Hello world!\n");
    console.print_string("Welcome to Francis os\n");

    Ok(())
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    pub fn test_something() {}
}
