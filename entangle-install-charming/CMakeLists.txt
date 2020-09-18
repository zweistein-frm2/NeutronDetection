﻿# CMakeList.txt : CMake project for Boost_Installer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)


project ("entangle-install-charming")
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

set(CMAKE_CXX_STANDARD 17)
message(CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
file(STRINGS "${CMAKE_SOURCE_DIR}/ENTANGLE_FILES" ENTANGLE_FILES_RC)

set(RESOURCE_FILES "")
list(LENGTH ENTANGLE_FILES_RC  LL)
message(LIST_LENGTH="${LL}")

FOREACH(FPATH ${ENTANGLE_FILES_RC})
 message(${FPATH})
 string(REGEX MATCH  "[\\[]" isdestdir "${FPATH}")
 message(isdestdir="${isdestdir}")
 if("${isdestdir}"  STREQUAL "[")
   message("is a destdirectory")
 else()
 get_filename_component(barename ${FPATH} NAME )
  MESSAGE("<<${barename}>>")
  if(NOT DEFINED ENTANGLE_FILES_DO_NOT_COPY)
  file(COPY ${FPATH}  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/rcfiles)
  endif()
  list(APPEND RESOURCE_FILES rcfiles/${barename})
 endif()

ENDFOREACH()


include(cmrc/CMakeRC.cmake)
cmrc_add_resource_library(installer-charming ALIAS installer::rc NAMESPACE resources ${RESOURCE_FILES})
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.71 REQUIRED COMPONENTS filesystem chrono thread  iostreams ${BOOST_PATHS}) # header only libraries must not be added here
include_directories("${Boost_INCLUDE_DIRS}" )

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)
#include_directories(${Charm_DIR}) # need this only for Zweistein.HomePath.hpp
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "entangle-install-charming.cpp" )
target_compile_options(${PROJECT_NAME} PRIVATE  $<$<CXX_COMPILER_ID:GNU>:-static-libstdc++>)
target_compile_options(${PROJECT_NAME} PRIVATE  $<$<CXX_COMPILER_ID:GNU>: -Wno-error>)

target_link_libraries(${PROJECT_NAME} PRIVATE installer::rc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
target_link_libraries( ${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} )
# TODO: Add tests and install targets if needed.


#add_custom_command(TARGET ${PROJECT_NAME}
  #                 POST_BUILD
   #                COMMAND make package
    #              )



install(TARGETS ${PROJECT_NAME}
    DESTINATION "${INSTALL_DIR}"
    RUNTIME DESTINATION  "${INSTALL_DIR}"
    )