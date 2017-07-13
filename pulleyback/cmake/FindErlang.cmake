#.rst:
#
# FindErlang
# ----------
#
# Try to find the Erlang runtime and C bindings.
#
# This will define the following variables:
#
# ``Erlang_FOUND``
#    True if Erlang is available.
#
# ``Erlang_INCLUDE_DIRS``
#    Directories to add to the include path for Erlang C binding headers.
#
#
message(STATUS "Looking for Erlang ..")

find_program(Erlang_EXECUTABLE NAMES erlc)

# Directories where erlang might live; list library directories, include
# directories, everything.
set(_erlang_places
    /usr/local/lib/erlang/usr/include
    /usr/local/lib/erlang/usr/lib
    )

find_path(_ei_dir ei.h HINTS ${_erlang_places})
find_path(_erl_dir erl_interface.h HINTS ${_erlang_places})

if(_ei_dir AND _erl_dir)
    if(_ei_dir STREQUAL _erl_dir)
        set(Erlang_INCLUDE_DIRS ${_ei_dir})
    else()
        set(Erlang_INCLUDE_DIRS ${_ei_dir} ${_erl_dir})
    endif()
    message(STATUS " .. includes ${_ei_dir}")
endif()

find_library(_ei_lib
    NAMES ei
    HINTS ${_erlang_places}
)
find_library(_erl_lib
    NAMES erl_interface
    HINTS ${_erlang_places}
)
if (_ei_lib AND _erl_lib)
    # These are generally static, and erl_ derpend on ei,
    # so take care with the order.
    set(Erlang_LIBRARIES ${_erl_lib} ${_ei_lib} pthread)
    message(STATUS " .. libraries ${_ei_lib} ${_erl_lib}")
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Erlang
    FOUND_VAR
        Erlang_FOUND
    REQUIRED_VARS
        Erlang_EXECUTABLE
        Erlang_INCLUDE_DIRS
        Erlang_LIBRARIES
)
