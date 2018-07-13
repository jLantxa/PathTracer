CC=g++

BUILD=build

SRC=src
INCL=$(SRC)/include
BUILD=build

TEST=$(SRC)/test

TRACER_SOURCES += \
	$(SRC)/RayTracer.cpp \
	$(SRC)/Canvas.cpp \
	$(SRC)/Objects.cpp \
	$(SRC)/Camera.cpp \
	$(SRC)/Light.cpp \
	$(SRC)/Geometry.cpp \
	$(SRC)/Utils.cpp

docs:
	doxygen

VectorTest:
	$(CC) -I $(INCL)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(TEST)/VectorTest.cpp -o $(BUILD)/VectorTest

CameraTest:
	$(CC) -I $(INCL)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(SRC)/Camera.cpp $(TEST)/CameraTest.cpp -o $(BUILD)/CameraTest

CanvasTest:
	$(CC) -I $(INCL)/ $(SRC)/Canvas.cpp $(TEST)/CanvasTest.cpp -o $(BUILD)/CanvasTest

ObjectsTest:
	$(CC) -I $(INCL)/ $(TRACER_SOURCES) $(TEST)/ObjectsTest.cpp -o $(BUILD)/ObjectsTest

AllUnitTests:
	@make VectorTest
	@make CameraTest
	@make CanvasTest
	@make ObjectsTest
	@echo "All tests compiled."

RayTracerTest:
	$(CC) -I $(INCL)/ $(TRACER_SOURCES) $(TEST)/RayTracerTest.cpp -o $(BUILD)/RayTracerTest -lSDL2