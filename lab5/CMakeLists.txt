cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/huffman_encode.c src/huffman_decode.c)
set(HEADERS src/huffman_encode.h src/huffman_decode.h src/huffman_core.h)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
