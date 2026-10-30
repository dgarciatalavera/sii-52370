CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT (TENIS)

include_directories(include)

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}")
add_executable(logger src/logger.cpp)
add_executable(bot src/bot.cpp src/Bot.cpp)
