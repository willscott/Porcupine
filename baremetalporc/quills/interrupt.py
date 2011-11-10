import random

class interrupt:
  throwInterval = 0.1

  interrupts = [
    'cli',   # disable interrupts
    'sti',   # enable interrupts
    'fclex', # clear floating point exceptions
    'invd',  # invalidate processor internal caches.
    'wbinvd',
    'int1',
    'int3',
    'smi',
    'wait',
  ]

  def genOp(self):
    if (random.random() < self.throwInterval):
      return random.choice(self.interrupts)

    return ''

