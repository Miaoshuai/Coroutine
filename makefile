CC = clang++ -std=c++11 
CFLAGS = -wall -g

main:main.o coroutine.o
	${CC} main main.o coroutine.o
main.o:
	${CC} -c main.cpp
coroutine.o:coroutine.h
	${CC} -c coroutine.cpp
.PHONY:clean
clean:
	rm *.o
