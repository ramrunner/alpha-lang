#ifndef __ACTIVEREC_HEADER
#define __ACTIVEREC_HEADER

#include "general.h"
#include "mem.h"
#include "cpu.h"

class ActivationRecord
{
	friend class CPU;
	public:
			ActivationRecord();
			ActivationRecord(int localNO,int formalNO);
			void activate();
			void deactivate();
			
			bool getState()	{ return this->state; }			

			void setLocal(int n) { this->localNO = n; }
			void setFormal(int n){ this->formalNO= n; }
			void setRetAddress(int n)	{ this->retAddress = n;  }

			int getLocal()	{ return this->localNO;	}
			int getFormal()	{ return this->formalNO;}
			int getRetAddress()		{ return this->retAddress;  }
	protected:
			int localNO, formalNO;
			int retAddress;
			bool state;	

	private:
};



#endif
