cmake_minimum_required(VERSION 3.12)
project(cpp_engine)

#
# Manually specifying all the header files.
#
set(HEADER_FILES
	GameObject.hpp
	Physics.hpp
	)

#
# Manually specifying all the source files.
#
set(SOURCE_FILES
	GameObject.cpp
	Physics.cpp
	)


#
# This is the main target
#
add_executable(engine ${HEADER_FILES} ${SOURCE_FILES} main.cpp)


#
# We want the C++ 20 standard for our project
#
set_property(TARGET engine PROPERTY CXX_STANDARD 20)


#
# Enable a lot of warnings for both compilers, forcing the developer to write better code
#
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(engine PRIVATE /W4 /permissive-)  #/WX will treat warnings as errors
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(engine PRIVATE -Wall -Wextra -pedantic -O3) # -Wconversion -Wsign-conversion
endif()

#
# Prepare a pre-build step to run clang-format over all the [ch]pp source files.
# Start by finding the location of the clang-format executable.
#
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    message("Clang-Format found at: " ${CLANG_FORMAT})

    #
    # Need to take the simple source file locations used for the project and get their full
    # file system locations for use in putting together the clang-format command line
    #
    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()

    #
    # This creates the clang-format target/command
    #
    add_custom_target(
        ClangFormat
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${SOURCE_FILES_PATHS}
    )
    #
    # This makes the clang-format target a dependency of the main HelloWorld project
    #
    add_dependencies(engine ClangFormat)
else()
    message("Unable to find clang-format")
endif()
