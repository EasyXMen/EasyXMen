#include <csr.h>

.extern main
.extern rv_trap_handler

.section .entry, "ax", %progbits
.align 3
.global _start
_start:
	/* Mask all interrupts */
	csrw CSR_IE, zero
	csrw CSR_IP, zero

	/* Clear BSS for flat non-ELF images */
	la a3, _bss_start
	la a4, _bss_end
	ble a4, a3, clear_bss_done
clear_bss:
	REG_S zero, (a3)
	add a3, a3, RISCV_SZPTR
	blt a3, a4, clear_bss
clear_bss_done:

	la sp, _stack_end

	/* Intialize vector */
	la t0,rv_trap_handler
	csrw CSR_STVEC,t0

	/* Go to C code */
	tail main

	/* Never get here */
	j .
