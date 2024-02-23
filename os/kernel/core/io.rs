use crate::error::{KernelError, Result};
use alloc::{borrow::ToOwned, vec, vec::Vec};

extern "C" {
    pub fn insb(port: u16) -> u8;
    pub fn insw(port: u16) -> u16;
    pub fn outb(port: u16, val: u8);
    pub fn outw(port: u16, val: u16);
}

const SECTOR_SIZE: usize = 512;
const DISK_FLAG_READY: u8 = 0x08;

pub enum Disk {
    Physical(PhysicalDisk),
}

impl Disk {
    pub fn init_disks() -> Vec<Disk> {
        let primary = PhysicalDisk {
            sector_size: 512,
            base_port: 0x1f0,
        };

        let secondary = PhysicalDisk {
            sector_size: 512,
            base_port: 0x170,
        };

        vec![Disk::Physical(primary), Disk::Physical(secondary)]
    }

    pub fn read_block(&self, lba: usize, total: u8, buf: &mut [u8]) -> Result<usize> {
        match self {
            Disk::Physical(disk) => disk.read_sectors(lba, total, buf),
        }
    }
}

struct PhysicalDisk {
    sector_size: usize,
    base_port: u16,
}

impl PhysicalDisk {
    pub fn read_sectors(&self, lba: usize, total: u8, buf: &mut [u8]) -> Result<usize> {
        unsafe {
            outb(self.base_port | 0x6, ((lba >> 24) | 0xe0) as u8);
            outb(self.base_port | 0x2, total);
            outb(self.base_port | 0x3, (lba >> 0) as u8);
            outb(self.base_port | 0x4, (lba >> 8) as u8);
            outb(self.base_port | 0x5, (lba >> 16) as u8);
            outb(self.base_port | 0x7, 0x20u8);

            let mut offset = 0;

            for b in 0..total {
                let mut flags = insb(self.base_port | 0x7);

                while (flags & DISK_FLAG_READY) == 0 {
                    flags = insb(self.base_port | 0x7);
                }

                for i in 0..(SECTOR_SIZE / 2) {
                    let bytes = insw(self.base_port | 0x0).to_be_bytes();

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

            Ok(offset)
        }
    }
}
