INCLUDE := include
SRC := src
BUILD := build
TEST := test
DOC := doc

CXXFLAGS += \
	-std=c++20 \
	-O3 \
	-Wall -Werror \
	-Wpedantic -Wconversion

all: doc | $(BUILD)

$(BUILD):
	mkdir -p $(BUILD)

doc:
	@doxygen

clean:
	rm -r $(DOC)
	rm -r $(BUILD)
