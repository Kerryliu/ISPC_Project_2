CXX=clang++ -std=c++11 -pthread
CXXFLAGS=-I temp/ -O3 -Wall
ISPC=ispc

#not avx intrinsics
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

#enable avx intrinsics
ISPCFLAGSAVX= --target=avx2-i32x16

default: squareRoot

squareRoot: sqrt.cpp sqrtISPC.ispc
	make clean
	mkdir -p temp/
	ispc $(ISPCFLAGS) sqrtISPC.ispc -o temp/sqrtISPC.o -h temp/sqrtISPC.h
	$(CXX) $(CXXFLAGS) sqrt.cpp -c -o temp/sqrt.o
	$(CXX) $(CXXFLAGS) -w tasksys.cpp -c -o temp/tasksys.o
	$(CXX) $(CXXFLAGS) -o  squareRoot.o temp/tasksys.o temp/sqrt.o temp/sqrtISPC.o
	./squareRoot.o
clean:
	rm -rf temp/
	find . -type f -name '*.o' -delete

squareRootAVX: sqrt.cpp sqrtISPC.ispc
	make clean
	mkdir -p temp/
	ispc $(ISPCFLAGSAVX) sqrtISPC.ispc -o temp/sqrtISPC.o -h temp/sqrtISPC.h
	$(CXX) $(CXXFLAGS) sqrt.cpp -c -o temp/sqrt.o
	$(CXX) $(CXXFLAGS) -w tasksys.cpp -c -o temp/tasksys.o
	$(CXX) $(CXXFLAGS) -o  squareRootAVX.o temp/tasksys.o temp/sqrt.o temp/sqrtISPC.o
	./squareRootAVX.o
