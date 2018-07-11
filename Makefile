CC=g++

SRC=src
BUILD=build

TEST=$(SRC)/test

docs:
	doxygen

VectorTest:
	$(CC) -I $(SRC)/ $(SRC)/Geometry.cpp $(TEST)/VectorTest.cpp -o $(BUILD)/VectorTest