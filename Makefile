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
	$(SRC)/Utils.cpp \
	$(SRC)/SceneParser.cpp

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

LIBXML2_LIBS=`xml2-config --libs`
LIBXML2_CFLAGS=`xml2-config --cflags`

VISUALIZER=$(SRC)/visualizer
VISUALIZER_FLAGS += \
	-DSCENE_PARSER_TEMPORARY \
	-DVISUALIZER_DUMP \
	-DDEBUG_LEVEL=6 \
	-lSDL2 \
	$(LIBXML2_LIBS) $(LIBXML2_CFLAGS)
Visualizer:
	@$(CC) $(CFLAGS) $(VISUALIZER_FLAGS) \
	-fopenmp -I $(INCLUDE)/ \
	$(TRACER_SOURCES) $(VISUALIZER)/Visualizer.cpp -o $(BUILD)/Visualizer
