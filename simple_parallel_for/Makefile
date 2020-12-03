CC              := g++
CFLAGS          := -O3 -Wall
LIBS            := -ltbb
TARGET          := simple_parallel_for
SOURCE_FILES    := main.cpp

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCE_FILES) -o $(TARGET)

run: all
	@./$(TARGET)

clean:
	@rm -f *.o $(TARGET)
