ls:main.o helperfunctions.o directoryFunctions.o
	gcc -o ls main.c helperfunctions.c directoryFunctions.c

main.o: main.c
	gcc -c main.c

helperfunctions.o: helperfunctions.c
	gcc -c helperfunctions.c

directoryFunctions.o: directoryFunctions.c
	gcc -c directoryFunctions.c

clean:
	rm *.o
