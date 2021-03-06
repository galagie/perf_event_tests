#include <stdio.h>
#include <stdlib.h>

#include "perf_event.h"
#include "branches_testcode.h"


/* This code has 1,500,000 total branches       */
/*                 500,000 not-taken branches   */
/*               1,000,000 taken branches       */
/*               XXXX    conditional branches */
int branches_testcode(void) {

#if defined(__i386__) || (defined __x86_64__)
	asm(	"\txor %%ecx,%%ecx\n"
		"\tmov $500000,%%ecx\n"
		"test_loop:\n"
		"\tjmp test_jmp\n"
		"\tnop\n"
		"test_jmp:\n"
		"\txor %%eax,%%eax\n"
		"\tjnz test_jmp2\n"
		"\tinc %%eax\n"
		"test_jmp2:\n"
		"\tdec %%ecx\n"
		"\tjnz test_loop\n"
		: /* no output registers */
		: /* no inputs */
		: "cc", "%ecx", "%eax" /* clobbered */
	);
    	return 0;

#elif defined(__arm__)
    /* Initial code contributed by sam wang linux.swang _at_ gmail.com */

	asm(	"\teor r3,r3,r3\n"
		"\tldr r3,=500000\n"
	    	"test_loop:\n"
		"\tB test_jmp\n"
		"\tnop\n"
		"test_jmp:\n"
		"\teor r2,r2,r2\n"
		"\tcmp r2,#1\n"
		"\tbge test_jmp2\n"	
		"\tnop\n"
		"\tadd r2,r2,#1\n"
		"test_jmp2:\n"
		"\tsub r3,r3,#1\n"
		"\tcmp r3,#1\n"
		"\tbgt test_loop\n"
		: /* no output registers */
		: /* no inputs		 */
		: "cc", "r2", "r3" /* clobbered */
	);

	return 0;
#endif

    return -1;

}


int random_branches_testcode(int number, int quiet) {

  int j,junk=0;
  double junk2=5.0;

   for(j=0;j<number;j++) {
	
	if (( ((random()>>2)^(random()>>4)) %1000)>500) goto label_false;

	junk++;   /* can't just add, the optimizer is way too clever */

	junk2*=junk;

	//printf("T");
      label_false:
	//printf("F");
	;
      }
   if (!quiet) printf("%lf\n",junk2);

   return junk;
}

