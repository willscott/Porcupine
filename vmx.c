#include <linux/smp.h>
#include <asm/system.h>
#include <asm/page.h>

#define ASM_VMX_VMXOFF            ".byte 0x0f, 0x01, 0xc4"
#define ASM_VMX_VMXON_RAX         ".byte 0xf3, 0x0f, 0xc7, 0x30"
#define X86_CR4_VMXE              0x00002000 /* enable VMX virtualization */

#define MSR_IA32_FEATURE_CONTROL  0x0000003a
#define MSR_IA32_DEBUGCTL         0x000001d9
#define MSR_PERF_FIXED_CTR0       0x00000309
#define MSR_PERF_FIXED_CTR1       0x0000030a
#define MSR_PERF_FIXED_CTR2       0x0000030b

#define FEATURE_CONTROL_LOCKED          (1<<0)
#define FEATURE_CONTROL_VMXON_ENABLED   (1<<2)

#define reginfo(reg) do {           \
  printk("Register %20s: ", #reg); \
  print_field(reg);                \
 } while(0)

  
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

static void print_field(unsigned int reg) {
  int bit;
  u64 field = read_msr(reg);
  for (bit = 63; bit >= 0; bit--) {
    if ((field & (1 << bit)) == (1 << bit)) {
      printk("1");
    } else {
      printk("0");
    }
  }
  printk("\n");
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
  reginfo(MSR_IA32_DEBUGCTL);
  reginfo(MSR_PERF_FIXED_CTR0);
  reginfo(MSR_PERF_FIXED_CTR1);
  reginfo(MSR_PERF_FIXED_CTR2);
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

