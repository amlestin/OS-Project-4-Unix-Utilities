all: myls mysearch mystat mytail

myls: myls-lestin.c strmode.c
	gcc -std=c99 -D_BSD_SOURCE -o myls myls-lestin.c

mysearch: mysearch-lestin.c
	gcc -std=c99 -D_BSD_SOURCE -o mysearch mysearch-lestin.c

mystat: mystat-lestin.c strmode.c
	gcc -std=c99 -D_BSD_SOURCE -o mystat mystat-lestin.c

mytail: mytail-lestin.c
	gcc -std=c99 -o mytail mytail-lestin.c

clean:
	rm -f myls
	rm -f mysearch
	rm -f mystat
	rm -f mytail
