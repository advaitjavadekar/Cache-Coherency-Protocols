/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	main.h
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
#ifndef MAIN_H
#define MAIN_H
#include "cache.h"
#include "io_function.h"
//
// Look up table to print the protocol names 
void print_input_format();
void create_processor_cache(int np, int s, int a, int b, int p);
int sharers_exclude(ulong addr, int proc_no);
int c2c_supplier(ulong addr, int proc_no);
int sharers(ulong addr);
void simulate_caches(char *fname);
void sendBusRd(ulong addr, int proc_no);
void sendBusWr(ulong addr, int proc_no);
void sendBusRdX(ulong addr, int proc_no);
void sendBusUpgr(ulong addr, int proc_no);
void print_dir();
//
#endif
