/**
 * @file tests/drakvuf/main.c
 * @ref test-drakvuf
 *
 * @page test-drakvuf drakvuf
 *
 * @todo Docs for test-drakvuf
 *
 * @see tests/drakvuf/main.c
 */
#include <xtf.h>

const char test_title[] = "DRAKVUF test";

int __attribute__ ((noinline)) drakvuf_test(int i)
{
    return i+9000;
}

void test_main(void)
{
    uint32_t eax, ebx, ecx, edx;

    cpuid_count(0xdeadbeef, (int)drakvuf_test, &eax, &ebx, &ecx, &edx);

    unsigned int i = drakvuf_test(eax);

    unsigned char volatile *test = (unsigned char*)drakvuf_test;
    unsigned char volatile *test2 = (unsigned char*)((0xff001 << 12) | ((unsigned long)drakvuf_test & 0xfff));
    unsigned char c = *test;

    if ( c == 0xCC )
    {
        cpuid_count(0xdeadbeef, 1, &eax, &ebx, &ecx, &edx);
        xtf_failure("Test result: breakpoint read test failed\n");
    }

    c = *test2;

    if ( c != 0xFF )
    {
        cpuid_count(0xdeadbeef, 2, &eax, &ebx, &ecx, &edx);
        xtf_failure("Test result: shadow page read test failed\n");
    }

    *test2 = i;
    unsigned char d = *test2;

    if ( d != 0xFF )
    {
        cpuid_count(0xdeadbeef, 3, &eax, &ebx, &ecx, &edx);
        xtf_failure("Test result: shadow page read test failed\n");
    }

    *(test - 1) = 0x90;

    i = drakvuf_test(eax);
    if ( i != eax + 9000 )
    {
        cpuid_count(0xdeadbeef, 4, &eax, &ebx, &ecx, &edx);
        xtf_failure("Test result: read write test failed\n");
    }

    cpuid_count(0xdeadbeef, 0, &eax, &ebx, &ecx, &edx);
    xtf_success("Test result: 0x%x 0x%x %i 0x%p 0x%p\n", c, d, i, drakvuf_test, test);
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
