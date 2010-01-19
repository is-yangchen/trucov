CC=g++
LINKCC=g++
EXENAME=hw1

$(EXENAME): main.o binary_search_tree.o test_drivers.o
	$(LINKCC) -o $(EXENAME) main.o binary_search_tree.o test_drivers.o -lgcov

main.o: main.cpp binary_search_tree.h
	$(CC) -fprofile-arcs -ftest-coverage -c -o main.o main.cpp

binary_search_tree.o: binary_search_tree.cpp binary_search_tree.h
	$(CC) -fprofile-arcs -ftest-coverage -c -o binary_search_tree.o binary_search_tree.cpp

test_drivers.o: test_drivers.cpp test_drivers.h
	$(CC) -fprofile-arcs -ftest-coverage -c -o test_drivers.o test_drivers.cpp

clean:
	rm *.o *.gcda *.trucov *.dot *.svg hw1