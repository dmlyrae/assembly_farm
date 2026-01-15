cmake_minimum_required(VERSION 3.14)

project(project_name VERSION 0.0.1)

# PATCH_VERSION может быть передан через -DPATCH_VERSION=... (например, из CI/CD)
if(NOT DEFINED PATCH_VERSION)
    set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
endif()
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

# Загружаем GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

# --- Определение целей (библиотека и исполняемый файл) ---
add_library(project_name src/lib.cpp)
add_executable(project_name_cli src/main.cpp)

# Устанавливаем стандарт C++
set_target_properties(project_name_cli project_name PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(project_name_cli PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"
)
target_include_directories(project_name PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"
)

target_link_libraries(project_name_cli PRIVATE project_name)

install(TARGETS project_name_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "dm.lyrae@ya.ru")
include(CPack)

enable_testing()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tests")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/tests")
endif()

# add_test(
#     NAME version_check_test
#     COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test_1.sh $<TARGET_FILE:helloworld_cli> ${PROJECT_VERSION}
# )

add_executable(project_name_tests tests/example_test.cpp)

target_include_directories(project_name_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"
)

target_link_libraries(project_name_tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    project_name
    # GTest::gmock
)

# Регистрируем тесты в CMake
include(GoogleTest)
gtest_discover_tests(project_name_tests)