#include "error.h"

Error::Error()		{	this->errMsg = "Fatal error";	}
Error::Error(string s)	{	this->errMsg = s;	}
string Error::getMsg()	{	return this->errMsg;	}

/*** TEST ERROR
static void f(int);
int main()
{
	try
	{
		f(-1);	cout << "f(-1)" << endl;
		f(0);	cout << "f(0)" << endl;
		f(1);	cout << "f(1)" << endl;
	}
	catch(Error e)
	{
		cout << "Error msg is: "<< e.getMsg() << endl;
	}
	return 0;
}

void f(int i)
{
	if (i==0)
		throw Error("i=0");
	else
		return;	
}
***/
