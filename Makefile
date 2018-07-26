CC=g++

BUILD=build

SRC=src
INCL=$(SRC)/include
BUILD=build

TEST=$(SRC)/test

TRACER_SOURCES += \
	$(SRC)/PathTracer.cpp \
	$(SRC)/Canvas.cpp \
	$(SRC)/Objects.cpp \
	$(SRC)/Camera.cpp \
	$(SRC)/Light.cpp \
	$(SRC)/Geometry.cpp \
	$(SRC)/Utils.cpp

docs:
	doxygen

PathTracerTest:
	$(CC) -fopenmp -O3 -std=c++17 -I $(INCL)/ $(TRACER_SOURCES) $(TEST)/PathTracerTest.cpp -o $(BUILD)/PathTracerTest

fire:
	@make PathTracerTest
	@./build/PathTracerTest 800, 450, 90
	@eog pathTracerTest.ppm &