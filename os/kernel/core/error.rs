use alloc::string::String;

#[derive(Debug)]
pub enum KernelError {
    InvalidArgument(String),
    FormatError(core::fmt::Error),
}

impl From<core::fmt::Error> for KernelError {
    fn from(value: core::fmt::Error) -> Self {
        KernelError::FormatError(value)
    }
}

pub type Result<T> = core::result::Result<T, KernelError>;
