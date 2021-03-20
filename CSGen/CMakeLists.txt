# Project setup
cmake_minimum_required(VERSION 3.14)
project("csgen" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Route executables into folder 'bin' and libraries into 'lib'
# https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
    src/Framework.cpp
    src/Main.cpp
    src/TurboPragmaticus.cpp
  "src/Framework.cpp")

target_include_directories(${TARGET_NAME}
PUBLIC
    src    
)

target_link_libraries(${TARGET_NAME} 
PUBLIC
    ${CONAN_LIBS}
)

# Copy resources to binary dir (for easier testing)
file(COPY "resources" DESTINATION "bin")
