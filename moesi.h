/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	msi.h
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/ 

class MOESI : public Cache{
 public:
	MOESI(int s, int a, int b, int p, int pn):
		Cache(s, a, b, p, pn) {};
	~MOESI() {};
	void PrRd(ulong addr, int processor_number);
	void PrWr(ulong addr, int processor_number);
	void BusRd(ulong addr);
        bool writeback_needed(cache_state state);
	void BusRdX(ulong addr);
	void BusUpgr(ulong addr);
};
