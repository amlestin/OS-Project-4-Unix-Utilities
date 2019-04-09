all: myls mysearch mystat mytail
	
myls:
	gcc -o myls myls-lestin.c

mysearch:
	gcc -o mysearch mysearch-lestin.c

mystat:
	gcc -o mystat mystat-lestin.c

mytail:
	gcc -o mytail mytail-lestin.c
