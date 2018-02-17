/************************************************************
			Course 		: 	CSC/ECE506
			Source 		:	msiup.h
			Instructor	:	Ed Gehringer
			Email Id	:	efg@ncsu.edu
------------------------------------------------------------
	© Please do not replicate this code without consulting
		the owner & instructor! Thanks!
*************************************************************/  
class MOSI : public Cache {
 public:
	MOSI(int s, int a, int b, int p , int pn)
				: Cache(s, a, b, p, pn) {};
	~MOSI() {};
	void PrRd(ulong addr, int processorNumber);
	void PrWr(ulong addr, int processorNumber);
	void BusRd(ulong addr);
	bool writeback_needed(cache_state state);
	void BusUpgr(ulong addr);
	void BusRdX(ulong addr);
};
