use core::str::FromStr;

use alloc::{borrow::ToOwned, string::String};

use crate::{console::Console, error::KernelError, io::Disk};

pub struct Path {
    full_path: String,
}

impl FromStr for Path {
    type Err = KernelError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(Path {
            full_path: s.to_owned(),
        })
    }
}

impl Path {
    pub fn is_parent(&self, other: &Path) -> bool {
        // TODO: Improve
        other.full_path.starts_with(&self.full_path)
    }
}

pub struct PathRoot<'a> {
    disk: &'a Disk,
    mount_point: Path,
}

impl<'a> PathRoot<'a> {
    pub fn new(disk: &'a Disk, mount_point: Path) -> Self {
        Self { disk, mount_point }
    }

    pub fn is_in_root(&self, path: &Path) -> bool {
        self.mount_point.is_parent(path)
    }
}
