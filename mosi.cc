/************************************************************
			Course 		: 	CSC456
			Source 		:	msi.cc
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	Â© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/ 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "main.h"
#include "cache.h"
#include "mosi.h"


void MOSI::PrRd(ulong addr, int processor_number) {
    cache_state state;
	// Per-cache global counter to maintain LRU order among 
	// cache ways, updated on every cache access
    current_cycle++; 
    reads++;
    cache_line * line = find_line(addr);
       if (line == NULL || line->get_state() == I) { 
		// This is a miss 
             read_misses++;
             cache_line *newline = allocate_line(addr);
			 if (c2c_supplier(addr,processor_number)== 0)
				 memory_transactions++;
			 else 
				 cache2cache ++;
		// State I --> S
			 I2S++; 
             newline->set_state(S);
		// Read miss --> BusRd		
             bus_reads++;
             sendBusRd(addr, processor_number);
       }
	   
        else{
			state=line->get_state();
		    if (state == M || state == S || state== O){
				update_LRU(line);
                }  
       }
}
                            

void MOSI::PrWr(ulong addr, int processor_number) {
    cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
    if (line == NULL || line->get_state() == I){ 
		/* This is a miss */
		write_misses++;
        cache_line *newline = allocate_line(addr); 
             if (c2c_supplier(addr,processor_number)== 0)
				 memory_transactions++;
			 else 
				 cache2cache ++;				
             I2M++;
             newline->set_state(M);
		bus_readxs++;
		sendBusRdX(addr, processor_number);
     }
    else 
	{
		state=line->get_state();
                if (state == M){
                update_LRU(line);
                }
                else if(state == O){
                O2M++;
                line->set_state(M);
                update_LRU(line);
                bus_upgrades++;
		        sendBusUpgr(addr, processor_number);
                } 
                else if (state == S){
		        S2M++;
                line->set_state(M);
                update_LRU(line);
                bus_upgrades++;
		        sendBusUpgr(addr, processor_number);
                }
         }
} 


void MOSI::BusRd(ulong addr) {
    cache_state state;    
    cache_line * line=find_line(addr);
    if (line!=NULL) 
       { 		
		state = line->get_state();
		if (state == M)
                    {
                     flushes++;
					 interventions++;
		             line->set_state(O);
		             M2O++;
                    }
		else if (state == O)
			{ 
				flushes++; 
				
			}               
	}
} 

void MOSI::BusRdX(ulong addr) {
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
                line->set_state(I);
                }
                else if (state == O){
                flushes++;
                invalidations++;
                line->set_state(I);                
                }
	}
} 

void MOSI::BusUpgr(ulong addr) {
    cache_line * line=find_line(addr);
    if (line!=NULL)	{ 
		cache_state state;
		state=line->get_state();
                if (state == S )
                {
		        invalidations++;
                line->set_state(I);
                }
                else if (state == O )
                {
	            invalidations++;
                line->set_state(I);
                }
	}
    
}

bool MOSI::writeback_needed(cache_state state) {
	if (state == M||state == O)
    {
    	return true;
    }
    else 
    {
	return false;
    }
	
}
