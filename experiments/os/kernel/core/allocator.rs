use core::alloc::GlobalAlloc;

use crate::console::Console;

static mut CURRENT_ADDR: *mut u8 = 0x01000000 as *mut u8;

// Cheap allocation just for fun
pub struct Allocator;

impl Allocator {
    pub const fn new() -> Self {
        Self
    }
}

unsafe impl GlobalAlloc for Allocator {
    unsafe fn alloc(&self, layout: core::alloc::Layout) -> *mut u8 {
        let tmp = CURRENT_ADDR;
        CURRENT_ADDR = CURRENT_ADDR.offset(layout.pad_to_align().size() as isize);
        tmp
    }

    unsafe fn dealloc(&self, _ptr: *mut u8, _layout: core::alloc::Layout) {}
}
