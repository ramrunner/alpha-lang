#include "idgen.h"

static int fcounter = 0;
static int tmpCounter = 0;

static const char nullC = '\0';

char* generateID()
{
        char* retValue;
        fcounter++;
        retValue = (char *)malloc( sizeof(char)*((fcounter % 10)+11) );
        snprintf(retValue, sizeof(char)*((fcounter%10)+11), "_%dsKrF%c", fcounter, nullC);
        return retValue;
}

char *newTmp()
{
	char *retValue;
	tmpCounter++;
	retValue = (char *)malloc(sizeof(char)*((tmpCounter % 10)+7));
	snprintf(retValue, sizeof(char)*((tmpCounter % 10)+7), "_tmp%d%c", tmpCounter, nullC);
	return retValue;
}

void resetTmp()
{
	tmpCounter = 0;
}
					
