OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -std=c++11 -msse2

OUT_DIR=./

BIKE_BIN=$(OUT_DIR)/bin_bike
RIDER_BIN=$(OUT_DIR)/bin_rider
TRACK_BIN=$(OUT_DIR)/a2-model-track
BIN=bike

SRC_DIR=./

# Yes I know there are better ways to do this. Short on time.
BIKE_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_bike.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
RIDER_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_rider.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
TRACK_SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/main_track.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp $(SRC_DIR)/spprism.cpp $(SRC_DIR)/hnode.cpp $(SRC_DIR)/entity.cpp
INCLUDES=$(SRC_DIR)/gl_framework.hpp $(SRC_DIR)/shader_util.hpp $(SRC_DIR)/point.hpp $(SRC_DIR)/triangle.hpp $(SRC_DIR)/main.hpp $(SRC_DIR)/spprism.hpp $(SRC_DIR)/hnode.hpp $(SRC_DIR)/bike.hpp $(SRC_DIR)/rider.hpp $(SRC_DIR)/entity.hpp $(SRC_DIR)/track.hpp

all: $(BIN)

bike: $(BIKE_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(BIKE_SRCS) -o $(BIKE_BIN) $(LDFLAGS) $(LIBS)

rider: $(RIDER_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(RIDER_SRCS) -o $(RIDER_BIN) $(LDFLAGS) $(LIBS)

track: $(TRACK_SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(TRACK_SRCS) -o $(TRACK_BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN) $(OUT_DIR)/*~ $(OUT_DIR)/*.o
