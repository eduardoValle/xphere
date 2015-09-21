find_package(Threads)

add_definitions(-Wall -g -pedantic -ggdb -lpthread -lcppdb_sqlite3)

add_executable(xphere main.c)

target_link_libraries(xphere mongoose)

target_link_libraries(xphere sqlite3)

target_link_libraries (xphere ${CMAKE_THREAD_LIBS_INIT})
