SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

add_executable(main_test ${pwd}/main_test.cpp)
target_link_libraries(main_test gtest_main)
add_test(NAME example_test COMMAND main_test)
