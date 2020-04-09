EXE			=	xbslicer

DIR			=	$(shell pwd)
SRC_DIR		=	$(DIR)/src
BUILD_DIR	=	$(DIR)/build

vpath %.cc $(SRC_DIR)
vpath %.hh $(SRC_DIR)
vpath %.o  $(BUILD_DIR)

CXX	:= g++
OPTIONS	= -std=c++11
MODE ?= debug

OBJ	:= main.o Slicer.o Slicing.o GCode.o STL.o \
	Configurations.o BufferReadBinary.o BufferWrite.o \
	Vec2d.o Vec3d.o Triangle.o Cross.o Loop.o Layer.o \
	DoubleToString.o

ifeq ($(strip $(MODE)), release)
	OPTIONS += -O2
	TARGET = $(EXE)
else
	OPTIONS += -g
	TARGET = $(EXE)-staging
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo $(strip $(MODE))
	$(CXX) $(OPTIONS) -pthread $(addprefix $(BUILD_DIR)/, $(OBJ)) -o $@

%.o:%.cc
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OPTIONS) -c $< -o $(BUILD_DIR)/$@

main.o: Slicer.o
Slicer.o: STL.o Slicing.o BufferWrite.o Slicer.hh
STL.o: Configurations.o BufferReadBinary.o Triangle.o STL.hh
Slicing.o: STL.o GCode.o Slicing.hh
GCode.o: Configurations.o Layer.o GCode.hh
Configurations.o: Configurations.hh
BufferReadBinary.o: BufferReadBinary.hh
BufferWrite.o: DoubleToString.o BufferWrite.hh
Vec2d.o: Vec3d.o Vec2d.hh
Vec3d.o: Configurations.o BufferReadBinary.o Vec3d.hh
Triangle.o: Vec3d.o Triangle.hh
Cross.o: Vec2d.o Cross.hh
Loop.o: Loop.hh
Layer.o: Loop.o Cross.o BufferWrite.o Layer.hh
DoubleToString.o: DoubleToString.hh

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(EXE)*
	@echo "Clean finished!"
