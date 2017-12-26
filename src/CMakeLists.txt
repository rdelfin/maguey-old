SET(pwd ${CMAKE_CURRENT_LIST_DIR})

include_directories(${pwd}/../include)

FILE(GLOB src ${pwd}/*.cpp)

add_library(maguey STATIC ${src})
message(STATUS "maguey added")

target_link_libraries(maguey ${stdgl_libraries})
target_link_libraries(maguey SOIL) # TODO: Find a better solution

install(TARGETS maguey
        EXPORT magueyTargets
        DESTINATION lib)
install(DIRECTORY ${pwd}/../include/
        DESTINATION include)

