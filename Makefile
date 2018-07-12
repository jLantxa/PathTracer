CC=g++

BUILD=build

SRC=src
INCL=$(SRC)/include
BUILD=build

TEST=$(SRC)/test

docs:
	doxygen

VectorTest:
	$(CC) -I $(INCL)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(TEST)/VectorTest.cpp -o $(BUILD)/VectorTest

CameraTest:
	$(CC) -I $(INCL)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(SRC)/Camera.cpp $(TEST)/CameraTest.cpp -o $(BUILD)/CameraTest

AllTests:
	@make VectorTest
	@make CameraTest
	@echo "All tests compiled."