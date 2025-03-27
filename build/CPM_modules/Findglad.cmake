include("/dev_share/dev/test_video/build/cmake/CPM_0.40.2.cmake")
CPMAddPackage("NAME;glad;GITHUB_REPOSITORY;Dav1dde/glad;GIT_TAG;master;OPTIONS;GLAD_GL_VERSION 4.6;GLAD_PROFILE core;GLAD_GENERATE_LOADER ON")
set(glad_FOUND TRUE)