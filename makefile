nguyenLouisA1: nguyenLouisA1.o nguyenLouisA1Main.o
	gcc nguyenLouisA1.o nguyenLouisA1Main.o -o nguyenLouisA1 -lm
nguyenLouisA1.o: nguyenLouisA1.c givenA1.h
	gcc -Wall -std=c99 -c nguyenLouisA1.c
nguyenLouisA1Main.o: nguyenLouisA1Main.c givenA1.h
	gcc -Wall -std=c99 -c nguyenLouisA1Main.c
clean:
	rm *.o nguyenLouisA1