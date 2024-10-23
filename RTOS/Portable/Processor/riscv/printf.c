#include "printf.h"


static void print_num(unsigned long num, int base)
{
	if (num == 0)
    {
        return;
    }
	print_num(num/base, base);

    sbi_console_putchar("0123456789abcdef"[num%base]);
}

static void print_dec(int dec)
{
    int num;

	if (dec < 0)
    {
        sbi_console_putchar('-');
		dec = -dec;
    }

    if (dec == 0)
    {
        sbi_console_putchar('0');
		return;
    }
    else
    {
        print_num(dec, 10);
    }
}

static void print_oct(unsigned oct)
{
    if (oct == 0)
    {
		sbi_console_putchar('0');
		return;
    }
    else
    {
        print_num(oct,8);
    }
}

static void print_hex(unsigned hex)
{
    if (hex == 0)
    {
        sbi_console_putchar('0');
		return;
    }
    else
    {
        print_num(hex,16);
    }
}

static void print_addr(unsigned long addr)
{
	sbi_console_putchar('0');
    sbi_console_putchar('x');

    print_num(addr, 16);
}

static void print_str(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        sbi_console_putchar(str[i++]);
    }
}

static void print_float(double f)
{
    int temp;

    temp = (int)f;
    print_num(temp,10);
	
    sbi_console_putchar('.');

    f -= temp;
    if (f == 0)
    {
		for (temp = 0; temp < 6; temp++)
		{
		    sbi_console_putchar('0');
		}
		return;
    }
    else
    {
        temp = (int)(f*1000000);
        print_num(temp,10);
    }
}


void myprintf(char *s, ...)
{
    int i = 0;

    va_list va_ptr;

    va_start(va_ptr, s);

    while (s[i] != '\0')
    {
		if (s[i] != '%')
		{
    	    sbi_console_putchar(s[i++]);
			continue;
		}
		
		switch (s[++i])
		{
			case 'd': 
                print_dec(va_arg(va_ptr,int));           
			  	break; 
		    case 'o': 
                print_oct(va_arg(va_ptr,unsigned int));  
			    break;
		    case 'x': 
                print_hex(va_arg(va_ptr,unsigned int));  
		    	break;
		    case 'c': 
                sbi_console_putchar(va_arg(va_ptr,int));            
			  	break;
		    case 'p': 
                print_addr(va_arg(va_ptr,unsigned long));
			  	break;
		    case 'f': 
                print_float(va_arg(va_ptr,double));      
			  	break;
		    case 's': 
                print_str(va_arg(va_ptr,char *));
				break;
			default : 
                break;
		}
		i++;
    }
    va_end(va_ptr);
}
