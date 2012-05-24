/* error.h
 * It is READY
 */

#ifndef __ERROR_HEADER
#define __ERROR_HEADER

#include "general.h"

class Error
{
		public:
				Error();
				Error(string);
				string getMsg();
		protected:
				string errMsg;
};

#endif
