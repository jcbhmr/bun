cmake_minimum_required(VERSION 3.29)

project(bun-cosmo LANGUAGES C CXX)

include(cmake/CPM.cmake)
CPMAddPackage("gh:batterycenter/embed@1.2.19")

if(NOT BUILD_TESTING)
  set(B_PRODUCTION_MODE ON)
endif()
if(NOT BUN_RELEASES)
  set(BUN_RELEASES ".")
endif()

add_executable(bun src/main.cpp src/zip/zip.c)
b_embed(bun "${BUN_RELEASES}/bun-darwin-aarch64.zip")
b_embed(bun "${BUN_RELEASES}/bun-darwin-x64-baseline.zip")
b_embed(bun "${BUN_RELEASES}/bun-linux-aarch64.zip")
b_embed(bun "${BUN_RELEASES}/bun-linux-x64-baseline.zip")
b_embed(bun "${BUN_RELEASES}/bun-windows-x64-baseline.zip")
target_compile_features(bun PRIVATE cxx_std_23)
target_compile_features(bun PRIVATE c_std_23)
target_compile_definitions(bun PUBLIC BUN_RELEASES="${BUN_RELEASES}")
