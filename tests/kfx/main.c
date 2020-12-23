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

static struct xen_domctl op = {
    .cmd = XEN_DOMCTL_createdomain,
};

void test_main(void)
{
    int interface_version = xtf_probe_domctl_interface_version();
    if ( interface_version < 0 )
        return xtf_error("Failed to get domctl version\n");

    printk("Domctl version: %#x. Struct @ %p size %lu\n", interface_version, &op, sizeof(op));
    op.interface_version = interface_version;

    // TODO - fill @create.u.createdomain with fuzzing input


    uint32_t eax, ebx, ecx, edx;
    uint32_t high = (uint64_t)&op >> 32, low = (uint64_t)&op;

    cpuid_count(0x13371337, sizeof(op), &eax, &ebx, &ecx, &edx);
    cpuid_count(high, low, &eax, &ebx, &ecx, &edx);

    int rc = hypercall_domctl(&op);
    if ( rc == 0 )
    {
        op.cmd = XEN_DOMCTL_destroydomain;
        hypercall_domctl(&op);
    }

    cpuid_eax(0x13371337);

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
