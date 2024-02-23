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

#[derive(Clone, Copy)]
pub union Addr<T> {
    pub ptr: *const T,
    pub num: usize,
}

fn assert_aligned(addr: usize, message: &str) -> Result<()> {
    if addr % 4096 != 0 {
        return Err(KernelError::InvalidArgument(message.to_owned()));
    }

    Ok(())
}

fn get_indexes<A>(virtual_address: Addr<A>) -> Result<(usize, usize)> {
    let (directory_index, page_index) = unsafe {
        assert_aligned(virtual_address.num, "Virtual address must be aligned")?;

        (
            virtual_address.num / (TOTAL_PAGE_ENTRIES * PAGE_SIZE),
            virtual_address.num % (TOTAL_PAGE_ENTRIES * PAGE_SIZE) / PAGE_SIZE,
        )
    };

    Ok((directory_index, page_index))
}

impl VirtualMemory {
    pub fn new(flags: u8) -> Self {
        let mut page_tables = vec![0; TOTAL_PAGE_ENTRIES];
        let mut page_entries = vec![0; page_tables.len() * TOTAL_PAGE_ENTRIES];
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

    pub fn set<A, B>(
        &mut self,
        virtual_address: Addr<A>,
        physical_address: Addr<B>,
        flags: u8,
    ) -> Result<()> {
        let (directory_index, page_index) = get_indexes(virtual_address)?;

        if let Some(entry) = self
            .page_entries
            .get_mut(directory_index * TOTAL_PAGE_ENTRIES + page_index)
        {
            *entry = unsafe {
                assert_aligned(physical_address.num, "Physical address must be aligned")?;

                physical_address.num | flags as usize
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
