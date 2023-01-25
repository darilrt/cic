CIC=cic
CC=g++

# Directories
CIC_DIR=src
CPP_DIR=cpp/src
CPP_OBJ_SRC=cpp/obj/src
BIN_DIR=bin
TARGET=game

# Files
FILES=$(shell ls $(CIC_DIR) | grep .cic | sed 's/.cic//g')
CIC_FILES=$(patsubst %, $(CIC_DIR)/%.cic, $(FILES))
OBJECTS=$(patsubst %, $(CPP_OBJ_SRC)/%.o, $(FILES))
OUTPUT=$(BIN_DIR)/$(TARGET)
CFLAGS=-Wall -std=c++17 -lstdc++
INCLUDES=
LIBS=

ifeq ($(OS),Windows_NT) # Windows_NT is the identifier for all versions of Windows
	INCLUDES += -I"C:\usr\include"
	LIBS += -L"C:\usr\lib"
	CFLAGS += -lmingw32 -lSDL2main -lSDL2
else
	INCLUDES += -I"/usr/include"
	CFLAGS += -lSDL2 
endif

# Output
$(OUTPUT): $(OBJECTS)
	mkdir -p bin
	$(CC) $(OBJECTS) -o $(OUTPUT) $(INCLUDES) $(LIBS) $(CFLAGS)

$(CPP_OBJ_SRC)/%.o : $(CPP_DIR)/%.cpp
	mkdir -p $(CPP_OBJ_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@

$(CPP_DIR)/%.cpp : $(CIC_DIR)/%.cic
	mkdir -p $(CPP_DIR)
	$(CIC) $< -o $(CPP_DIR)

run: $(OUTPUT)
	$(OUTPUT)

setup:
	mkdir -p $(CPP_DIR)
	for file in $(FILES) ; do \
		$(CIC) $(CIC_DIR)/$$file.cic -o $(CPP_DIR) ; \
	done

clean:
	rm -rf $(CPP_DIR) $(CPP_OBJ_SRC) $(OUTPUT)
	rm -rf $(BIN_DIR)
	rm -rf ./cpp