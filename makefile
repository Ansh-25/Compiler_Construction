
test: driver.exe 
	driver.exe ./test_cases/t2.txt testOutput 32

driver.exe: driver.c lexer.c parse.c
	gcc driver.c lexer.c parse.c -o driver.exe

parse.c: parseDef.h lexer.h
	gcc -c parse.c

lexer.c: lexerDef.h hash.h
	gcc -c lexer.c

driver.c: lexer.h parse.h
	gcc -c driver.c

clean: 
	rm -v *.o *.out driver.exe commentFreeFile.txt