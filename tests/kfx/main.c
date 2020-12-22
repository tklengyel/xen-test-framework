/**
 * @file tests/kfx/main.c
 * @ref test-kfx
 *
 * @page test-kfx kfx
 *
 * @todo Docs for test-kfx
 *
 * @see tests/kfx/main.c
 */
#include <xtf.h>

const char test_title[] = "Test kfx";

static struct xen_domctl op;

static inline void harness(unsigned int magic, void *a, size_t s)
{
    asm volatile ("cpuid"
                  : "=a" (magic), "=c" (magic), "=S" (magic)
                  : "a" (magic), "c" (s), "S" (a)
                  : "bx", "dx");
}

void test_main(void)
{
    int interface_version = xtf_probe_domctl_interface_version();
    if ( interface_version < 0 )
        return xtf_error("Failed to get domctl version\n");

    printk("Domctl version: %#x. Struct @ %p size %lu\n", interface_version, &op, sizeof(op));

    harness(0x13371337, &op, sizeof(op));

    op.cmd = XEN_DOMCTL_createdomain,
    op.interface_version = interface_version;

    int rc = hypercall_domctl(&op);
    if ( rc == 0 )
    {
        op.cmd = XEN_DOMCTL_destroydomain;
        hypercall_domctl(&op);
    }

    harness(0x13371337, NULL, 0);

    xtf_success("Fuzzing done\n");
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
