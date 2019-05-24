########
# Download freeglut: http://freeglut.sourceforge.net/

project(spline_glut_demo)

cmake_minimum_required(VERSION 3.1)

find_package(OpenGL REQUIRED)


#-------------------------------------------------------------------------------
# Build flags

if(DEFINED CMAKE_COMPILER_IS_GNUCC)
    # Enable C++11
    SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x" )
endif()

add_definitions( -DFREEGLUT_STATIC )

#-------------------------------------------------------------------------------
# List of headers locations
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src      
    ${OPENGL_INCLUDE_DIR}
)

#-------------------------------------------------------------------------------
# List of cpu sources
file(GLOB_RECURSE host_sources 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../splines.cpp
)

file(GLOB_RECURSE headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/../splines.inl
)

#-------------------------------------------------------------------------------
# The project build setup

ADD_EXECUTABLE( ${PROJECT_NAME} ${host_sources} ${headers})

#-------------------------------------------------------------------------------
# Link application with opengl glu etc

if(WIN32)
    if(MINGW)
        set(MISC -static-libgcc -static-libstdc++)
    endif()
endif()

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
    freeglut_static
    winmm  
    gdi32      
    ${MISC}    
    ${OPENGL_LIBRARIES}    
)

