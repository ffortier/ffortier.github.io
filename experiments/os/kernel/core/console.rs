use core::fmt::Write;

#[repr(packed)]
#[derive(Debug)]
#[allow(dead_code)]
struct Chr {
    ch: u8,
    color: u8,
}

#[derive(Debug)]
pub struct Console {
    line: usize,
    col: usize,
    color: u8,
    video_mem: *mut Chr,
    width: usize,
    height: usize,
}

impl Default for Console {
    fn default() -> Self {
        Self::new(0xB8000 as *mut u16)
    }
}

impl Write for Console {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        self.print_string(s);
        Ok(())
    }
}

impl Console {
    pub fn new(video_mem: *mut u16) -> Self {
        Self {
            line: 0,
            col: 0,
            color: 15,
            video_mem: video_mem as *mut Chr,
            width: 80,
            height: 25,
        }
    }

    pub fn print_char(&mut self, ch: char) {
        match ch {
            '\r' => {
                self.col = 0;
            }
            '\n' => {
                self.col = 0;
                self.line += 1;
            }
            _ => {
                self.put_char(ch, self.color, self.line * self.width + self.col);
                self.col += 1;
            }
        }

        if self.col >= self.width {
            self.col = 0;
            self.line += 1;
        }

        // TODO: check line >= height
    }

    pub fn clear(&mut self) {
        self.col = 0;
        self.line = 0;

        for i in 0..(self.width * self.height) {
            self.put_char(' ', 0, i);
        }
    }

    pub fn print_string(&mut self, s: &str) {
        for ch in s.chars() {
            self.print_char(ch);
        }
    }

    fn put_char(&mut self, ch: char, color: u8, offset: usize) {
        let chr = Chr {
            ch: ch as u8,
            color,
        };

        unsafe {
            *self.video_mem.offset(offset as isize) = chr;
        }
    }
}
