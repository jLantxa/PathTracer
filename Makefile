CC=clang++
CFLAGS=-std=c++17 -O3 -Werror

BUILD=build

SRC=src
INCLUDE=include
BUILD=build

TEST=$(SRC)/test

TRACER_SOURCES += \
	$(SRC)/PathTracer.cpp \
	$(SRC)/GeometryRenderer.cpp \
	$(SRC)/Surface.cpp \
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

count-lines:
	@cloc Makefile $(SRC) $(INCLUDE)

docs:
	doxygen

PathTracerTest:
	$(CC) $(CFLAGS) -DDEBUG_LEVEL=6 -fopenmp  -I $(INCLUDE)/ $(TRACER_SOURCES) $(TEST)/PathTracerTest.cpp -o $(BUILD)/PathTracerTest
