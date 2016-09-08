#
# Makefile for crmq producer api test
#
#
CC=gcc -m64
CXX=g++ -m64
CFLAGS= -g -c -O0 -pipe -Wall
CXXFLAGS= -g -c -O0 -pipe -Wall -fno-strict-aliasing -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64
LIBS= -L../src -lcmq
INCLUDE= -I../src/

TARGET=cmq_sample

all: $(TARGET)

cmq_sample:cmq_sample.o
	@echo Compling the program $@
	$(CXX) -o $@ $^ $(LIBS)

%.o:%.cpp
	@echo Compiling the $<
	$(CXX) $(CXXFLAGS) -o $@ $< $(INCLUDE) 

%.o:%.c
	@echo Compiling the $<
	$(CC) $(CFLAGS) -o $@ $< $(INCLUDE) 

.PHONY: clean
clean:
	@rm -rf *.o
	@rm -rf $(TARGET)

#End
