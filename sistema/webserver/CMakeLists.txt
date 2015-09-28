find_package(Threads)

add_definitions(-Wall -g -pedantic -ggdb -lpthread)

add_executable(xphere main.c)

target_link_libraries(xphere resources)

target_link_libraries(xphere webserver)

target_link_libraries (xphere ${CMAKE_THREAD_LIBS_INIT})
