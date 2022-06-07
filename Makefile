CXX = g++
CFLAGS = -pthread -std=c++17 -Wall

SOURCES = Myers_Millers/mm.cpp Smith_Waterman/smith_waterman.cpp Smith_Waterman/test_sw.cpp
OBJECTS = $(SOURCES:.cpp=.o)

grader: $(OBJECTS)
	$(CXX) $(CFLAGS) -o grader $(OBJECTS) -latomic 

gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

grading.o: grading/grading.cpp gradinglib/gradinglib.hpp td7.cpp
	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.

main.o: main.cpp grading/grading.hpp
	$(CXX) -c $(CFLAGS) -o main.o main.cpp

SMITH_WATERMAN_SOURCES = Smith_Waterman/smith_waterman.cpp Smith_Waterman/test_sw.cpp
SMITH_WATERMAN_OBJECTS = $(SMITH_WATERMAN_SOURCES:.cpp=.o)

smith_waterman: $(SMITH_WATERMAN_OBJECTS)
	$(CXX) $(CFLAGS) -o Smith_Waterman/test_sw Smith_Waterman/test_sw.o -latomic

test_sw.o: Smith_Waterman/test_sw.cpp Smith_Waterman/smith_waterman.cpp
	$(CXX) -c $(CFLAGS) -o Smith_Waterman/test_sw.o Smith_Waterman/test_sw.cpp -I.

smith_waterman.o: Smith_Waterman/smith_waterman.cpp
	$(CXX) -c $(CFLAGS) -o Smith_Waterman/smith_waterman.o Smith_Waterman/smith_waterman.cpp -I.

clean:
	rm -rf *.o
	rm -f Smith_Waterman/*.o
	rm -f Smith_Waterman/smith_waterman
	rm -f Smith_Waterman/test_sw
	rm -f Myers_Millers/*.o
	rm -f Myers_Millers/mm