cmake_minimum_required(VERSION 3.5)

include(./scripts/CPM.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(CMAKE_CXX_STANDARD 11)

set(LOGS_ENABLED OFF)
set(USE_CCACHE ON)
set(CCACHE_OPTIONS "CCACHE_CPP2=true")
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm_cache")

CPMAddPackage(
	NAME Ccache.cmake
	GITHUB_REPOSITORY TheLartians/Ccache.cmake
	VERSION 1.2.5
)

set(PROJECT_NAME test_video)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
project(${PROJECT_NAME})

include(./scripts/dependencies.cmake)

# Collect sources and headers
set(SRC_DIRS ${PROJECT_DIR}/src)
set(INC_DIRS ${PROJECT_DIR}/inc)

foreach(dir ${SRC_DIRS})
	file(GLOB_RECURSE PROJECT_SOURCES ${dir}/*.c*)
endforeach()

foreach(dir ${INC_DIRS})
	file(GLOB_RECURSE PROJECT_HEADERS ${dir}/*.h*)
endforeach()

if (LOGS_ENABLED)
	message(STATUS "Project ${PROJECT_NAME} INC_DIRS: ${INC_DIRS}")
	message(STATUS "Project ${PROJECT_NAME} EXPORT_INC_DIRS: ${EXPORT_INC_DIRS}")
endif()

process_deps(LIBRARIES LIBRARIES_INCLUDES)

if (LOGS_ENABLED)
	message(STATUS "Project ${PROJECT_NAME} LIBRARIES: ${LIBRARIES}")
	message(STATUS "Project ${PROJECT_NAME} LIBRARIES_INCLUDES: ${LIBRARIES_INCLUDES}")
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIRS})