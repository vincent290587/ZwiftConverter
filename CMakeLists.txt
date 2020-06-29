cmake_minimum_required (VERSION 3.5.1)

project (Zwifter LANGUAGES CXX)

set(CMAKE_COLOR_MAKEFILE                        OFF)

if(CMAKE_HOST_UNIX)
add_compile_options(-std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti -fpermissive -rdynamic)
else()
#add_compile_options(-std=gnu++0x)
add_definitions(-DWIN32)
add_definitions(-D_WIN32_WINNT=0x0600)
add_definitions(-DWINVER=0x600)
add_definitions(-D_USE_MATH_DEFINES)
endif()


# add the binary tree to the search path for include files
aux_source_directory(./src/                   SRCS)
 
 
 
# add the executable
add_executable(Zwifter ${SRCS})


if(CMAKE_HOST_UNIX)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND size --format=berkeley "${PROJECT_NAME}"
    COMMENT "Invoking: Print Size")

else()

target_link_libraries(Zwifter imagehlp)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND size --format=berkeley "${PROJECT_NAME}.exe"
#    COMMENT "Invoking: Print Size")

endif()

add_custom_target(debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
	)
add_dependencies(debug Zwifter)

add_custom_target(release
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Switch CMAKE_BUILD_TYPE to Release"
	)
add_dependencies(release Zwifter)
