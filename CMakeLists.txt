cmake_minimum_required(VERSION 3.1)
project(downloader)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
set (CMAKE_CXX_STANDARD 11)

add_executable(downloader main.cpp SettingsHandler.cpp FileDownloader.cpp)

target_link_libraries(downloader curl)

install(TARGETS downloader RUNTIME DESTINATION bin)
