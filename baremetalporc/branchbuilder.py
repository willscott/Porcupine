import sys
usage = 'python branchbuilder.py <count>'
dest_filename = 'branches.asm'

if __name__ == '__main__':
  if (len(sys.argv) < 2):
    print usage
    sys.exit(-1)
  count = int(sys.argv[1])
  print 'Generating to branches.asm with count=' + str(count)

  #Open and write headers
  fw = open(dest_filename, 'w')
  fw.write('section .bss\n')
  fw.write('\tbranch_base: resb ' + str(count) + '; branch directions.\n')

  fw.write('section .text\n');
  #Setup.
  fw.write('forward:\n')
  fw.write('\tmov al, 1\n')
  fw.write('\tmov ecx, ' + str(count) + '\n')
  fw.write('\tjmp bsetloop\n')
  fw.write('backward:\n')
  fw.write('\tmov al, 0\n')
  fw.write('\tmov ecx, ' + str(count) + '\n')
  fw.write('bsetloop:\n')
  fw.write('\tsub ecx, 1\n')
  fw.write('\tmov [branch_base + ecx], al\n')
  fw.write('\tcmp ecx,  0\n')
  fw.write('\tjne bsetloop\n')
  fw.write('\tret\n')
  #The Loop
  fw.write('branch:\n')
  fw.write('\tmov al, 1\n');
  fw.write('\tcmp [branch_base + 1], al\n')
  fw.write('\tje branch_' + str(count - 1) + '\n')
  fw.write('\tjmp branch_1\n')
  fw.write('branch_0:\n')
  fw.write('\tjmp branchend\n')
  for i in range(1, count):
    fw.write('branch_' + str(i) +':\n')
    fw.write('\tmov al, [branch_base + ' + str(i) + ']\n') 
    fw.write('\tcmp al, 1\n')
    fw.write('\tjne branch_' + str(i - 1) + '\n')
  fw.write('branch_' + str(count) + ':\n')
  fw.write('\tjmp branchend\n')

  #Return to OS
  fw.write('branchend:\n')
  fw.write('\tret\n')

  fw.close()

