all: calc.c format.c
	gcc -Wall -g -o calc calc.c
	gcc -Wall -g -o format format.c
	#Use ./format [args] or ./calc [args]

calc: calc.c
	gcc -Wall -g -o calc calc.c
	#Use ./calc [args]

fomat: format.c
	gcc -Wall -g -o format format.c
	#Use ./format [args]

test:
	./calc - xFF o360 d
	./calc + xFFFFFFFFFFFF d1 d
	./calc + x7BCDEFABCDEFABCFF d1 d
	./calc + d11111111111111111111111111111111111111111111111111111111111000 d33333333333333333333333333333333333333333333333333333333333789 d
	./calc - d11111111111111111111111111111111111111111111111111111111111000 d33333333333333333333333333333333333333333333333333333333333789 d

clean:
	rm -f calc
	rm -f format
	#Cleaned
