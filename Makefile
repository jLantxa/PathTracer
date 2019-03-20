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

PATH_TRACER_TEST_FLAGS += \
	-DPATH_TRACER_TIME_ESTIMATION=1 \
	-DDEBUG_LEVEL=6

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
	@$(CC) $(CFLAGS) $(PATH_TRACER_TEST_FLAGS) \
	-fopenmp -I $(INCLUDE)/ \
	$(TRACER_SOURCES) $(TEST)/PathTracerTest.cpp -o $(BUILD)/PathTracerTest
