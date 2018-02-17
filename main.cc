/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	main.cc
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	Version	|	Programmer		|	Change Description		
------------------------------------------------------------
		1.0	|	Ahmad Samih & 	|
			|	Yan Solihin		|	Initial Version
		2.0	|	Ed Gehringer	|	Revised for Spring 2015
			|					|	CSC/ECE506
------------------------------------------------------------
	© Please do not replicate this code without consulting 	
		the owner & instructor! Thanks!						
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <fstream>
using namespace std;
// Cache header file which contains cache super class and 
// related methods
#include "cache.h"			
#include "main.h"
// Input-Output Header File to print simulator statistics
#include "io_function.h"	

#define INVALIDATE 1
const int max_processors = 16;
int num_processors;
int blk_size,protocol;
Cache* processor_cache[max_processors];
const char* protocol_names[ ] = {	"MSI",
									"MOSI",
									"MOESI",
								   	};

//
//****************************************************************//
//*                        Main Function                         *//
//****************************************************************//

int main(int argc, char *argv[]){
	//*** accept the simulator parameters from the command prompt ****//
    if (argv[1] == NULL){
	print_input_format();
	exit(0);
    }

//***** Move the parameters to the local variables *****//
    int cache_size 		= atoi(argv[1]);
    int cache_assoc		= atoi(argv[2]);
    blk_size	   		= atoi(argv[3]);
    num_processors 		= atoi(argv[4]);
    protocol			= atoi(argv[5]);	
	char *fname 		= (char *)malloc(20);
    fname 				= argv[6];
//*******Print out simulator configuration here*******//
	print_sim_stats(cache_size, cache_assoc, blk_size,
					num_processors,	protocol_names,
					protocol, fname);
//*******Create an array of caches here*********//
	create_processor_cache(num_processors, cache_size,
						   cache_assoc, blk_size, protocol);
//********Process the trace file and simulate the cache*********//
	simulate_caches(fname);
//********Print out final statistics for all the processor caches **//
	print_sim_results(num_processors, processor_cache, protocol);
}
//
//******************************************************************//
//*	Supporting methods for the main function                       *//
//******************************************************************//
//
void print_input_format(){
	printf("input format: ");
	printf("./simulate_cache <cache_size> <assoc> <block_size> \n");
	printf(" <num_processors> <protocol> <trace_file> \n");
	printf(" *--------------------------------------*\n");
	printf(" *\t Protocol Lookup Table \t\t*\n");
	printf(" *--------------------------------------*\n");
	printf(" * Protocol Number \t Protocol Name \t*\n");
	printf(" *\t 0 \t\t MSI \t*\n");
    printf(" *\t 1 \t\t MOSI \t*\n");
	printf(" *\t 2 \t\t MOESI \t*\n");
	printf(" *--------------------------------------*\n");
}
//
//**	This function is used to create cache for all the processors 
//
void create_processor_cache(int num_processors, int size,
							int assoc, int blk_size,
							int protocol){
    int i;
	// create cache for each processor based on 
	// the protocol using abstract factory pattern
	for (i=0;i<num_processors;i++)
		processor_cache[i] = cache_factory::create_cache(protocol, size, 
														assoc, blk_size,
														num_processors, i);
}
//
//	This function performs the following			 	
//	[1] Opens the file to be read and reports an error if open fails
// 	[2]	Reads the trace file,line by line, each 
//  (processor#, operation, address) propagate each request down 
//	through memory hierarchy by calling cachesArray[processor#]->Access(...)	    
//
void simulate_caches(char *fname){
    ifstream fin;
    FILE * pFile;
	int proc_no, addr;
    char op;
	
	// Print error if the file cannot be found or opened
    pFile = fopen (fname,"r");
    if (pFile == 0)	{	
		printf("Trace file problem\n");
		exit(0);
    }
   
	// Read each entry from the trace file and perform 
	// the relevant operation (read/write) 
	// based on the input
    while (fscanf(pFile, "%d %c %x", &proc_no, &op, &addr) != EOF) {
	  if (op == 'w')
		processor_cache[proc_no]->PrWr((ulong) addr, proc_no);
	  else 
		processor_cache[proc_no]->PrRd((ulong) addr, proc_no);
    }
    fclose(pFile);
}
//
//	Return the number of sharers of the block
//
int sharers(ulong addr) {
	int count = 0;
	for (int i = 0; i < num_processors; i++){
		if (processor_cache[i]->find_line(addr) != NULL) 
			count++;
	}
	return count;
}

//
//	Return the number of sharers of the block other than the processor calling this fucntion
//
int sharers_exclude(ulong addr, int proc_no) {
	int count = 0;
	for (int i = 0; i < num_processors; i++){
		if (i!=proc_no) {
		  if (processor_cache[i]->find_line(addr)!= NULL) {
			if (processor_cache[i]->find_line(addr)->get_state()!=I) {
			count++;
			}
		  }
		}
	}
	return count;
}


//This function should tell if the cache block can be supplied by another cache; Note that in case of MSI/MOSI/MOESI, if all the processors are in S state or I state, the cache block has to come from main Memory. Only, M or O act as suppliers of cache blocks.
int c2c_supplier(ulong addr, int proc_no) {
	int count = 0;
	for (int i = 0; i < num_processors; i++){
		if (i!=proc_no) {
			//If in M or O or E state, then block will be passed from processor to processor instead of memory fetch
			if (processor_cache[i]->find_line(addr)!= NULL) {
			if ((processor_cache[i]->find_line(addr)->get_state()==M) || (processor_cache[i]->find_line(addr)->get_state()==O)
				|| (processor_cache[i]->find_line(addr)->get_state()==E)) {
			count++;
			} 
			}
		}
	}
	return count;
}

void sendBusRd(ulong addr, int proc_no){
		for (int i=0; i<num_processors; i++){
			if (i!=proc_no)
				processor_cache[i]->BusRd((ulong) addr);
		}
}

void sendBusWr(ulong addr, int proc_no){
		for (int i=0; i<num_processors; i++){
			if (i!=proc_no)
				processor_cache[i]->BusWr((ulong) addr);
		}
}

void sendBusRdX(ulong addr, int proc_no){
		for (int i=0; i<num_processors; i++){
			if (i!=proc_no)
				processor_cache[i]->BusRdX((ulong) addr);
		}
}

void sendBusUpgr(ulong addr, int proc_no){
		for (int i=0; i<num_processors; i++){
			if (i!=proc_no)
				processor_cache[i]->BusUpgr((ulong) addr);
		}
}
