#include <sbi.h>

/* default SBI version is 0.1 */
unsigned long sbi_spec_version = SBI_SPEC_VERSION_DEFAULT;

struct sbiret sbi_ecall(int ext, int fid, xlen_t arg0,
			xlen_t arg1, xlen_t arg2,
			xlen_t arg3, xlen_t arg4,
			xlen_t arg5)
{
	struct sbiret ret;

	register xlen_t a0 asm ("a0") = arg0;
	register xlen_t a1 asm ("a1") = arg1;
	register xlen_t a2 asm ("a2") = arg2;
	register xlen_t a3 asm ("a3") = arg3;
	register xlen_t a4 asm ("a4") = arg4;
	register xlen_t a5 asm ("a5") = arg5;
	register xlen_t a6 asm ("a6") = fid;
	register xlen_t a7 asm ("a7") = ext;
	asm volatile ("ecall"
		      : "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
		      : "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}

/**
 * sbi_console_putchar() - Writes given character to the console device.
 * @ch: The data to be written to the console.
 *
 * Return: None
 */
void sbi_console_putchar(int ch)
{
	sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}

/**
 * sbi_console_getchar() - Reads a byte from console device.
 *
 * Returns the value read from console.
 */
int sbi_console_getchar(void)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0, 0, 0, 0, 0);

	return ret.error;
}

static void __sbi_set_timer_v02(u64 stime_value)
{
#if __riscv_xlen == 32
	sbi_ecall(SBI_EXT_TIME, SBI_EXT_TIME_SET_TIMER, stime_value,
		  stime_value >> 32, 0, 0, 0, 0);
#else
	sbi_ecall(SBI_EXT_TIME, SBI_EXT_TIME_SET_TIMER, stime_value, 0,
		  0, 0, 0, 0);
#endif
}


static void sbi_srst_reset(unsigned long type, unsigned long reason)
{
	sbi_ecall(SBI_EXT_SRST, SBI_EXT_SRST_RESET, type, reason,
		  0, 0, 0, 0);
}

static void sbi_srst_power_off(void)
{
	sbi_srst_reset(SBI_SRST_RESET_TYPE_SHUTDOWN,
		       SBI_SRST_RESET_REASON_NONE);
}

/**
 * sbi_probe_extension() - Check if an SBI extension ID is supported or not.
 * @extid: The extension ID to be probed.
 *
 * Return: 1 or an extension specific nonzero value if yes, 0 otherwise.
 */
long sbi_probe_extension(int extid)
{
	struct sbiret ret;

	ret = sbi_ecall(SBI_EXT_BASE, SBI_EXT_BASE_PROBE_EXT, extid,
			0, 0, 0, 0, 0);
	if (!ret.error)
		return ret.value;

	return 0;
}

static long __sbi_base_ecall(int fid)
{
        struct sbiret ret;

        ret = sbi_ecall(SBI_EXT_BASE, fid, 0, 0, 0, 0, 0, 0);
        if (!ret.error)
                return ret.value;
	else
		while(1);
}

static inline long sbi_get_spec_version(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_SPEC_VERSION);
}

static inline long sbi_get_firmware_id(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_IMP_ID);
}

static inline long sbi_get_firmware_version(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_IMP_VERSION);
}

long sbi_get_mvendorid(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_MVENDORID);
}

long sbi_get_marchid(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_MARCHID);
}

long sbi_get_mimpid(void)
{
	return __sbi_base_ecall(SBI_EXT_BASE_GET_MIMPID);
}
