/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	msi.h
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/ 

class MSI : public Cache{
 public:
	MSI(int s, int a, int b, int p, int pn):
		Cache(s, a, b, p, pn) {};
	~MSI() {};
	void PrRd(ulong addr, int processor_number);
	void PrWr(ulong addr, int processor_number);
	bool writeback_needed(cache_state state);
	void BusRd(ulong addr);
	void BusRdX(ulong addr);
};
