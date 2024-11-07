
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

User code should be placed in the `/projects` folder

## Build

- `make`

## Flash

- Boot device in programming mode by holding button while powering on device
- `make flash`
