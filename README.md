# STG-8nn-Scaffold-Example

This is an example project using [stg-8nn-scaffold](https://github.com/adolfogc/stg-8nn-scaffold).

## Cloning
```bash
git clone https://github.com/adolfogc/stg-8nn-scaffold-example.git
cd stg-8nn-scaffold-example
git submodule init
git submodule update --init --recursive
```

## Building

### Firmware
```bash
cmake -DSTG_MODEL:STRING=850 -DCMAKE_TOOLCHAIN_FILE=stg-8nn-scaffold/arm-gcc-toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -GNinja ..
```

### Tester
```bash
cmake -DSTG_MODEL:STRING=TESTER -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -GNinja ..
```
