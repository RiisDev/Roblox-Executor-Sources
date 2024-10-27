add_subdirectory("fmt")
add_subdirectory("spdlog")

option(LUAU_BUILD_CLI OFF)
option(LUAU_BUILD_TESTS OFF)
add_subdirectory("luau")
