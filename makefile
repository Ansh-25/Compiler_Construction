
# ID: 2020A7PS0035P | Name: Shreekar Puranik
# ID: 2020A7PS1209P | Name: Sriram Ramanathan
# ID: 2020A7PS1205P | Name: Nikhil Pradhan
# ID: 2020A7PS0146P | Name: Toshit Jain
# ID: 2020A7PS0116P | Name: Ansh Gupta

compiler.exe: driver.c ast.c lexer.c parser.c typeChecker.c hash.c
	gcc driver.c lexer.c parser.c hash.c ast.c typeChecker.c -o compiler.exe

cleanw:
	del *.o *.exe