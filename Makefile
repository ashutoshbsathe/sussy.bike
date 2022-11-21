OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -std=c++11 -msse2

OUT_DIR=./bin/
FRAME_DIR=./rendered-frames/

FMX_ANIMATE_BIN=$(OUT_DIR)/fmx_animate
BIN=fmx_animate

SRC_DIR=./src/
INC_DIR=$(SRC_DIR)/includes/

# Yes I know there are better ways to do this. Short on time.
FMX_ANIMATE_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
INCLUDES=$(INC_DIR)/gl_framework.hpp $(INC_DIR)/shader_util.hpp $(INC_DIR)/point.hpp $(INC_DIR)/triangle.hpp $(INC_DIR)/main.hpp $(INC_DIR)/spprism.hpp $(INC_DIR)/hnode.hpp $(INC_DIR)/bike.hpp $(INC_DIR)/rider.hpp $(INC_DIR)/entity.hpp $(INC_DIR)/track.hpp

all: $(BIN)

fmx_animate: $(FMX_ANIMATE_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	mkdir -p $(FRAME_DIR)
	g++ $(CPPFLAGS) $(FMX_ANIMATE_SRCS) -o $(FMX_ANIMATE_BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(FMX_ANIMATE_BIN) $(OUT_DIR)/*~ $(OUT_DIR)/*.o
