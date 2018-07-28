CC=g++

BUILD=build

SRC=src
INCLUDE=include
BUILD=build

TEST=$(SRC)/test

TRACER_SOURCES += \
	$(SRC)/PathTracer.cpp \
	$(SRC)/Canvas.cpp \
	$(SRC)/Objects.cpp \
	$(SRC)/Camera.cpp \
	$(SRC)/Light.cpp \
	$(SRC)/Vector3D.cpp \
	$(SRC)/Utils.cpp

docs:
	doxygen

PathTracerTest:
	$(CC) -fopenmp -O3 -std=c++17 -I $(INCLUDE)/ $(TRACER_SOURCES) $(TEST)/PathTracerTest.cpp -o $(BUILD)/PathTracerTest

fire:
	@make PathTracerTest
	@./build/PathTracerTest 800 600 90 128 6
	@eog pathTracerTest.ppm &
