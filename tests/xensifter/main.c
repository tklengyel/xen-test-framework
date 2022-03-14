/**
 * @file tests/xensifter/main.c
 * @ref test-xensifter - Xensifter stub VM.
 *
 * @page xensifter
 *
 * A simple stub VM to get into the architecturally desired setup.
 *
 * @see tests/example/main.c
 */
#include <xtf.h>

const char test_title[] = "xensifter";

void test_main(void)
{
    unsigned int i;
    uint32_t eax, ebx, ecx, edx;

    /* set interrupt handlers to magic pointers */
    struct xtf_idte idte = {
        .addr = _u(0x1337),
        .cs   = __KERN_CS,
    };

    for (i=0; i<=20; i++)
    {
        xtf_set_idte(i, &idte);
        idte.addr += 1;
    }

    /* this is where xensifter will hook the execution at */
    cpuid_count(0x13371337, 0x1337, &eax, &ebx, &ecx, &edx);

    /* never reached when ran under xensifter */
    xtf_success(NULL);
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
