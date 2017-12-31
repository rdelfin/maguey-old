SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

add_executable(main_test   ${pwd}/main_test.cpp)
add_executable(loader_test ${pwd}/loader_test.cpp
                           ${pwd}/../src/obj_loader.cpp
                           ${pwd}/../src/util.cpp)

target_link_libraries(main_test   GTest::GTest GTest::Main)
target_link_libraries(loader_test GTest::GTest GTest::Main)

add_test(NAME mn_test  COMMAND main_test  )
add_test(NAME ldr_test COMMAND loader_test)
