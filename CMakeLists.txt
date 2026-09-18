cmake_minimum_required(VERSION 3.16)
project(SentinelStream VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SENTINELSTREAM_BUILD_TESTS "Build unit tests" ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RDKAFKA REQUIRED rdkafka)

add_library(sentinel_core
    src/risk/rule_engine.cpp
    src/risk/risk_detector.cpp
)

target_include_directories(sentinel_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RDKAFKA_INCLUDE_DIRS}
)

target_link_libraries(sentinel_core PUBLIC ${RDKAFKA_LIBRARIES})
target_compile_options(sentinel_core PRIVATE ${RDKAFKA_CFLAGS_OTHER})

add_executable(sentinelstream
    src/main.cpp
    src/kafka/kafka_consumer.cpp
    src/kafka/kafka_producer.cpp
)

target_include_directories(sentinelstream PRIVATE include ${RDKAFKA_INCLUDE_DIRS})
target_link_libraries(sentinelstream PRIVATE sentinel_core ${RDKAFKA_LIBRARIES})
target_compile_options(sentinelstream PRIVATE ${RDKAFKA_CFLAGS_OTHER})

if(SENTINELSTREAM_BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(sentinel_tests tests/test_rule_engine.cpp)
    target_include_directories(sentinel_tests PRIVATE include)
    target_link_libraries(sentinel_tests PRIVATE sentinel_core GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(sentinel_tests)
endif()
