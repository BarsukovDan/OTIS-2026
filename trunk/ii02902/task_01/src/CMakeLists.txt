
cmake_minimum_required(VERSION 3.10)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(ii02902_task_01 LANGUAGES CXX)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_executable(ii02902_task_01 ${SOURCES} ${HEADERS})
