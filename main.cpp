
#include "Program.h"

//-----------------------------------------------------------------------------
int main (int argc, char ** argv)
{
	if(argc != ARGS_NUM) { return -1; }
	
	intro(argv[1]);
	Program mmcsa;
	if(!mmcsa.readInput(argv[1])) { return -1; }

	mmcsa.processMatrices();
	mmcsa.buildTree(argv[2]);
	results(argv[2]);
	
	getchar();
	return 0;
}