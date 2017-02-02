cmake_minimum_required(VERSION 2.8)
project(pbfa64)

option (USE_FLOATING_SQRT "Use float sqrt implementation" OFF)

if (USE_FLOATING_SQRT) 
  add_definitions(-DFLOATING_SQRT)
endif (USE_FLOATING_SQRT)

add_executable(pbfa64 src/pbfa64.cpp)

install(TARGETS pbfa64 RUNTIME DESTINATION bin)