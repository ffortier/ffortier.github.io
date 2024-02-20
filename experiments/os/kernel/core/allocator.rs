use core::alloc::GlobalAlloc;
use core::ptr::null_mut;
use core::sync::atomic::AtomicBool;

use crate::config::{KERNEL_BLOCK_SIZE, KERNEL_HEAP_SIZE};

// Cheap allocation just for fun
pub struct Allocator {
    start_address: *mut u8,
    heap_table_address: *mut u8,
    heap_table_size: usize,
    initialized: AtomicBool,
}

unsafe impl Sync for Allocator {}

impl Allocator {
    pub const fn new() -> Self {
        Self {
            start_address: 0x01000000 as *mut u8,
            heap_table_address: 0x0007ffff as *mut u8,
            heap_table_size: KERNEL_HEAP_SIZE / KERNEL_BLOCK_SIZE,
            initialized: AtomicBool::new(false),
        }
    }

    #[cfg(test)]
    pub const fn new_at(
        start_address: *mut u8,
        heap_table_address: *mut u8,
        heap_table_size: usize,
    ) -> Self {
        Self {
            start_address,
            heap_table_address,
            heap_table_size,
            initialized: AtomicBool::new(false),
        }
    }

    unsafe fn ensure_init(&self) {
        let initialized = self.initialized.as_ptr();

        if !*initialized {
            self.heap_table_address.write_bytes(0, self.heap_table_size);
            *initialized = true;
        }
    }
}

const OCCUPIED_BLOCK: u8 = 0b00000001;

unsafe impl GlobalAlloc for Allocator {
    unsafe fn alloc(&self, layout: core::alloc::Layout) -> *mut u8 {
        self.ensure_init();

        let aligned_size = layout
            .align_to(KERNEL_BLOCK_SIZE)
            .expect("Must align with kernel block size")
            .pad_to_align()
            .size();

        let block_count = aligned_size / KERNEL_BLOCK_SIZE;
        let mut free_streak = 0;

        for i in 0..self.heap_table_size {
            if *self.heap_table_address.add(i) == 0 {
                free_streak += 1;

                if free_streak == block_count {
                    let start_index = i - (block_count - 1);

                    self.heap_table_address
                        .add(start_index)
                        .write_bytes(OCCUPIED_BLOCK, block_count);

                    return self.start_address.add(start_index * KERNEL_BLOCK_SIZE);
                }
            } else {
                free_streak = 0;
            }
        }

        null_mut()
    }

    unsafe fn dealloc(&self, ptr: *mut u8, layout: core::alloc::Layout) {
        self.ensure_init();

        let offset = ptr.offset_from(self.start_address);
        let i = offset as usize / KERNEL_BLOCK_SIZE;
        let start = self.heap_table_address.add(i);
        let aligned_size = layout
            .align_to(KERNEL_BLOCK_SIZE)
            .expect("Must align with kernel block size")
            .pad_to_align()
            .size();

        let block_count = aligned_size / KERNEL_BLOCK_SIZE;

        start.write_bytes(0, block_count)
    }
}

#[cfg(test)]
mod tests {
    use core::alloc::Layout;

    use alloc::vec;

    use super::*;

    #[test]
    fn test_simple_alloc() {
        let mut table_buffer = vec![1u8; 25];
        let mut heap_buffer = vec![1u8; 25 * KERNEL_BLOCK_SIZE];
        let allocator = Allocator::new_at(heap_buffer.as_mut_ptr(), table_buffer.as_mut_ptr(), 25);

        let (p1, p2, p3) = unsafe {
            (
                allocator.alloc(Layout::from_size_align(10, 4).unwrap()),
                allocator.alloc(Layout::from_size_align(10 * KERNEL_BLOCK_SIZE - 1, 4).unwrap()),
                allocator.alloc(Layout::from_size_align(10, 4).unwrap()),
            )
        };

        assert_eq!(
            table_buffer
                .iter()
                .copied()
                .filter(|entry| *entry == OCCUPIED_BLOCK)
                .count(),
            12
        );

        let (o1, o2, o3) = unsafe {
            (
                p1.offset_from(heap_buffer.as_ptr()),
                p2.offset_from(heap_buffer.as_ptr()),
                p3.offset_from(heap_buffer.as_ptr()),
            )
        };

        assert_eq!(o1, KERNEL_BLOCK_SIZE as isize * 0);
        assert_eq!(o2, KERNEL_BLOCK_SIZE as isize * 1);
        assert_eq!(o3, KERNEL_BLOCK_SIZE as isize * 11);

        unsafe {
            allocator.dealloc(p1, Layout::from_size_align(10, 4).unwrap());
        };

        let (p4, p5) = unsafe {
            (
                allocator.alloc(Layout::from_size_align(10 * KERNEL_BLOCK_SIZE, 4).unwrap()),
                allocator.alloc(Layout::from_size_align(10, 4).unwrap()),
            )
        };

        let (o4, o5) = unsafe {
            (
                p4.offset_from(heap_buffer.as_ptr()),
                p5.offset_from(heap_buffer.as_ptr()),
            )
        };

        assert_eq!(o4, KERNEL_BLOCK_SIZE as isize * 12);
        assert_eq!(o5, KERNEL_BLOCK_SIZE as isize * 0);
    }
}
