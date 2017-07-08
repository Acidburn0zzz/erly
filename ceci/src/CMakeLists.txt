find_package(Erlang REQUIRED)

include_directories(${Erlang_INCLUDE_DIRS})
add_executable(ceci
	ceci.c
)
target_link_libraries(ceci ${Erlang_LIBRARIES})

