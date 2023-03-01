
test: driver.exe 
	./driver.exe ./test_cases/t5.txt testOutput 32

driver.exe: driver.c lexer.c
	gcc driver.c lexer.c -o driver.exe

clean: 
	rm -v *.o *.out driver.exe commentFreeFile.txt