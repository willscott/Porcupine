import random

class store:
  cmds = {
    'wrmsr;%s':[8,16,32,64],
    'mov CR0, %s':[64],
    'mov CR2, %s':[64],
    'mov CR3, %s':[64],
    'mov CR4, %s':[64]
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

  def genOp(self):
    key = random.choice(self.cmds.keys())
    registers = []
    for s in self.cmds[key]:
      registers.extend(self.registers[s])

    return key % random.choice(registers)
