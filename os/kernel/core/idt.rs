use core::{mem::size_of, usize};

use crate::config;
use crate::console::Console;
use crate::error::{Error, Result};
use crate::io::outb;

#[repr(C, packed)]
#[derive(Debug, Default, Clone, Copy)]
struct IdtDescriptor {
    offset_1: u16,
    selector: u16,
    zero: u8,
    type_attr: u8,
    offset_2: u16,
}

#[repr(C, packed)]
#[derive(Debug)]
#[allow(dead_code)]
struct IdtrDescriptor {
    limit: u16,
    base: usize,
}

static mut DESCRIPTORS: [IdtDescriptor; config::TOTAL_INTERRUPTS] = [IdtDescriptor {
    offset_1: 0,
    selector: 0,
    zero: 0,
    type_attr: 0,
    offset_2: 0,
}; config::TOTAL_INTERRUPTS];

static mut IDTR_DESC: IdtrDescriptor = IdtrDescriptor {
    limit: (size_of::<[IdtDescriptor; config::TOTAL_INTERRUPTS]>() - 1) as u16,
    base: 0,
};

// TODO: cfg(target_arch = "x86") and add something for 64 bit targets when running tests
struct AddressOffset {
    offset_1: u16,
    offset_2: u16,
}

union Address<T> {
    val: usize,
    ptr: *const T,
    offset: core::mem::ManuallyDrop<AddressOffset>,
}

#[allow(dead_code)]
extern "C" {
    fn idt_load(p: *const IdtrDescriptor);
    fn int21h();
    fn no_interrupt();
    fn enable_interrupts();
    fn disable_interrupts();
}

pub fn init() -> Result<()> {
    unsafe {
        let p = Address {
            ptr: DESCRIPTORS.as_ptr(),
        };

        IDTR_DESC.base = p.val;
    }

    for i in 0..config::TOTAL_INTERRUPTS {
        set_interrupt(i, no_interrupt as *const ())?;
    }

    set_interrupt(0, handle_zero as *const ())?;

    set_interrupt(0x21, int21h as *const ())?;

    unsafe {
        idt_load(&IDTR_DESC);
        enable_interrupts();
    }

    Ok(())
}

fn handle_zero() {
    Console::default().print_string("Division by zero!");
}

#[no_mangle]
fn int21_handler() {
    Console::default().print_string("Keyboard pressed");

    unsafe { outb(0x20, 0x20) }
}

#[no_mangle]
fn no_interrupt_handler() {
    unsafe { outb(0x20, 0x20) }
}

fn set_interrupt(id: usize, handler: *const ()) -> Result<()> {
    unsafe {
        let handler_address = Address { ptr: handler };

        match DESCRIPTORS.get_mut(id) {
            Some(descr) => {
                descr.selector = config::KERNEL_CODE_SELECTOR;
                descr.offset_1 = handler_address.offset.offset_1;
                descr.offset_2 = handler_address.offset.offset_2;
                descr.type_attr = 0xee;
                Ok(())
            }
            _ => Err(Error::InvalidInterruptNumber),
        }
    }
}
