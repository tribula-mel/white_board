#include <stdio.h>

#define MODULE_SYMBOL_PREFIX ""

struct kernel_symbol
{
        unsigned long value;
        const char *name;
};

#define __CRC_SYMBOL(sym, sec)					\
	extern void *__crc_##sym __attribute__((weak));		\
	static const unsigned long __kcrctab_##sym		\
	__attribute__((section("___kcrctab" sec "+" #sym), unused))	\
	= (unsigned long) &__crc_##sym;

/* For every exported symbol, place a struct in the __ksymtab section */
#define __EXPORT_SYMBOL(sym, sec)				\
	extern typeof(sym) sym;					\
	__CRC_SYMBOL(sym, sec)					\
	static const char __kstrtab_##sym[]			\
	__attribute__((section("__ksymtab_strings"), aligned(1))) \
	= MODULE_SYMBOL_PREFIX #sym;				\
	static const struct kernel_symbol __ksymtab_##sym	\
	__attribute__((section("___ksymtab" sec "+" #sym), unused))	\
	= { (unsigned long)&sym, __kstrtab_##sym }

#define EXPORT_SYMBOL(sym)					\
	__EXPORT_SYMBOL(sym, "")

int mel(int m)
{
	return m;
}
EXPORT_SYMBOL(mel);

int main(void)
{
	printf("&mel[%p]\n", &mel);
	printf("mel[%p]\n", mel);

	return 0;
}
