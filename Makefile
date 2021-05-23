.SECONDEXPANSION:

define define_mkdir_target
$(1):
	@mkdir -p $(1)
endef

SRC_DIRS	 := src
INCLUDE_DIRS := include
TEST_DIR	 := test
BUILD_DIR  	 := build
DOC_DIR		 := doc

LIBXML2_LIBS=`xml2-config --libs`
LIBXML2_CFLAGS=`xml2-config --cflags`

VISUALIZER=$(SRC)/visualizer
VISUALIZER_FLAGS += \
	-DSCENE_PARSER_TEMPORARY \
	-DVISUALIZER_DUMP \
	-DDEBUG_LEVEL=0 \
	$(LIBXML2_LIBS) $(LIBXML2_CFLAGS)

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
CXXFLAGS := \
	-std=c++17 -Werror -Wall \
	$(patsubst %, -I %, $(INCLUDE_DIRS)/) \
	$(VISUALIZER_FLAGS) -lSDL2 -fopenmp

TARGET_VISUALIZER := $(BUILD_DIR)/Visualizer

.PHONY: all
all: Visualizer

.PHONY: Visualizer
Visualizer: $(TARGET_VISUALIZER)

.PHONY: doc
doc:
	@doxygen

$(TARGET_VISUALIZER): $(OBJS) | $$(dir $$@)
	$(CXX) $(CXXFLAGS) -o $(TARGET_VISUALIZER) $(OBJS)

$(BUILD_DIR)/%.o: %.cpp | $$(dir $$@)
	$(CXX) -c $(CXXFLAGS) -o $@ $?

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(DOC_DIR)

$(foreach dir,$(sort $(dir $(OBJS))),$(eval $(call define_mkdir_target,$(dir))))
