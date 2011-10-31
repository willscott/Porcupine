import quills
import sys
usage = 'python porcumommy.py <seed>'
dest_filename = 'porcubaby.asm'

if __name__ == '__main__':
	if (len(sys.argv) < 2):
		print usage
		sys.exit(-1)
	seed = int(sys.argv[1])
	print 'Generating porcupine baremetal assembly with seed=' + str(seed)
	
	#Open and write headers
	fw = open(dest_filename, 'w')
	fw.write('[BITS 64]\n[ORG 0x0000000000200000]\n\n')
	fw.write('%include "bmdev.asm"\n\n')

	#Allocate some fixed address memory
	fw.write('SECTION .bss\n')
	fw.write('s_ebp:\tresd\t1;\n')
	fw.write('s_esp:\tresd\t1;\n')
	fw.write('SECTION .text\n')
	
	#Write start point
	fw.write('start:\n')
	fw.write('\tjmp asm0\n\n')

	#Generate assembly sequence
	fw.write('asm0:\n')
	fw.write('\tmov [s_ebp],ebp\n');
	fw.write('\tmov [s_esp],esp\n');
	fw.write('\t' + quills.render(seed, 100) + '\n')
	fw.write('\tmov ebp,[s_ebp]\n');
	fw.write('\tmov esp,[s_esp]\n');
	
	#Return to OS
	fw.write('\tret\n')

	fw.close()


