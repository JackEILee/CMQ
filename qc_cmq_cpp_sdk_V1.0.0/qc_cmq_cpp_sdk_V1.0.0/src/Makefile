#
# Makefile for cmq api
#
#
TARGET=libcmq
CC=gcc 
CXX=g++ 
ifeq ($(DEBUG), y)
   CC=gcc -g
   CXX=g++ -g
endif
CFLAGS= -O0 -pipe -Wall -D_GNU_SOURCE -D_REENTRANT -fPIC
CXXFLAGS= -O0 -pipe -Wall -fno-strict-aliasing -D_GNU_SOURCE -D_REENTRANT -fPIC
LIBS= -lcurl

INCLUDE= -I./  
OBJS = account.o queue.o cmq_client.o cmq_http.o cmq_exception.o cmq_tool.o jsoncpp.o sha1.o

all: $(TARGET).so $(TARGET).a

$(TARGET).a: $(OBJS)
	@echo Compiling the program $@
	ar rcs $@ $(OBJS)

$(TARGET).so: $(OBJS)
	@echo Compling the program $@
	$(CXX) -shared -o $@ $(OBJS) $(LIBS)

%.o:%.cpp
	@echo Compling the $<
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDE) 

%.o: %.cc
	@echo Compling the $<
	$(CXX)  $(CFLAGS) $(INC) -c -o $@ $< 	

%.o:%.c
	@echo Compling the $<
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)

.PHONY: clean
clean:
	-rm -rf $(OBJS)
	-rm -rf $(TARGET).*
	-rm -rf lib/$(TARGET).*

install:
	-mkdir lib
	-cp $(TARGET).* lib/
#End
