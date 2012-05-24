CC = g++
SRC = src/
INC = include/
OBJ = lib/
COPTS = -g -I $(INC)

../avm:				$(OBJ)/avm.o $(OBJ)/avmClass.o $(OBJ)/memCell.o $(OBJ)/mem.o $(OBJ)/instruction.o $(OBJ)/cpu.o $(OBJ)/error.o $(OBJ)/table.o $(OBJ)/activationRecord.o
				$(CC) $(COPTS) $(OBJ)/avm.o $(OBJ)/avmClass.o $(OBJ)/mem.o $(OBJ)/instruction.o $(OBJ)/memCell.o $(OBJ)/cpu.o $(OBJ)/error.o $(OBJ)/table.o $(OBJ)/activationRecord.o -o ../avm

$(OBJ)/avm.o:			$(SRC)/avm.cpp $(INC)/avm.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/avm.cpp -o $(OBJ)/avm.o

$(OBJ)/avmClass.o:		$(SRC)/avmClass.cpp $(INC)/avmClass.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/avmClass.cpp -o $(OBJ)/avmClass.o

$(OBJ)/mem.o:			$(SRC)/mem.cpp $(INC)/mem.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/mem.cpp -o $(OBJ)/mem.o

$(OBJ)/memCell.o:		$(SRC)/memCell.cpp $(INC)/memCell.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/memCell.cpp -o $(OBJ)/memCell.o

$(OBJ)/instruction.o:		$(SRC)/instruction.cpp $(INC)/instruction.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/instruction.cpp -o $(OBJ)/instruction.o

$(OBJ)/cpu.o:			$(SRC)/cpu.cpp $(INC)/cpu.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/cpu.cpp -o $(OBJ)/cpu.o

$(OBJ)/error.o:			$(SRC)/error.cpp $(INC)/error.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/error.cpp -o $(OBJ)/error.o

$(OBJ)/table.o:			$(SRC)/table.cpp $(INC)/memCell.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/table.cpp -o $(OBJ)/table.o

$(OBJ)/activationRecord.o:	$(SRC)/activationRecord.cpp $(INC)/activationRecord.h $(INC)/general.h
				$(CC) $(COPTS) -c $(SRC)/activationRecord.cpp -o $(OBJ)/activationRecord.o



clean:
	rm -f $(OBJ)/*.o
	rm -f ../avm
	rm -f core*

