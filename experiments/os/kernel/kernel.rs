#![no_std]
#![no_main]

#[cfg(not(test))]
use core::panic::PanicInfo;

mod video_mem {
    #[repr(packed)]
    #[derive(Debug)]
    #[allow(dead_code)]
    struct Chr {
        ch: u8,
        color: u8,
    }

    const VIDEO_MEM: *mut Chr = 0xB8000 as *mut Chr;
    const VGA_HEIGHT: isize = 25;
    const VGA_WIDTH: isize = 80;

    pub fn put_char(ch: char, color: u8, offset: isize) {
        let chr = Chr {
            ch: ch as u8,
            color,
        };

        unsafe {
            *VIDEO_MEM.offset(offset) = chr;
        }
    }

    pub fn put_chars(ch: &[char], color: u8, offset: isize) {
        for i in 0..ch.len() {
            put_char(ch[i], color, offset + i as isize);
        }
    }

    pub fn clear() {
        let len = VGA_HEIGHT * VGA_WIDTH;
        let mut i = 0;
        while i < len {
            put_char(' ', 0, i);
            i += 1;
        }
    }
}

#[no_mangle]
#[link_section = ".text.main"]
extern "C" fn kernel_main() -> ! {
    video_mem::clear();
    video_mem::put_chars(&['F', 'r', 'a', 'n', 'c', 'i', 's', ' ', 'o', 's'], 2, 0);

    loop {}
}

#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[no_mangle]
extern "C" fn rust_eh_personality() {}
