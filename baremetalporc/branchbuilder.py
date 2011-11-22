import sys
usage = 'python branchbuilder.py <count> [grain_width grain_reps]'
dest_filename = 'branches.asm'

if __name__ == '__main__':
  if (len(sys.argv) < 2):
    print usage
    sys.exit(-1)
  grain = (1, '1')
  if len(sys.argv) > 3:
    grain = (int(sys.argv[2]), sys.argv[3])
  count = int(sys.argv[1])
  print 'Generating branches with count=' + str(count) + ', grain=' + str(grain)

  #Open and write headers
  fw = open(dest_filename, 'w')
  fw.write('section .data\n')
  fw.write('\tup_base: times ' + str(count) + ' db 0; zeros.\n')
  fw.write('\tdown_base: times ' + str(count) + ' db 1; ones.\n')

  fw.write('section .text\n');
  #Intro
  fw.write('branch_up:\n')
  fw.write('\tlea rdi, [up_base]\n')
  if grain[0] is not 1:
    fw.write('\tmov edx, ' + grain[1] + '\n')
  fw.write('\tjmp branch_' + str(count - 1) + '\n')
  fw.write('branch_down:\n')
  if grain[0] is not 1:
    fw.write('\tmov edx, 1\n')
  fw.write('\tlea rdi, [down_base]\n')
  fw.write('\tjmp branch_1\n')
  #The Loop
  fw.write('branch_0:\n')
  fw.write('\tjmp branchend\n')    

  for i in range(1, count):
    fw.write('branch_' + str(i) +':\n')
    if i % grain[0] is 0 and grain[0] is not 1 and (i + grain[0]) < count:
      fw.write('\tdec edx\n')
      fw.write('\tcmp edx, 0\n')
      fw.write('\tjne branch_' + str(i + grain[0] - 1) +'\n')
      fw.write('\tmov edx, ' + grain[1] + '\n')
    fw.write('\tmov al, [rdi + ' + str(i) + ']\n') 
    fw.write('\tcmp al, 1\n')
    fw.write('\tjne branch_' + str(i - 1) + '\n')
    if i % grain[0] is 0 and grain[0] is not 1 and (i + grain[0]) < count:
      fw.write('\tmov edx, 1\n')

  fw.write('branch_' + str(count) + ':\n')
  fw.write('\tjmp branchend\n')

  #Return to Caller
  fw.write('branchend:\n')
  fw.write('\tret\n')

  fw.close()

