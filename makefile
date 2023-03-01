
test: driver.exe 
	./driver.exe Program.txt testOutput 32

driver.exe: driver.o lexer.o
	gcc driver.o lexer.o -o driver.exe

driver.o: driver.c lexer.h lexerDef.h
	gcc -c driver.c

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c

clean: 
	rm -v *.o *.out driver.exe commentFreeFile.txt