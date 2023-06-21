cmake_minimum_required(VERSION 3.4)
project(StringList)

add_library(StringList SHARED stringlist.h)

write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
	VERSION 3.1.4
	COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}StringList.cmake.in
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}StringList.cmake
    INSTALL_DESTINATION .
    PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

install(
	FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
	DESTINATION .
)