
# WCH Toolchain

This toolchain currently support the following devices:

- [LANA TNY](./devices/lana_tny/README.md)

## Install

### MacOS

Install Compiler

- `brew tap riscv-software-src/riscv`
- `brew install riscv-software-src/riscv/riscv-tools`

Install Flashtool

- `brew install libusb`
- (Install Rust `brew install rust`)
- `cargo install wchisp --git https://github.com/ch32-rs/wchisp`
- Add `export PATH=$PATH:/Users/<username>/.cargo/bin` to `~/.zprofile`
- `source ~/.zprofile`

## Develop

User code should be placed in the `/projects` folder.<br>
When creating new project folder, copy `makefile` from a different project inside your new project's folder or copy an existing project.

## Build

Inside your project's folder:

- `make`

## Flash

Inside your project's folder:

- Boot device in programming mode by holding button while powering on device
- `make flash`
