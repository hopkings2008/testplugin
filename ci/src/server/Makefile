.PHONY: all clean

ROOT=$(shell pwd)

HOME=/home/zouyu/hopkings

VENDOR=$(ROOT)/vendor

TARGET=server

all:
	export GOPATH=$(VENDOR):$(HOME) && go build -gcflags "-N -l" -o $(TARGET)

clean:
	-@rm -f $(TARGET)
