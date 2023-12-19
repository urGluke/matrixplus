CFLAGS = -Wall -Werror -Wextra -std=c++17
TEST_FLAGS = -lgtest -lm -lpthread 
SRC = $(wildcard *.cpp)
TEST_SRC = $(wildcard tests/*.cpp)
TEST_O = s21*.o test.o
HEADER = s21_matrix_oop.h
GCOV_FLAGS = -ftest-coverage -fprofile-arcs --coverage

ifeq ($(shell uname -s), Linux)
	TEST_FLAGS += -lsubunit
endif

OBJ_LIB := $(patsubst %.cpp, %.o, $(SRC))

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

all: test

s21_matrix_oop.a: $(OBJ_LIB)
	ar rcs $@ $(OBJ_LIB)
	ranlib $@

test: clean s21_matrix_oop.a
	g++ $(CFLAGS) -c $(SRC) $(TEST_SRC)
	g++ $(CFLAGS) -o test.out $(TEST_O) s21_matrix_oop.a $(TEST_FLAGS)
	rm -rf *.o *.a
	./test.out

style:
	clang-format -style=Google -i *.cpp *.h
	clang-format -style=Google -i tests/*.cpp
	clang-format -style=Google -n *.cpp *.h

gcov_report: clean
	mkdir gcov_report
	g++ $(TEST_SRC) $(SRC) -lcheck -o gcov_r $(GCOV_FLAGS) $(TEST_FLAGS)
	./gcov_r
	lcov -t "gcov_s21_matrix" -o s21_matrix.info -c -d .
	gcovr -r . --html --html-details -o coverage_report.html
	mv *.html *.css *.gcno *.gcda gcov_r *.info gcov_report 
	open ./gcov_report/coverage_report.html

clean:
	rm -rf *.o *.a *.out gcov_report *.gcno *.tar gcov_r* *.info

dist: clean
	tar -cf s21_matrix_oop.tar *.cpp *.h tests Makefile