set(LIBRARIES_DIR ${PROJECT_DIR}/../libraries)
set(LIBRARIES_BIN_DIR ${CMAKE_BINARY_DIR}/libraries)

message(STATUS "Libraries directory: ${LIBRARIES_DIR}")

function(process_deps OUT_LIBS OUT_INCS)
	# Initialize variables in the parent scope
	SET(L_LIBS "" PARENT_SCOPE)
	SET(L_INCS "" PARENT_SCOPE)

	LIST(APPEND L_LIBS glfw)
	CPMAddPackage(
			NAME glfw
			GITHUB_REPOSITORY glfw/glfw
			GIT_TAG 3.4
			OPTIONS
			"GLFW_BUILD_TESTS OFF"
			"GLFW_BUILD_EXAMPLES OFF"
			"GLFW_BUILD_DOCS OFF"
			"GLFW_BUILD_X11 ON"
			"GLFW_BUILD_WAYLAND OFF"
	)

	set(OpenGL_GL_PREFERENCE "LEGACY")
	# CPMAddPackage(
	# 	NAME glew
	# 	GITHUB_REPOSITORY Perlmint/glew-cmake
	# 	GIT_TAG 72ff6b1
	# 	QUIET
	# )
	# list(APPEND L_LIBS GLEW)
	# list(APPEND L_LIBS GLVND)

	CPMAddPackage(
		NAME glad
		GITHUB_REPOSITORY Dav1dde/glad
		GIT_TAG master
		OPTIONS
		"GLAD_GL_VERSION 4.6"
		"GLAD_PROFILE core"
		"GLAD_GENERATE_LOADER ON"
	)

	SET(${OUT_LIBS} ${L_LIBS} PARENT_SCOPE)
	SET(${OUT_INCS} ${L_INCS} PARENT_SCOPE)
endfunction()