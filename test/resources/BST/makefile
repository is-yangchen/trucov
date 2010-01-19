H = bst.h

#OBJS = global.o utility.o main.o include.o
OBJS = bst.o driver.o
driver: $(OBJS)
	g++ -fprofile-arcs -ftest-coverage -o driver $(OBJS)

bst.o: bst.h bst.cpp
	g++ -fprofile-arcs -ftest-coverage -c -o bst.o bst.cpp

driver.o: bst.o driver.cpp 
	g++ -fprofile-arcs -ftest-coverage -c -o driver.o driver.cpp

clean:
	rm *.o driver



