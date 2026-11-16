cmake_minimum_required(VERSION 3.20)

project(MPPCKP_Heuristic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CPLEX installation directory
set(CPLEX_STUDIO_DIR
        "$ENV{HOME}/ibm/CPLEX_Studio2220"
)

# CPLEX platform and library format
set(CPLEX_PLATFORM "x86-64_linux")
set(CPLEX_LIB_FORMAT "static_pic")

# Your executable
add_executable(MPPCKP_Heuristic
        main.cpp
        src/Instance.cpp
        src/Instance.h
)

# Include directories
target_include_directories(MPPCKP_Heuristic PRIVATE
        "${CPLEX_STUDIO_DIR}/cplex/include"
        "${CPLEX_STUDIO_DIR}/concert/include"
)

# CPLEX compilation definitions
target_compile_definitions(MPPCKP_Heuristic PRIVATE IL_STD)

target_compile_options(MPPCKP_Heuristic PRIVATE
        -m64
        -fPIC
        -fno-strict-aliasing
        -fexceptions
)

# Library directories
target_link_directories(MPPCKP_Heuristic PRIVATE
        "${CPLEX_STUDIO_DIR}/cplex/lib/${CPLEX_PLATFORM}/${CPLEX_LIB_FORMAT}"
        "${CPLEX_STUDIO_DIR}/concert/lib/${CPLEX_PLATFORM}/${CPLEX_LIB_FORMAT}"
)

# Link CPLEX libraries
target_link_libraries(MPPCKP_Heuristic PRIVATE
        ilocplex
        concert
        cplex
        m
        pthread
        dl
)