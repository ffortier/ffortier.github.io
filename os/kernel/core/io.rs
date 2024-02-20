extern "C" {
    pub fn insb(port: u16) -> u8;
    pub fn insw(port: u16) -> u16;
    pub fn outb(port: u16, val: u8);
    pub fn outw(port: u16, val: u16);
}
