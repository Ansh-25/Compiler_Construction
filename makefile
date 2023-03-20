
# ID: 2020A7PS0035P | Name: Shreekar Puranik
# ID: 2020A7PS1209P | Name: Sriram Ramanathan
# ID: 2020A7PS1205P | Name: Nikhil Pradhan
# ID: 2020A7PS0146P | Name: Toshit Jain
# ID: 2020A7PS0116P | Name: Ansh Gupta

stage1.exe: driver.c lexer.c parser.c
	gcc driver.c lexer.c parser.c -o stage1.exe

parser.c: parserDef.h lexer.h
	gcc -c parser.c

lexer.c: lexerDef.h hash.h
	gcc -c lexer.c

driver.c: lexer.h parser.h
	gcc -c driver.c

clean: 
	rm -v *.o stage1.exe commentFreeFile.txt 