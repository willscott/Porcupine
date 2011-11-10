import random

class store:
  cmds = {
    'mov CR0, %s':[64],
    'mov CR2, %s':[64],
    'mov CR3, %s':[64],
    'mov CR4, %s':[64],
    'outsd;%s':[8]
  }

  registers = {8:[ 'al','ah','cl','ch', 'dl', 'dh', 'bl','bh',
    'sil','dil','r8b','r9b','r10b','r11b','r12b','r13b','r14b','r15b'
  ], 16:[
    'ax','cx','dx','bx','si','di','r8w','r9w','r10w','r11w','r12w',
    'r13w','r14w','r15w'
  ], 32:[
    'eax','ecx','edx','ebx','esi','edi','r8d','r9d','r10d','r11d',
    'r12d','r13d','r14d','r15d'
  ], 64: [
    'rax','rcx','rdx','rbx','rsi','rdi','r8','r9','r10','r11','r12',
    'r13','r14','r15',
  ]}

  def __init__(self):
    msrs = [
      '0x3a',  # feature control
      '0x1d9', # debug control
      '0x309'  # perf control
    ]
    for addr in msrs:
      self.cmds['mov ecx,%s;%%s\n\twrmsr' %addr] = [8]

  def genOp(self):
    key = random.choice(self.cmds.keys())
    registers = []
    for s in self.cmds[key]:
      registers.extend(self.registers[s])

    return key % random.choice(registers)
