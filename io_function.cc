/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	io_function.cc
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <fstream>
using namespace std;
#include "cache.h"
#include "io_function.h"
//	This function is used to print simulator configurations
void print_sim_stats(int cache_size, int cache_assoc, 
					int blk_size, int num_processors, 
					const char **protocol_names, int protocol, 
					char *fname){
	printf("===== SP15 506 BUS BASED CACHE Configuration =====\n");
    printf("L1_SIZE:\t\t%d\n", cache_size);
    printf("L1_ASSOC:\t\t%d\n", cache_assoc);
    printf("L1_BLOCKSIZE:\t\t%d\n", blk_size);
    printf("NUMBER OF PROCESSORS:\t%d\n", num_processors);
    printf("COHERENCE PROTOCOL:\t%s \n", protocol_names[protocol]);
    printf("TRACE FILE:\t\t%s\n", fname);
}
// Print the final statistics for each processor cache
void print_sim_results(int num_processors,
						Cache** processor_cache,
						int protocol){
    for (int i=0; i<num_processors; i++)
    {
		processor_cache[i]->print_stats(i,protocol);
    }
}
