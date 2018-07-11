CC=g++

SRC=src
BUILD=build

TEST=$(SRC)/test

docs:
	doxygen

VectorTest:
	$(CC) -I $(SRC)/ $(SRC)/Geometry.cpp $(SRC)/Light.cpp $(TEST)/VectorTest.cpp -o $(BUILD)/VectorTest