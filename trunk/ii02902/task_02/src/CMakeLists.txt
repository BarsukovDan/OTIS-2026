cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.16 CACHE STRING "" FORCE)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(OTIS_Task02_ii02902 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

set(TASK01_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../task_01/src")

# Ядро логики: модели и сигналы из task_01 + новые модули task_02
add_library(otis_core_ii02902
    "${TASK01_SRC_DIR}/Model1_2.cpp"
    "${TASK01_SRC_DIR}/Model2_4.cpp"
    "${TASK01_SRC_DIR}/Model3_6.cpp"
    "${TASK01_SRC_DIR}/InputSignal.cpp"
    StabilityChecker.cpp
    Simulator.cpp
)

target_include_directories(otis_core_ii02902
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${TASK01_SRC_DIR}
)

# Исполняемый файл симуляции
add_executable(otis_app_ii02902 main.cpp)
target_link_libraries(otis_app_ii02902 PRIVATE otis_core_ii02902)

# Исполняемый файл модульных тестов
add_executable(otis_tests_ii02902
    ../tests/test_models.cpp
    ../tests/test_stability.cpp
    ../tests/test_simulator.cpp
)

target_link_libraries(otis_tests_ii02902
    PRIVATE
        otis_core_ii02902
        gtest
        gtest_main
        gmock
)

# Флаги покрытия для GCC / MinGW
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(otis_core_ii02902 PRIVATE --coverage -O0 -g)
    target_compile_options(otis_tests_ii02902 PRIVATE --coverage -O0 -g)
    target_link_options(otis_app_ii02902 PRIVATE --coverage)
    target_link_options(otis_tests_ii02902 PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(otis_tests_ii02902)