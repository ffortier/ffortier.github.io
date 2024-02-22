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

pub struct VirtualMemory {
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

// #[derive(Clone, Copy)]
union Addr<T> {
    ptr: *const T,
    num: usize,
}

fn assert_aligned(addr: usize, message: &str) -> Result<()> {
    if addr % 4096 != 0 {
        return Err(KernelError::InvalidArgument(message.to_owned()));
    }

    Ok(())
}

fn get_indexes(virtual_address: *const ()) -> Result<(usize, usize)> {
    let (directory_index, page_index) = unsafe {
        let addr = Addr {
            ptr: virtual_address,
        };

        assert_aligned(addr.num, "Virtual address must be aligned")?;

        (
            addr.num / (TOTAL_PAGE_TABLES * PAGE_SIZE),
            addr.num % (TOTAL_PAGE_TABLES * PAGE_SIZE) / PAGE_SIZE,
        )
    };

    Ok((directory_index, page_index))
}

impl VirtualMemory {
    pub fn new(flags: u8) -> Self {
        let mut page_tables = vec![0; TOTAL_PAGE_TABLES];
        let mut page_entries = vec![0; TOTAL_PAGE_TABLES * TOTAL_PAGE_ENTRIES];
        let mut entry = page_entries.as_ptr();

        for i in 0..TOTAL_PAGE_TABLES {
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

    pub fn set(
        &mut self,
        virtual_address: *const (),
        physical_address: *const (),
        flags: u8,
    ) -> Result<()> {
        let (directory_index, page_index) = get_indexes(virtual_address)?;

        if let Some(entry) = self
            .page_entries
            .get_mut(directory_index * TOTAL_PAGE_TABLES + page_index)
        {
            *entry = unsafe {
                let addr = Addr {
                    ptr: physical_address,
                };

                assert_aligned(addr.num, "Physical address must be aligned")?;

                addr.num | flags as usize
            };
        }

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_paging() {
        let mem = VirtualMemory::new(0);
    }
}
