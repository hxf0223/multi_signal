file(GLOB multi_signal_srcs *.cc)
add_executable(multi_signal ${multi_signal_srcs})

find_package(Threads)
target_link_libraries(multi_signal ${CMAKE_THREAD_LIBS_INIT})
