CC	= $(CROSS_COMPILE)gcc
CPP	= $(CROSS_COMPILE)cpp
AR 	= $(CROSS_COMPILE)ar
LD 	= $(CROSS_COMPILE)ld
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

CFLAGS	= -fPIC -fno-stack-protector
TARGET	= riscv_helloworld

CC_INCLUDE := -I$(CURDIR)/Paltform/Include -I$(CURDIR)/../../RTOS/Kernel/
CC_INCLUDE += -I$(CURDIR)/../../RTOS/Portable/Processor/riscv/
CC_INCLUDE += -I$(CURDIR)/Config/OS_Config/
CC_INCLUDE += -I$(CURDIR)/Paltform/Include/

CC_COMPILE := $(CURDIR)/Config/OS_Config/%.c
CC_COMPILE += $(CURDIR)/ASW/src/%.c
CC_COMPILE += $(CURDIR)/../../RTOS/Kernel/%.c
CC_COMPILE += $(CURDIR)/../../RTOS/Portable/Processor/riscv/%.c

${TARGET}.elf: start.o RV_PortASM.o *.o
	$(LD) $(CROSS_LDFLAGS) -melf32lriscv -T$(CURDIR)/Startup/link.ld *.o -o $@
	$(OBJCOPY) -O binary $@ ${TARGET}.bin
	$(OBJDUMP) -D $@ > ${TARGET}.asm

%.o: $(CC_COMPILE)
	$(CC) -mcmodel=medlow ${CFLAGS} -g -c $^ $(CC_INCLUDE)

ASM_FLAG := $(CC_INCLUDE) -D__ASSEMBLY__

start.o: $(CURDIR)/Startup/start.S
	$(CC) ${CFLAGS} -g -c $^ $(ASM_FLAG)

RV_PortASM.o: $(CURDIR)/../../RTOS/Portable/Processor/riscv/RV_PortASM.S
	$(CC) ${CFLAGS} -g -c $^ $(ASM_FLAG)

clean:
	-rm -rf *.o *.bin *.elf *.asm
