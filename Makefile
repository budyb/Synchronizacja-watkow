CC = g++
CFLAGS = -Wall
LDFLAGS = -pthread -lncursesw
OBJFILES = checkout.o gate.o fan.o guard.o main.o 
TARGET = main
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~