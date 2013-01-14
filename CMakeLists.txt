CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)


PROJECT(testengine)
ADD_LIBRARY(game SHARED game.c)
#SET_TARGET_PROPERTIES(testgame PROPERTIES COMPILE_FLAGS -D___BUILD_LINUX_GNU_X86_64___ -lX11 -lGL -lGLU -std=gnu99 -O3 -fomit-frame-pointer -ffast-math -mno-ieee-fp -fno-exceptions -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fpic)
SET_TARGET_PROPERTIES(game PROPERTIES COMPILE_FLAGS -D___BUILD_LINUX_GNU_X86_64___ -lX11 -lGL -lGLU -std=gnu99 -ffast-math -mno-ieee-fp -fno-exceptions -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fpic)
SET_TARGET_PROPERTIES(game PROPERTIES PREFIX "")
TARGET_LINK_LIBRARIES(game X11 glapi GL GLU)

PROJECT(hotloader)
ADD_EXECUTABLE(hotloader hotloader.c)
SET_TARGET_PROPERTIES(hotloader PROPERTIES COMPILE_FLAGS -D___BUILD_LINUX_GNU_X86_64___)
TARGET_LINK_LIBRARIES(hotloader dl X11 glapi GL GLU)
