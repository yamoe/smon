
include ../Makefile.common


TARGET = smon_native_test
SRC_DIR = .
OBJS = $(wildcard $(SRC_DIR)/*.cpp)
INC = -I../smon_native/include


all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "build: " $(TARGET)
	@$(CXX) $(CXXFLAGS) $(INC) -o $@ $^ $(LDFLAGS)


clean: 
	@echo "clean: " $(TARGET)
	@rm -rf $(TARGET)
	@find  -name '*.o' -exec rm {} \;
	

test:
	@echo $(OBJS)


