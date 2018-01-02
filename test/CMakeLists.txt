SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

add_executable(main_test   ${pwd}/main_test.cpp)
add_executable(loader_test ${pwd}/loader_test.cpp
                           ${pwd}/../src/obj_loader.cpp
                           ${pwd}/../src/util.cpp)

target_link_libraries(main_test   gtest gtest_main)
target_link_libraries(loader_test gtest gtest_main)

add_test(NAME main_test  COMMAND main_test  )
add_test(NAME loader_test COMMAND loader_test)
