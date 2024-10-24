1.下载RV64ILP32工具链

[ruyisdk/riscv-gnu-toolchain-rv64ilp32: GNU toolchain for RISC-V, including GCC (github.com)](https://github.com/ruyisdk/riscv-gnu-toolchain-rv64ilp32)

2.在XMen/Examples/riscv_helloworld目录下执行

```
make CROSS_COMPILE:= {your path}/riscv/bin/riscv64-unknown-elf-
```

3.新建xmen-qume-rv64ilp32.sh文件
文件内容

```
#!/bin/sh

export PATH="../bin/:${PATH}"

exec qemu-system-riscv64ilp32 -cpu rv64 -M virt -m 1G -nographic -bios fw_dynamic.bin_m64lp64 -kernel riscv_helloworld.bin
```

3.将riscv_helloworld.bin拷贝到/riscv/qemu-linux

```
cp riscv_helloworld.bin {your path}/riscv/qemu-linux/
```

4.在/riscv/qemu-linux目录下执行

```
./xmen-qume-rv64ilp32.sh
```

5.执行成功