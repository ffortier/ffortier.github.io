use core::fmt::Write;

#[repr(packed)]
#[derive(Debug)]
#[allow(dead_code)]
struct Chr {
    ch: u8,
    color: u8,
}

#[derive(Debug, Default)]
pub struct ConsoleState {
    line: usize,
    col: usize,
    color: u8,
}

impl ConsoleState {
    pub const fn new() -> Self {
        Self {
            line: 0,
            col: 0,
            color: 7,
        }
    }
}

#[derive(Debug)]
pub struct Console<'a> {
    video_mem: *mut Chr,
    width: usize,
    height: usize,
    state: &'a mut ConsoleState,
}

static mut GLOBAL_CONSOLE_STATE: ConsoleState = ConsoleState::new();

impl Default for Console<'static> {
    fn default() -> Self {
        unsafe { Self::new(0xB8000 as *mut u16, &mut GLOBAL_CONSOLE_STATE) }
    }
}

impl Write for Console<'_> {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        self.print_string(s);
        Ok(())
    }
}

impl<'a> Console<'a> {
    pub fn new(video_mem: *mut u16, state: &'a mut ConsoleState) -> Self {
        Self {
            video_mem: video_mem as *mut Chr,
            width: 80,
            height: 25,
            state,
        }
    }

    pub fn print_char(&mut self, ch: char) {
        match ch {
            '\r' => {
                self.state.col = 0;
            }
            '\n' => {
                self.state.col = 0;
                self.state.line += 1;
            }
            _ => {
                self.put_char(
                    ch,
                    self.state.color,
                    self.state.line * self.width + self.state.col,
                );
                self.state.col += 1;
            }
        }

        if self.state.col >= self.width {
            self.state.col = 0;
            self.state.line += 1;
        }

        // TODO: check line >= height
    }

    pub fn clear(&mut self) {
        self.state.col = 0;
        self.state.line = 0;

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
