use alloc::vec::Vec;
use alloc::{borrow::ToOwned, vec};

pub mod page_flags {
    pub const PAGING_CACHE_DISABLED: u8 = 0b00010000;
    pub const PAGING_WRITE_THROUGH: u8 = 0b00001000;
    pub const PAGING_ACCESS_FROM_ALL: u8 = 0b00000100;
    pub const PAGING_IS_WRITABLE: u8 = 0b00000010;
    pub const PAGING_IS_PRESENT: u8 = 0b00000001;
}

use crate::error::{KernelError, Result};
use page_flags::*;

const TOTAL_PAGE_TABLES: usize = 1024;
const TOTAL_PAGE_ENTRIES: usize = 1024;
const PAGE_SIZE: usize = 4096;

pub struct PagingChunk {
    page_tables: Vec<usize>,
    page_entries: Vec<usize>,
    current_directory: Option<usize>,
}

mod asm {
    extern "C" {
        pub fn paging_load_directory(directory: *const usize);
        pub fn enable_paging();
    }
}

pub fn enable_paging() {
    unsafe {
        asm::enable_paging();
    }
}

union Addr<T> {
    ptr: *const T,
    num: usize,
}

impl PagingChunk {
    pub fn new(flags: u8) -> Self {
        let mut page_tables = vec![0; TOTAL_PAGE_TABLES];
        let mut page_entries = vec![0; TOTAL_PAGE_TABLES * TOTAL_PAGE_ENTRIES];
        let mut entry = page_entries.as_ptr();

        for i in 0..page_tables.len() {
            unsafe {
                let addr = Addr { ptr: entry };
                page_tables[i] = addr.num | flags as usize | PAGING_IS_WRITABLE as usize;
                entry = entry.add(TOTAL_PAGE_ENTRIES);
            }
        }

        for i in 0..page_entries.len() {
            page_entries[i] = (i * PAGE_SIZE) | flags as usize;
        }

        return Self {
            page_tables,
            page_entries,
            current_directory: None,
        };
    }

    pub fn switch_page(&mut self, page_table_index: usize) {
        unsafe {
            let ptr = self.page_tables.as_ptr().add(page_table_index);

            asm::paging_load_directory(ptr);
        }

        self.current_directory = Some(page_table_index);
    }

    pub fn get_indexes(&self, virtual_address: *const (), table_index: usize) -> Result<usize> {
        Err(KernelError::InvalidArgument(("".to_owned())))
    }
}
