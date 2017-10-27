# Compiler flags
CXX = g++
CXXFLAGS = -Wall -g

# Points to the root of cgreen test files
CGREEN_DIR = ../../../cgreen

# Google Test headers
CGREEN_HEADERS = -I$(CGREEN_DIR)/include -I$(CGREEN_DIR)/include/cgreen/internal

# External cgreen library and where it's located
LIBS = -L$(CGREEN_DIR) -lcgreen

# Where to place your unit test code
UT_DIR = code

# Example directory
EXAMPLE_DIR = example

# SJSU Project file directories
PROJ_DIR = ../
HEADER_DIR = -I$(PROJ_DIR)/L0_LowLevel \
               -I$(PROJ_DIR)/L1_FreeRTOS \
               -I$(PROJ_DIR)/L1_FreeRTOS/include \
               -I$(PROJ_DIR)/L1_FreeRTOS/hooks \
               -I$(PROJ_DIR)/L1_FreeRTOS/MemMang \
               -I$(PROJ_DIR)/L1_FreeRTOS/portable \
               -I$(PROJ_DIR)/L1_FreeRTOS/portable/mpu \
               -I$(PROJ_DIR)/L2_Drivers/base \
               -I$(PROJ_DIR)/L3_Utils \
               -I$(PROJ_DIR)/L3_Utils/tlm \
               -I$(PROJ_DIR)/L4_IO \
               -I$(PROJ_DIR)/L4_IO/fat \
               -I$(PROJ_DIR)/L4_IO/fat/disk \
               -I$(PROJ_DIR)/L4_IO/fat/option \
               -I$(PROJ_DIR)/L4_IO/wireless \
               -I$(PROJ_DIR)/L5_Application \
               -I$(PROJ_DIR)/L5_Application \
               -I$(PROJ_DIR)/L5_Application/examples \
               -I$(PROJ_DIR)/L5_Application/periodic_scheduler \
               -I$(EXAMPLE_DIR) \

# Default for "make"
all : test_all

clean :
	rm -f test_all *.o

test_main.o: $(UT_DIR)/test_main.cpp
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_main.cpp $(LIBS)

turtle.o: $(EXAMPLE_DIR)/turtle.cpp
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(EXAMPLE_DIR)/turtle.cpp $(LIBS)

foo.o: $(EXAMPLE_DIR)/foo.cpp turtle.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(EXAMPLE_DIR)/foo.cpp $(LIBS)

test_foo.o: $(UT_DIR)/test_foo.cpp foo.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_foo.cpp $(LIBS)

c_list.o: $(PROJ_DIR)/L3_Utils/src/c_list.c
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(PROJ_DIR)/L3_Utils/src/c_list.c $(LIBS)

geo_gps.o: $(PROJ_DIR)/L5_Application/source/geo_gps.cpp
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(PROJ_DIR)L5_Application/source/geo_gps.cpp $(LIBS)

test_geo_gps.o: $(UT_DIR)/test_geo_gps.cpp geo_gps.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_geo_gps.cpp $(LIBS)

geo_compass.o: $(PROJ_DIR)/L5_Application/source/geo_compass.cpp
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(PROJ_DIR)L5_Application/source/geo_compass.cpp $(LIBS)

test_geo_compass.o: $(UT_DIR)/test_geo_compass.cpp geo_compass.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_geo_compass.cpp $(LIBS)

geo_controller.o: $(PROJ_DIR)/L5_Application/source/geo_controller.cpp
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(PROJ_DIR)L5_Application/source/geo_controller.cpp $(LIBS)

test_geo_controller.o: $(UT_DIR)/test_geo_controller.cpp geo_controller.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_geo_controller.cpp $(LIBS)

#gps_parse.o: $(PROJ_DIR)/L5_Application/gps_parse.c
#	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(PROJ_DIR)/L5_Application/gps_parse.c $(LIBS)
	
test_c_list.o: $(UT_DIR)/test_c_list.c $(HEADER_DIR) c_list.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) -c $(UT_DIR)/test_c_list.c $(LIBS)

test_all: test_main.o c_list.o foo.o geo_gps.o geo_controller.o geo_compass.o
	$(CXX) $(CGREEN_HEADERS) $(HEADER_DIR) $(CXXFLAGS) $^ -o $@ $(LIBS)
