cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS
        "*.cpp"
        )
# Google Test
add_subdirectory(${CMAKE_HOME_DIRECTORY}/lib/googletest
  ${CMAKE_HOME_DIRECTORY}/build/googletest-build)

include_directories("${gtest_SOURCE_DIR}/include")

# RapidCheck
set(RC_ENABLE_GTEST ON)
add_subdirectory("${CMAKE_HOME_DIRECTORY}/lib/rapidcheck"
${CMAKE_HOME_DIRECTORY}/build/rapidcheck-build)

add_executable(pulsebind_test ${TEST_SRC})
target_link_libraries(pulsebind_test PUBLIC gtest_main rapidcheck rapidcheck_gtest pulsebind)
