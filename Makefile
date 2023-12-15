CC          := gcc
SRC         := ./src
BIN         := ./bin
OBJ			:= ./obj
BINNAME     := bin
HEADERS     := ./headers
EXEC        := prog.exe
ZIP         := exercicio.zip

all:
	$(CC) -Wall -Werror -c $(SRC)/funcoesFornecidas.c -I $(HEADERS) -o $(OBJ)/funcoesFornecidas.o
	$(CC) -Wall -Werror -c $(SRC)/funcoesAuxiliares.c -I $(HEADERS) -o $(OBJ)/funcoesAuxiliares.o
	$(CC) -Wall -Werror -c $(SRC)/funcionalidades.c -I $(HEADERS) -o $(OBJ)/funcionalidades.o
	$(CC) $(SRC)/main.c $(OBJ)/*.o -I $(HEADERS) -o $(BIN)/$(EXEC)

run:
	$(BIN)/$(EXEC)

clean_Win:
	del $(BIN)\* $(OBJ)\*.o $(ZIP)

clean_Linux:
	rm -f $(BIN)/*.exe $(OBJ)/*.o $(ZIP)

compile: clean reset all

crun : compile run

zip:
	rm -f $(ZIP)
	zip $(ZIP) Makefile $(HEADERS)/*.h $(BIN)/*.exe $(OBJ)/*.o $(SRC)/*.c ./*.bin ./*.csv