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
  fw.write('section .data\n')
  fw.write('\tforward_base: times ' + str(count) + ' db 0; zeros.\n')
  fw.write('\tbackward_base: times ' + str(count) + ' db 1; ones.\n')

  fw.write('section .text\n');
  #Intro
  fw.write('branch_take:\n')
  fw.write('\tlea rdi, [forward_base]\n')
  fw.write('\tjmp branch_' + str(count - 1) + '\n')
  fw.write('branch_leave:\n')
  fw.write('\tlea rdi, [backward_base]\n')
  fw.write('\tjmp branch_1\n')
  #The Loop
  fw.write('branch_0:\n')
  fw.write('\tjmp branchend\n')

  for i in range(1, count):
    fw.write('branch_' + str(i) +':\n')
    fw.write('\tmov al, [rdi + ' + str(i) + ']\n') 
    fw.write('\tcmp al, 1\n')
    fw.write('\tjne branch_' + str(i - 1) + '\n')

  fw.write('branch_' + str(count) + ':\n')
  fw.write('\tjmp branchend\n')

  #Return to Caller
  fw.write('branchend:\n')
  fw.write('\tret\n')

  fw.close()

