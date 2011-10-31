import random

class interrupt:
  interrupts = [
    'fclex', # clear floating point exceptions
    'into',  # interrupt (sig 4) if overflow
    'int1',
    'int3',
    'smi'
  ]

  def genOp(self):
    return random.choice(self.interrupts)

