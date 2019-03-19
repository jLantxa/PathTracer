CC=g++
CFLAGS=-std=c++17 -O3

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

init:
	@mkdir build/
	@mkdir pic/

clean:
	@rm -rf build/*
	@rm -rf pic/*

docs:
	doxygen

PathTracerTest:
	$(CC) $(CFLAGS) -fopenmp  -I $(INCLUDE)/ $(TRACER_SOURCES) $(TEST)/PathTracerTest.cpp -o $(BUILD)/PathTracerTest
