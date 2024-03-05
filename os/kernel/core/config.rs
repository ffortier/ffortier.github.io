#![allow(dead_code)]

mod x86 {
    pub const VIDEO_DISPLAY_MEMORY: usize = 0x000A0000;
    pub const TOTAL_INTERRUPTS: usize = 0x200;
    pub const KERNEL_CODE_SELECTOR: u16 = 0x08;
    pub const KERNEL_DATA_SELECTOR: u16 = 0x10;
    pub const KERNEL_HEAP_SIZE: usize = 100 * 1024 * 1024; // 100mb
    pub const KERNEL_BLOCK_SIZE: usize = 4096; // 4kb
}

pub use x86::*;
