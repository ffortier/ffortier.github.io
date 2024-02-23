use crate::error::{KernelError, Result};
use alloc::{borrow::ToOwned, vec::Vec};

extern "C" {
    pub fn insb(port: u16) -> u8;
    pub fn insw(port: u16) -> u16;
    pub fn outb(port: u16, val: u8);
    pub fn outw(port: u16, val: u16);
}

const SECTOR_SIZE: usize = 512;
const DISK_FLAG_READY: u8 = 0x08;

pub struct Disk;

impl Disk {
    pub fn read_sectors(lba: usize, total: u8, buf: &mut [u8]) -> Result<()> {
        unsafe {
            outb(0x1f6, ((lba >> 24) | 0xe0) as u8);
            outb(0x1f2, total);
            outb(0x1f3, (lba >> 0) as u8);
            outb(0x1f4, (lba >> 8) as u8);
            outb(0x1f5, (lba >> 16) as u8);
            outb(0x1f7, 0x20u8);

            let mut offset = 0;

            for b in 0..total {
                let mut flags = insb(0x1f7);

                while (flags & DISK_FLAG_READY) == 0 {
                    flags = insb(0x1f7);
                }

                for i in 0..(SECTOR_SIZE / 2) {
                    let bytes = insw(0x1f0).to_be_bytes();

                    match buf.get_mut(offset) {
                        Some(a) => *a = bytes[0],
                        _ => return Err(KernelError::IO("Buffer Overflow".to_owned())),
                    }

                    match buf.get_mut(offset) {
                        Some(a) => *a = bytes[1],
                        _ => return Err(KernelError::IO("Buffer Overflow".to_owned())),
                    }

                    offset += 2;
                }
            }
        }

        Ok(())
    }
}
