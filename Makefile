all: main.exe crossValidation.exe

CC=g++
CFLAGS=-Wall -O2 -fopenmp

OBJS=kNN.o distance.o
CVOBJS=evaluation.o $(OBJS)

%.o: src/%.cpp inc/%.h
	$(CC) $(CFLAGS) -c $< -o $@

main.exe: main.cpp $(OBJS)
	$(CC) $(CFLAGS) $< -o $@ $(OBJS)

crossValidation.exe: crossValidation.cpp $(CVOBJS)
	$(CC) $(CFLAGS) $< -o $@ $(CVOBJS)

test: main.exe
	valgrind ./main.exe data/01_kid.csv data/01_questions.csv 3 3 2 0 CR
	valgrind ./main.exe data/01_kid.csv data/01_questions.csv 3 3 2 1 CR
	valgrind ./main.exe data/01_kid.csv data/01_questions.csv 3 3 2 2 CR
	valgrind ./main.exe data/01_kid.csv data/01_questions.csv 3 3 2 3 CR 1.5
	valgrind ./main.exe data/01_kid.csv data/01_questions.csv 3 3 2 4 CR
	valgrind ./main.exe data/02_iris.csv data/02_iris_questions.csv 5 4 1 0 C
	valgrind ./main.exe data/02_iris.csv data/02_iris_questions.csv 5 4 1 1 C
	valgrind ./main.exe data/02_iris.csv data/02_iris_questions.csv 5 4 1 2 C
	valgrind ./main.exe data/02_iris.csv data/02_iris_questions.csv 5 4 1 3 C 1.75
	valgrind ./main.exe data/02_iris.csv data/02_iris_questions.csv 5 4 1 4 C

testCVfor01: crossValidation.exe
	./crossValidation.exe data/01_kid.csv 6 3 3 2 0 CR
	./crossValidation.exe data/01_kid.csv 6 3 3 2 1 CR
	./crossValidation.exe data/01_kid.csv 6 3 3 2 2 CR
	./crossValidation.exe data/01_kid.csv 6 3 3 2 3 CR 1.5
	./crossValidation.exe data/01_kid.csv 6 3 3 2 4 CR

testCVfor02: crossValidation.exe
	./crossValidation.exe data/02_iris.csv 6 5 4 1 0 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 1 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 2 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 3 C 1.75
	./crossValidation.exe data/02_iris.csv 6 5 4 1 4 C

testCVfor03: crossValidation.exe
	./crossValidation.exe data/03_Skin_NonSkin.csv 1000 5 3 1 0 C
	./crossValidation.exe data/03_Skin_NonSkin.csv 1000 5 3 1 1 C
	./crossValidation.exe data/03_Skin_NonSkin.csv 1000 5 3 1 2 C
	./crossValidation.exe data/03_Skin_NonSkin.csv 1000 5 3 1 3 C 2.0
	./crossValidation.exe data/03_Skin_NonSkin.csv 1000 5 3 1 4 C

testCVfor04: crossValidation.exe
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 0 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 1 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 2 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 3 RR 2.25
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 4 RR

testCVfor05: crossValidation.exe
	./crossValidation.exe data/05_music_plus.csv 6 5 116 2 0 CC
	./crossValidation.exe data/05_music_plus.csv 6 5 116 2 1 CC
	./crossValidation.exe data/05_music_plus.csv 6 5 116 2 2 CC
	./crossValidation.exe data/05_music_plus.csv 6 5 116 2 3 CC 2.5
	./crossValidation.exe data/05_music_plus.csv 6 5 116 2 4 CC

t:crossValidation.exe
	./crossValidation.exe data/02_iris.csv 6 5 4 1 0 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 1 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 2 C
	./crossValidation.exe data/02_iris.csv 6 5 4 1 3 C 1.75
	./crossValidation.exe data/02_iris.csv 6 5 4 1 4 C
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 0 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 1 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 2 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 3 RR
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 4 RR
	s:crossValidation.exe
	./crossValidation.exe data/04_music_default.csv 6 5 68 2 0 RR
d:
	@cat>data.out<<"eof"
	@./crossValidation.exe data/03_Skin_NonSkin.csv 6 5 3 1 0 C>>data.out
temp:
	valgrind ./crossValidation.exe data/02_iris.csv 6 5 4 1 0 C
	valgrind ./crossValidation.exe data/03_Skin_NonSkin.csv 6 5 3 1 0 C
	valgrind ./crossValidation.exe data/04_music_default.csv 6 5 68 2 0 RR
	valgrind ./crossValidation.exe data/05_music_plus.csv 6 5 116 2 0 CC
clean:
	rm *.exe *.o
