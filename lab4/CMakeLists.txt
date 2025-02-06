cmake_minimum_required(VERSION 3.5.1)
project(lab4 C)

set(SRC src/main.c src/stack.c src/rpn.c)
set(HEADERS src/stack.h src/rpn.h)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
