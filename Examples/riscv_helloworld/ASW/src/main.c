#include "Os.h"
#include <sbi.h>

int main(void)
{
	sbi_console_putchar('X');
	sbi_console_putchar('m');
	sbi_console_putchar('e');
	sbi_console_putchar('n');
	sbi_console_putchar('-');
	sbi_console_putchar('r');
	sbi_console_putchar('v');
	sbi_console_putchar('6');
	sbi_console_putchar('4');
	sbi_console_putchar('i');
	sbi_console_putchar('l');
	sbi_console_putchar('p');
	sbi_console_putchar('3');
	sbi_console_putchar('2');
	sbi_console_putchar('\n');

	StartOS(1);

	return 0;
}
