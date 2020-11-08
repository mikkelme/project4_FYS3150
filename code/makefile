CPPflags = c++ #-O3 -std=c++11
LIB =  -larmadillo #-llapack -lblas
PROG = main

${PROG}: 						${PROG}.o
										${CPPflags} ${PROG}.o ${LIB} -o ${PROG}.exe

${PROG}.o: 					${PROG}.cpp
										${CPPflags} -c ${PROG}.cpp


# vec3.o: 						vec3.cpp vec3.h
# 										${CPPflags} -c vec3.cpp
#
#
# solarsystem.o: 			solarsystem.cpp solarsystem.h
# 										${CPPflags} -c solarsystem.cpp
#
#
# celestialbody.o: 		celestialbody.cpp celestialbody.h
# 										${CPPflags} -c celestialbody.cpp
#
# solver.o:						solver.cpp solver.h
# 										${CPPflags} -c solver.cpp
#
# force.o:						force.cpp force.h
# 										${CPPflags} -c force.cpp

clean:
	rm -f *.o *~
