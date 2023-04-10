
# ID: 2020A7PS0035P | Name: Shreekar Puranik
# ID: 2020A7PS1209P | Name: Sriram Ramanathan
# ID: 2020A7PS1205P | Name: Nikhil Pradhan
# ID: 2020A7PS0146P | Name: Toshit Jain
# ID: 2020A7PS0116P | Name: Ansh Gupta

run2: ast.exe
	./ast.exe

ast.exe: ast.c lexer.c parser.c typeChecker.c hash.c
	gcc ast.c lexer.c parser.c typeChecker.c hash.c -o ast.exe

run1: stage1.exe
	stage1.exe testcase7.txt testOutput.txt 32

stage1.exe: driver.c lexer.c parser.c
	gcc driver.c lexer.c parser.c hash.c -o stage1.exe

# typeChecker.c: typeChecker.h
# 	gcc -c typeChecker.c

# parser.c: parserDef.h lexer.h
# 	gcc -c parser.c

# lexer.c: lexerDef.h hash.h
# 	gcc -c lexer.c

# hash.c: hash.h
# 	gcc -c hash.c

# driver.c: lexer.h parser.h
# 	gcc -c driver.c

cleanw:
	del *.o *.exe

clean: 
	rm -v *.o stage1.exe commentFreeFile.txt 

commit:
	git commit -am "."
	git push origin main