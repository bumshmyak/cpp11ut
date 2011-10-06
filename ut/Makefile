SRCROOT = $(realpath ..)
include $(SRCROOT)/config.mak

TARGET = test 

SOURCES = $(SRCROOT)/ut/*.cc
CXXFLAGS += -I$(GTEST_INCLUDE_PATH)
LDFLAGS += $(GTEST_LIB_PATH)/gtest_main.a 

.PHONY: all clean check

all : $(OUTPUT_BIN_DIR)/$(TARGET)

$(OUTPUT_BIN_DIR)/$(TARGET) : $(OUTPUT_BIN_DIR) $(SOURCES)
	$(CXX) -o $(OUTPUT_BIN_DIR)/$(TARGET) $(CXXFLAGS) $(LDFLAGS) $(SOURCES)

$(OUTPUT_BIN_DIR) : 
	mkdir $(OUTPUT_BIN_DIR)

clean :
	rm -rf $(OUTPUT_BIN_DIR)

check : $(OUTPUT_BIN_DIR)/$(TARGET)
	./$(OUTPUT_BIN_DIR)/$(TARGET)
