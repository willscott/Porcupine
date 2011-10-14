#include <linux/smp.h>
#include <asm/system.h>
#include <asm/page.h>

#define ASM_VMX_VMXOFF            ".byte 0x0f, 0x01, 0xc4"
#define ASM_VMX_VMXON_RAX         ".byte 0xf3, 0x0f, 0xc7, 0x30"
#define X86_CR4_VMXE              0x00002000 /* enable VMX virtualization */

#define MSR_IA32_FEATURE_CONTROL  0x0000003a
#define FEATURE_CONTROL_LOCKED          (1<<0)
#define FEATURE_CONTROL_VMXON_ENABLED   (1<<2)

  
static inline unsigned long long read_msr(unsigned int msr)
{
       unsigned low, high;
       asm volatile("rdmsr" : "=a" (low), "=d" (high) : "c" (msr));
       return ((low) | ((u64)(high) << 32));
}

static inline void write_msr(unsigned int msr, unsigned long long val)
{
        unsigned low = (unsigned)val, high = (unsigned)(val >> 32);
        asm volatile("wrmsr" : : "c" (msr), "a"(low), "d" (high) : "memory");
}

struct vmcs {
        u32 revision_id;
         u32 abort;
         char data[0];
};

static DEFINE_PER_CPU(struct vmcs *, vmxarea);

static void hardware_status() {
  int cpu;
  u64 status, cr4;
  cpu = raw_smp_processor_id();
  printk("CPU %d: ", cpu);
  status = read_msr(MSR_IA32_FEATURE_CONTROL);
  if ((status & FEATURE_CONTROL_LOCKED) == FEATURE_CONTROL_LOCKED) {
    printk("Locked. ");
  }
  if ((status & FEATURE_CONTROL_VMXON_ENABLED) == FEATURE_CONTROL_VMXON_ENABLED) {
    printk("VMX On Enabled. ");
  }
  cr4 = read_cr4();
  if ((cr4 & X86_CR4_VMXE) == X86_CR4_VMXE) {
    printk("CR4 VMX bit set.");
  }
  printk("\n");
}

static void hardware_enable(void *garbage)
 {
       int cpu = raw_smp_processor_id();
       u64 phys_addr = __pa(per_cpu(vmxarea, cpu));
       u64 old;
       
       old = read_msr(MSR_IA32_FEATURE_CONTROL);
       if ((old & (FEATURE_CONTROL_LOCKED |
                   FEATURE_CONTROL_VMXON_ENABLED))
            != (FEATURE_CONTROL_LOCKED |
             FEATURE_CONTROL_VMXON_ENABLED))
                 /* enable and lock */
                write_msr(MSR_IA32_FEATURE_CONTROL, old |
                       FEATURE_CONTROL_LOCKED |
                       FEATURE_CONTROL_VMXON_ENABLED);
       write_cr4(read_cr4() | X86_CR4_VMXE);
       asm volatile (ASM_VMX_VMXON_RAX
                      : : "a"(&phys_addr), "m"(phys_addr)
                     : "memory", "cc");
}

static void hardware_disable(void *garbage)
{
        asm volatile (ASM_VMX_VMXOFF : : : "cc");
        write_cr4(read_cr4() & ~X86_CR4_VMXE);
}

