cmake_minimum_required (VERSION 3.1)
project (wavelets_smart)
set (CMAKE_CXX_STANDARD 11)

if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(wavelets_smart ${CMAKE_SOURCE_DIR}/src/wavelets_smart.cpp)

install (TARGETS 
	wavelets_smart  
	DESTINATION ${CMAKE_SOURCE_DIR}/bin/
)

