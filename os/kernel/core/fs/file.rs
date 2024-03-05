use super::path::Path;
use crate::error::{KernelError, Result};
use crate::io::Disk;

pub enum SeekMode {
    Set,
    Current,
    End,
}

pub enum FileMode {
    Read,
    Write,
    Append,
    Invalid,
}

pub struct FileDescriptor;

pub trait FileSystem {
    fn open(disk: &Disk, path: Path, mode: FileMode) -> Result<FileDescriptor>;
    fn resolve(disk: &Disk) -> Result<bool>;
}
