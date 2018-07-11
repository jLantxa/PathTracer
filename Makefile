CC=g++

SRC=src
BUILD=build

TEST=$(SRC)/test

docs:
	doxygen

VectorTest:
	$(CC) -I $(SRC)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(TEST)/VectorTest.cpp -o $(BUILD)/VectorTest

CameraTest:
	$(CC) -I $(SRC)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(SRC)/Camera.cpp $(TEST)/CameraTest.cpp -o $(BUILD)/CameraTest

AllTests:
	@make VectorTest
	@make CameraTest
	@echo "All tests compiled."