#![no_std]

pub mod allocator;
pub mod config;
pub mod console;
pub mod error;
pub mod idt;
pub mod io;

#[macro_use]
extern crate alloc;

use alloc::{borrow::ToOwned, fmt::format, format, string::String, vec};
use allocator::Allocator;
use console::Console;
use error::Result;

#[cfg(not(test))]
#[global_allocator]
static ALLOCATOR: Allocator = Allocator::new();

pub fn run() -> Result<()> {
    let mut console = Console::default();

    crate::idt::init()?;

    console.clear();

    console.print_string("Francis os\n");

    Ok(())
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    pub fn test_something() {}
}
