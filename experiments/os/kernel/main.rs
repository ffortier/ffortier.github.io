#![no_std]
#![no_main]

use core::default::Default;
use core::panic;
#[cfg(not(test))]
use core::panic::PanicInfo;
use kernel_core::run;

#[no_mangle]
#[link_section = ".text.main"]
#[cfg(not(test))]
extern "C" fn kernel_main() -> ! {
    run().unwrap();

    loop {}
}

#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    let mut console = kernel_core::console::Console::default();
    console.print_string("panicked");
    loop {}
}

#[no_mangle]
#[cfg(not(test))]
extern "C" fn rust_eh_personality() {
    panic!("rust_eh_personality")
}
