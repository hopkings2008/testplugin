.PHONY: all clean

TARGET=client

all:
	go build -gcflags "-N -l" -o $(TARGET)

clean:
	-@rm -f $(TARGET)
