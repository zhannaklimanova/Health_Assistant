CC = g++
CFLAGS = -g -Wall -std=c++17
TARGET = HealthAssistant

all: $(TARGET)
$(TARGET): $(TARGET).cpp
	mkdir -p ./bin/
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).cpp

clean:
	$(RM) bin/$(TARGET)