"""
Load quills for the porcupine.

"""
import random
import string

import load

modules = [
  load.load()
]

def render(seed, count):
  random.seed(seed)
  ops = [random.choice(modules).genOp() for i in range(count)]
  return string.join(ops, ';\n\t') + ';'
