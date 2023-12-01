CC = clang
CFLAGS = -Wall -Wextra

# Debug options
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    DEBUG_FLAGS = -g
    OPTIMIZATION_FLAGS =
else
    DEBUG_FLAGS =
    OPTIMIZATION_FLAGS = -O3
endif

# Output executable name
TARGET = filerate

# Source file
SRC = filerate.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
