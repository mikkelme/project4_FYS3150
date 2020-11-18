CPPflags = c++ -O3 -std=c++11
LIB =  -larmadillo #-llapack -lblas
PROG = main

${PROG}: 						${PROG}.o functions.o
										${CPPflags} ${PROG}.o functions.o ${LIB} -o ${PROG}.exe


${PROG}.o: 					${PROG}.cpp
										${CPPflags} -c ${PROG}.cpp

functions.o: 				functions.cpp functions.h
										${CPPflags} -c functions.cpp


clean:
	rm -f *.o *~
