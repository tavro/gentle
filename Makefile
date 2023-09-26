CC = g++
CFLAGS = -std=c++11

SRC_DIR = ./sources
EDITOR_SRC_DIR = $(SRC_DIR)/editor

SRCS = main.cpp \
       $(SRC_DIR)/utils/vector2d.cpp \
       $(SRC_DIR)/animation.cpp \
       $(SRC_DIR)/audio_source.cpp \
       $(SRC_DIR)/button.cpp \
       $(SRC_DIR)/game_object.cpp \
       $(SRC_DIR)/particle.cpp \
       $(SRC_DIR)/player.cpp \
       $(SRC_DIR)/text.cpp \
       $(SRC_DIR)/texture.cpp \
       $(SRC_DIR)/tile.cpp \
       $(SRC_DIR)/tile_map.cpp \
       $(SRC_DIR)/timer.cpp \
       $(SRC_DIR)/ui_object.cpp \
       $(SRC_DIR)/ui_state_object.cpp \
       $(SRC_DIR)/input_field.cpp \
       $(SRC_DIR)/canvas.cpp \
       $(SRC_DIR)/image.cpp \
       $(SRC_DIR)/ui_panel.cpp \
       $(EDITOR_SRC_DIR)/explorer.cpp \
       $(EDITOR_SRC_DIR)/heirarchy.cpp \
       $(SRC_DIR)/scene.cpp

TARGET = main

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

all: $(TARGET)
