/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	io_function.h
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/ 
#ifndef IO_FUNCTION_H
#define IO_FUNCTION_H
int sharers(ulong addr);
void print_sim_stats(	int cache_size, int cache_assoc,
						int blk_size, int num_processors,
						const char ** protocol_name,
						int protocol, char *fname);
void print_sim_results(int np,Cache** processor_cache,int protocol);
#endif
