%option outfile="src/lexCode.c"
%option noyywrap
%option yylineno
%{
	#include "lexhead.h"
%}

ESC_CHAR		\\[a-zA-Z\\\"\^]
CL_COM			*/

%%


"if"			return keyIF;
"else"			return keyELSE;
"while"			return keyWHILE;
"for"			return keyFOR;
"function"		return keyFUNCT;
"return"		return keyRETURN;
"break"			return keyBREAK;
"continue"		return keyCONTIN;
"local"			return keyLOCAL;
"true"			return keyTRUE;
"false"			return keyFALSE;
"nil"			return keyNIL;


"{"		return semLCURLBRACE;
"}"		return semRCURLBRACE;
"["		return semLBRACE;
"]"		return semRBRACE;
"."		return semDOT;
"("		return semLPAR;
")"		return semRPAR;
";"		return semCOLON;
","		return semCOMMA;
"::"		return sem2DDOT;
":"		return semDDOT;

"="		return opASSIGN;
"+"		return opPLUS;
"-"		return opMINUS;
"*"		return opMULTI;
"/"		return opDIV;
"%"		return opMOD;
"=="		return opEQ;
"!="		return opNEQ;
"||"		return opOR;
"&&"		return opAND;
"!"		return opNOT;
"++"		return opINC;
"--"		return opDEC;
">"		return opGT;
"<"		return opLT;
">="		return opGTE;
"<="		return opLTE;

[0-9]+				yylval.intValue=(int)strtol(yytext,(char **) NULL,10);return constINT;
[0-9]+"."[0-9]*			yylval.realValue=strtod(yytext,(char **)NULL);return constREAL;
\"({ESC_CHAR}|[^\\\"\n])*\"	yylval.stringValue=createCopy(&(yytext[1]),yyleng-2);return typeSTRING;
[a-zA-Z][a-zA-Z0-9_]*		yylval.stringValue=createCopy(yytext,yyleng); return typeIDENTIFIER;
"//".*				//return typeCOMMENT;
"/*"([^*/])*"*/"		//return typeCOMMENT;


[ \t\n]*			
.				{
					printf("ERR: Unrecognised token: \"%s\"\n", yytext);
					return 0;	// Epistrefontas 0 kanoume emulate to
							// EOF ka8ws i yylex() epistrefei 0
							// otan brei to EOF.
				}			


%%

/* PROSOXI:
 * H yylex() epistrefei 0 otan brei EOF. Epeidi omws keyIF = 0, 
 * 8a epistrefoume to tokenID au3imeno kata 1. An i yylex()
 * epistrepsei 0 tote exoume EOF alliws exoume ID kai to
 * epanaferoume stin kanoniki tou timi (tokID--;)
 */
/*int alpha_yylex(void *ylval)	// ylval -> atoken **
{
	int tokID = yylex();
	void *data;

	if (tokID == 0)
		return EOF;
	else
		tokID--;
	if ( HAS_STRING(tokID) )
	{
		data = NULL;
	}
	else
	{
		data = createCopy(yytext, yyleng);
	}
	*((atoken **)ylval) = create_token(tokID, data);
	return tokID;
}
*/
