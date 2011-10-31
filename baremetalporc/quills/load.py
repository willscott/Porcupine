import random

class load:
  loaders = {
    'mov %s, 1':[8,16,32,64],
    'mov %s, 0':[8,16,32,64],
    'mov %s, CR0':[64],
    'mov %s, CR2':[64],
    'mov %s, CR3':[64],
    'mov %s, CR4':[64],
    'cpuid;%s':[8]
  }

  registers = {8:[ 'al','ah','cl','ch', 'dl', 'dh', 'bl','bh','spl','bpl',
    'sil','dil','r8b','r9b','r10b','r11b','r12b','r13b','r14b','r15b'
  ], 16:[
    'ax','cx','dx','bx','sp','bp','si','di','r8w','r9w','r10w','r11w','r12w',
    'r13w','r14w','r15w'
  ], 32:[
    'eax','ecx','edx','ebx','esp','ebp','esi','edi','r8d','r9d','r10d','r11d',
    'r12d','r13d','r14d','r15d'
  ], 64: [
    'rax','rcx','rdx','rbx','rsp','rbx','rsi','rdi','r8','r9','r10','r11','r12',
    'r13','r14','r15',
  ]}

  def __init__(self):
    msrs = [
      '0x3a',  # feature control
      '0x1d9', # debug control
      '0x309'  # perf control
    ]
    for addr in msrs:
      self.loaders['mov ecx,%s;%%s\n\trdmsr' %addr] = [8]

  def genOp(self):
    key = random.choice(self.loaders.keys())
    registers = []
    for s in self.loaders[key]:
      registers.extend(self.registers[s])

    return key % random.choice(registers)
