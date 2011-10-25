import sys
usage = 'python porcumommy.py <#threads>'
dest_filename = 'porcubaby.asm'

def genasm( threadid ):
	asmresult = ('asm'+str(threadid)+':\n'
		'\tmov rsi, test_message'+str(threadid)+'\n'
		'\tcall b_print_string\n'
		'\n')

	return asmresult


if __name__ == '__main__':
	if (len(sys.argv) < 2):
		print usage
		sys.exit(-1)
	num_threads = int(sys.argv[1])
	print 'Generating porcupine baremetal assembly with threads=' + str(num_threads)
	
	#Open and write headers
	fw = open(dest_filename, 'w')
	fw.write('[BITS 64]\n[ORG 0x0000000000200000]\n\n')
	fw.write('%include "bmdev.asm"\n\n')
	
	#Write start point
	fw.write('start:\n')
	fw.write('\tjmp asm0\n\n')

	#Generate assembly sequences for each thread
	for i in range(num_threads):
		fw.write(genasm(i))
	
	#Return to OS
	fw.write('\nret\n')

	#Constants
	for i in range(num_threads):
		fw.write("test_message"+str(i)+": db 'Hello "+str(i)+"', 13, 0\n")

	fw.close()


