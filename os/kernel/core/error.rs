#[derive(Debug)]
pub enum Error {
    InvalidInterruptNumber,
}

pub type Result<T> = core::result::Result<T, Error>;
