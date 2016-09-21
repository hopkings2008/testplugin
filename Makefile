.PHONY: all clean

BASE=$(shell pwd)

ROOT=$(shell pwd)/..

AUTH=/home/zouyu/hopkings/cis_gw_ats_plugin/auth

TARGET=libtest_plugin.so

SOURCES=$(wildcard *.cc)

HEADERS=$(wildcard *.h)

CFLAGS=-D_REENTRANT -DUSE_STD_SHARED_PTR -DHAVE_STD_SHARED_PTR -Wall -Werror -g -O0 -Wint-to-pointer-cast -fPIC -std=c++11

CXXFLAGS=-I./include -I$(AUTH)/include -I/home/sq/include

LDFLAGS=-shared -Wl,-rpath,/home/sq/lib

GTEST_LIBS=-L/home/sq/lib

OBJS=$(patsubst %.cc,%.o,$(SOURCES))

#LIBS=-Wl,-Bstatic -lgtest -lgmock -Wl,-Bdynamic -lstdc++ -lpthread -lc

LIBS=-latscppapi -ltsutil -laprutil-1 -lapr-1 -lcrypto -lstdc++ -lpthread -lc

all: $(TARGET)
	@echo "build successfully. $(TARGET) has been generated."


$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(LDFLAGS) $(GTEST_LIBS) $(OBJS) $(LIBS)

%.o:%.cc
	gcc $(CFLAGS) $(CXXFLAGS) -c -o $@ $<


clean:
	-@rm -rf $(OBJS) $(TARGET)
