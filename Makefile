# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
dicttest : dicttest.o populatetree.o countwords.o
	$(CXX) $(CXXFLAGS) -g -pthread -o dicttest $^ 

dicttest.o : dicttest.cpp populatetree.cpp countwords.cpp
populatetree.o : populatetree.cpp populatetree.h
countwords.o : countwords.cpp countwords.h
exec.o : exec.h

clean :
	rm dicttest.o populatetree.o countwords.o