CIC=cic
CC=g++

# Directories
CIC_DIR=src
CPP_DIR=cpp/src
CPP_OBJ_SRC=cpp/obj/src
BIN_DIR=bin
TARGET=cic

# Files
FILES=$(shell ls $(CIC_DIR) | grep .cic | sed 's/.cic//g')
CIC_FILES=$(patsubst %, $(CIC_DIR)/%.cic, $(FILES))
OBJECTS=$(patsubst %, $(CPP_OBJ_SRC)/%.o, $(FILES))
OUTPUT=$(BIN_DIR)/$(TARGET)

# Output
$(OUTPUT): $(OBJECTS)
	mkdir -p bin
	$(CC) $(OBJECTS) -o $(OUTPUT) -lstdc++

$(CPP_OBJ_SRC)/%.o : $(CPP_DIR)/%.cpp
	mkdir -p $(CPP_OBJ_SRC)
	$(CC) -c $< -o $@

$(CPP_DIR)/%.cpp : $(CIC_DIR)/%.cic
	mkdir -p $(CPP_DIR)
	$(CIC) $< -o $(CPP_DIR)

selfbuild:
	mkdir -p $(CPP_DIR)
	for file in $(FILES) ; do \
		./bin/cic $(CIC_DIR)/$$file.cic -o $(CPP_DIR) ; \
	done
	$(CC) $(OBJECTS) -o $(OUTPUT) -lstdc++

setup:
	mkdir -p $(CPP_DIR)
	for file in $(FILES) ; do \
		$(CIC) $(CIC_DIR)/$$file.cic -o $(CPP_DIR) ; \
	done

clean:
	rm -rf $(CPP_DIR) $(CPP_OBJ_SRC) $(OUTPUT)
	rm -rf $(BIN_DIR)
	rm -rf ./cpp