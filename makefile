all: myls mysearch mystat mytail
	
myls: myls-lestin.c
	gcc -o myls myls-lestin.c

mysearch: mysearch-lestin.c
	gcc -o mysearch mysearch-lestin.c

mystat: mystat-lestin.c
	gcc -o mystat mystat-lestin.c

mytail: mytail-lestin.cj
	gcc -o mytail mytail-lestin.c

clean:
	rm -f myls
	rm -f mysearch
	rm -f mystat
	rm -f mytail
