CC = gcc
SRCDIR = src
INCDIR = include
OBJDIR = lib
CCOPTS = -g -I $(INCDIR)
	## PROSOXI: Oi kanones yak.tab.o, lexCode.o exoun epipleon flag -w wste na MIN bgazoun warnings

all:		ac

ac:		$(OBJDIR)/lexCode.o $(OBJDIR)/yak.tab.o $(OBJDIR)/symFunc.o $(OBJDIR)/symPrint.o $(OBJDIR)/quad.o $(OBJDIR)/expr.o $(OBJDIR)/stack.o $(OBJDIR)/finalCode.o $(OBJDIR)/idgen.o
		$(CC) $(CCOPTS) $(OBJDIR)/expr.o $(OBJDIR)/quad.o $(OBJDIR)/idgen.o $(OBJDIR)/symPrint.o $(OBJDIR)/finalCode.o $(OBJDIR)/lexCode.o $(OBJDIR)/yak.tab.o $(OBJDIR)/symFunc.o $(OBJDIR)/stack.o -o ../ac

$(OBJDIR)/lexCode.o:	$(OBJDIR)/yak.tab.o $(SRCDIR)/lexCode.lex $(INCDIR)/lexhead.h
			flex $(SRCDIR)/lexCode.lex
			$(CC) $(CCOPTS) -w -c $(SRCDIR)/lexCode.c -o $(OBJDIR)/lexCode.o

$(OBJDIR)/yak.tab.o: 	$(SRCDIR)/yak.y
			/opt/local/bin/bison -v --defines=$(INCDIR)/yak.tab.h --output=$(SRCDIR)/yak.tab.c $(SRCDIR)/yak.y
			$(CC) $(CCOPTS) -w -c $(SRCDIR)/yak.tab.c -o $(OBJDIR)/yak.tab.o

$(OBJDIR)/symFunc.o:	$(SRCDIR)/symFunc.c $(INCDIR)/symHeader.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/symFunc.c -o $(OBJDIR)/symFunc.o

$(OBJDIR)/idgen.o: 	$(SRCDIR)/idgen.c $(INCDIR)/idgen.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/idgen.c -o $(OBJDIR)/idgen.o

$(OBJDIR)/symPrint.o: 	$(SRCDIR)/symPrint.c $(INCDIR)/symHeader.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/symPrint.c -o $(OBJDIR)/symPrint.o

$(OBJDIR)/quad.o:	$(SRCDIR)/quad.c $(INCDIR)/quad.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/quad.c -o $(OBJDIR)/quad.o

$(OBJDIR)/expr.o:	$(SRCDIR)/expr.c $(INCDIR)/expr.h
				$(CC) $(CCOPTS) -c $(SRCDIR)/expr.c -o $(OBJDIR)/expr.o

$(OBJDIR)/stack.o:	$(SRCDIR)/stack.c $(INCDIR)/stack.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/stack.c -o $(OBJDIR)/stack.o

$(OBJDIR)/finalCode.o:	$(SRCDIR)/finalCode.c $(INCDIR)/finalCode.h
			$(CC) $(CCOPTS) -c $(SRCDIR)/finalCode.c -o $(OBJDIR)/finalCode.o

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(SRCDIR)/lexCode.c $(SRCDIR)/yak.output
	rm -f $(SRCDIR)/yak.tab.c
	rm -f $(INCDIR)/yak.tab.h
	rm -f ../core*
	rm -f ../ac.core*
	rm -f ../ac

