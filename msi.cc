/************************************************************
			Course 		: 	CSC456
			Source 		:	msi.cc
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/ 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "main.h"
#include "msi.h"

void MSI::PrRd(ulong addr, int processor_number) {
    cache_state state;
	// Per-cache global counter to maintain LRU order among 
	// cache ways, updated on every cache access
    current_cycle++; 
    reads++;
    cache_line * line = find_line(addr);
       if (line == NULL) { 
		// This is a miss 
             read_misses++;
             cache_line *newline = allocate_line(addr);
             memory_transactions++;
		// State I --> S
		I2S++; 
             newline->set_state(S);
		// Read miss --> BusRd		
             bus_reads++;
             sendBusRd(addr, processor_number);
       }
      else { 
		// The block is cached
	     state=line->get_state();
             if (state == I){
			// The block is cached, but in invalid state.
			// Hence Read miss
			read_misses++;
                        cache_line *newline = allocate_line(addr);
                        memory_transactions++;
			I2S++; 
			newline->set_state(S);
			bus_reads++;
			sendBusRd(addr, processor_number);
		}
             else if (state == M || state == S){
                update_LRU(line);
                }  
             }                            
}

void MSI::PrWr(ulong addr, int processor_number) {
    cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
    if (line == NULL || line->get_state() == I){ 
		/* This is a miss */
		write_misses++;
                cache_line *newline = allocate_line(addr); 		
                memory_transactions++;
		I2M++;
                newline->set_state(M);
		bus_readxs++;
		sendBusRdX(addr, processor_number);
     }
    else 
	{
		state=line->get_state();
                if (state == M){
                       
                /* Since it's a hit, update LRU and update state */
		update_LRU(line);
                }
                else if (state == S){
		S2M++;
                line->set_state(M);
                update_LRU(line);
                bus_readxs++;
                memory_transactions++;
		sendBusRdX(addr, processor_number);
                }
         }
}

//cache_line * MSI::allocate_line(ulong addr) { 

void MSI::BusRd(ulong addr) {
    cache_state state;    
    cache_line * line=find_line(addr);
    if (line!=NULL) 
       { 		
		state = line->get_state();
		if (state == M)
                    {
                      flushes++;
		      write_backs++;
                      memory_transactions++;
		      line->set_state(S);
		      M2S++;
                    }
	}
}

void MSI::BusRdX(ulong addr) {
    cache_line * line=find_line(addr);
    if (line!=NULL)	{ 
		cache_state state;
		state=line->get_state();
                if (state == S )
                {
		invalidations++;
                line->set_state(I);
                }
                else if (state == M )
                {
		flushes++;
		invalidations++;
                write_backs++;
                memory_transactions++;
                line->set_state(I);
                }
	}
}

bool MSI::writeback_needed(cache_state state) {
    if (state == M)
    {
    	return true;
    } else {
	return false;
    }
}
