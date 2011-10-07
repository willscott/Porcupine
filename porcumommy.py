import sys
usage = 'python porcumommy.py <#threads>'
original_filename = 'porcupine-orig.c'
dest_filename = 'porcupine.c'

def genasm( threadid ):
	asmresult = ('int asm'+str(threadid)+'(int id) {\n'
		'  int val;\n'
		'  asm("movl $1, %%eax;"\n'
		'  "movl $2, %%ebx;"\n'
		'  "addl %%ebx, %%eax;"\n'
		'  "movl %%eax, %0;"\n'
		'  : "=r" ( val )\n'
		'  );\n'
  		'  printk("Assembly Result: %d\\n", val);\n'
		'  return 0;\n'
		'}\n\n')

	return asmresult


if __name__ == '__main__':
	if (len(sys.argv) < 2):
		print usage
		sys.exit(-1)
	num_threads = int(sys.argv[1])
	print 'Generating porcupine kernel module source with threads=' + str(num_threads)
	
	fr = open(original_filename, 'r')
	fw = open(dest_filename, 'w')
	fw.write('//The number of porcupine threads to be spawned by the kernel module.\n')
	fw.write('#define NUM_THREADS '+str(num_threads)+'\n')
	for line in fr:
		fw.write(line)
	
	#Generate assembly sequences for each thread
	for i in range(num_threads):
		fw.write(genasm(i))

	#Write thread run function
	fw.write(''
	'int porcupine_run(void* param) {\n'
  	'  int id = *(int *)param;\n'
  	'  printk("Porcupine thread %d created with id %d.\\n", current->pid, id);\n')
	for i in range(num_threads):
		fw.write('  if (id == '+str(i)+') asm'+str(i)+'(id);\n')
  	fw.write(''
	'  do_exit(0);\n'
  	'  return 0;\n'
	'}')

	fr.close()
	fw.close()


