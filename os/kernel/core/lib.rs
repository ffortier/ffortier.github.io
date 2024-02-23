// #![cfg_attr(not(test), no_std)]
#![no_std]

pub mod allocator;
pub mod config;
pub mod console;
pub mod error;
pub mod idt;
pub mod io;
pub mod paging;

extern crate alloc;

use core::{fmt::Write, ops::Add};

use alloc::vec;
use console::Console;
use error::Result;
use idt::{enable_interrupts, init as idt_init};
use io::Disk;
use paging::{enable_paging, page_flags, VirtualMemory};

pub fn run() -> Result<()> {
    let mut console = Console::default();

    console.clear();

    idt_init()?;
    writeln!(&mut console, "{}", "Francis os")?;

    let mut vmem = VirtualMemory::new(
        page_flags::PAGING_IS_WRITABLE
            | page_flags::PAGING_IS_PRESENT
            | page_flags::PAGING_ACCESS_FROM_ALL,
    );

    vmem.switch_page(0);

    enable_paging();
    enable_interrupts();

    let mut buf = vec![0u8; 512];
    let disks = Disk::init_disks();

    disks.get(0).unwrap().read_block(0, 1, &mut buf);

    writeln!(&mut console, "{:?}", buf)?;
    writeln!(&mut console, "{}", "Ok")?;

    loop {}
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    pub fn test_something() {}
}
