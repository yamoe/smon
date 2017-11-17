
include ../Makefile.common


TARGET = libsmon_native.so
OBJS = $(shell find . -name "*.cpp")
INC = -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux


all: $(TARGET)

$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -fPIC $(INC) -shared -o $@ $^ $(LDFLAGS)


clean: 
	@rm -rf $(TARGET)
	@find  -name '*.o' -exec rm {} \;
	

test:
	@echo ${JAVA_HOME}


