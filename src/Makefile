OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -std=c++11 -msse2

OUT_DIR=./
FRAME_DIR=../rendered-frames/

BIKE_BIN=$(OUT_DIR)/bin_bike
BIKE_LIGHT_BIN=$(OUT_DIR)/lbin_bike
RIDER_BIN=$(OUT_DIR)/bin_rider
TRACK_BIN=$(OUT_DIR)/a2-model-track
BIN=bike_light

SRC_DIR=./
INC_DIR=$(SRC_DIR)/includes/

# Yes I know there are better ways to do this. Short on time.
BIKE_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_bike.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
BIKE_LIGHT_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
RIDER_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_rider.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
TRACK_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_track.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
INCLUDES=$(INC_DIR)/gl_framework.hpp $(INC_DIR)/shader_util.hpp $(INC_DIR)/point.hpp $(INC_DIR)/triangle.hpp $(INC_DIR)/main.hpp $(INC_DIR)/spprism.hpp $(INC_DIR)/hnode.hpp $(INC_DIR)/bike.hpp $(INC_DIR)/rider.hpp $(INC_DIR)/entity.hpp $(INC_DIR)/track.hpp

all: $(BIN)

bike: $(BIKE_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(BIKE_SRCS) -o $(BIKE_BIN) $(LDFLAGS) $(LIBS)

bike_light: $(BIKE_LIGHT_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	mkdir -p $(FRAME_DIR)
	g++ $(CPPFLAGS) $(BIKE_LIGHT_SRCS) -o $(BIKE_LIGHT_BIN) $(LDFLAGS) $(LIBS)

rider: $(RIDER_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(RIDER_SRCS) -o $(RIDER_BIN) $(LDFLAGS) $(LIBS)

track: $(TRACK_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(TRACK_SRCS) -o $(TRACK_BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN) $(OUT_DIR)/*~ $(OUT_DIR)/*.o
