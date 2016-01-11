#include "converter.h"

int main(int argc,char* argv[]){
	
	converter cc;
	cc.load_file(argv[1]);
	cc.process_file();
	return 0;
}
