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
#include "moesi.h"

void MOESI::PrRd(ulong addr, int processor_number) {
	cache_state state;
	current_cycle++;
	reads++;
	cache_line * line = find_line(addr);
	if (line == NULL || line->get_state()==I) { 
		// This is a miss 
             read_misses++;
             cache_line *newline = allocate_line(addr);
			 //check whether same block present in E,O or M state in another cache
             if (c2c_supplier(addr,processor_number)==0) memory_transactions++;
			 else cache2cache ++;
		if (sharers_exclude(addr, processor_number)==0){	//check whether copies exist
			// State I --> E
			I2E++;
			newline->set_state(E);
		}
		else{
			// State I --> S
			I2S++; 
            newline->set_state(S);
		} 
		// Read miss --> BusRd		
             bus_reads++;
             sendBusRd(addr, processor_number);
       }
	
	//For states other than I
	else { 
		// The block is cached
	    state=line->get_state();
        if (state == M || state == S || state==E || state==O){
				update_LRU(line);
				//std::cout<<"block present"<<endl;
            }  
    }
} 

void MOESI::PrWr(ulong addr, int processor_number) {
	cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
	//if iinvalid, go to state M
    if (line == NULL || line->get_state() == I){ 
		/* This is a miss */
		write_misses++;
        cache_line *newline = allocate_line(addr); 
		//check whether same block present in E,O or M state in another cache		
        if (c2c_supplier(addr,processor_number)==0) memory_transactions++;
		else cache2cache ++;
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
                bus_upgrades++;
                sendBusUpgr(addr, processor_number);
                }
				else if (state == E){
				E2M++;
				line->set_state(M);
				update_LRU(line);
				}
				else if (state == O){
				O2M++;
				line->set_state(M);
				update_LRU(line);
				bus_upgrades++;
				sendBusUpgr(addr, processor_number);
				}
         }
} 


void MOESI::BusRd(ulong addr) {
	cache_state state;    
    cache_line * line=find_line(addr);
    if (line!=NULL){
		state = line->get_state();
		if (state == E){
            line->set_state(S);
			E2S++;
        }
		else if (state == M){
			line->set_state(O);
			M2O++;
			flushes++;			//if in M, flush to bus and go to state O
			interventions++;
		}
		else if (state == O) flushes++;		//if in O, flush to bus
	
	}
} 

void MOESI::BusRdX(ulong addr) {
	cache_state state;    
    cache_line * line=find_line(addr);
    if (line!=NULL){
		state = line->get_state();
		if (state == E){
			line->set_state(I);
			invalidations++;	//if in E, simply invalidate
		}
		else if (state == S){
			line->set_state(I);
			invalidations++;	//if in S, simply invalidate
        }
		else if (state == M){
			line->set_state(I);
			invalidations++;
			flushes++;			//if in M, invalidate and flush to bus
		}
		else if (state == O) {
			line->set_state(I);
			invalidations++;
			flushes++;			//if in M, invalidate and flush to bus
		}
	}
} 

void MOESI::BusUpgr(ulong addr) {
	cache_state state;    
    cache_line * line=find_line(addr);
    if (line!=NULL){
		state = line->get_state();
		if (state == S){
            line->set_state(I);
			invalidations++;
		}
		else if (state == O){
			line->set_state(I);
			invalidations++;
		}
	}
}	

bool MOESI::writeback_needed(cache_state state) {		//only when block in unique, i.e. in M or O state
	if (state == M || state== O) {return true;}
	else {return false;}
}
