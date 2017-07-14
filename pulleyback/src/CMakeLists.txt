add_library(pulleyback_erl MODULE
  pulleyback_erl.c
)
set_target_properties(pulleyback_erl PROPERTIES PREFIX "")
target_link_libraries(pulleyback_erl ${Erlang_LIBRARIES})

install(TARGETS pulleyback_erl
  LIBRARY DESTINATION share/steamworks/pulleyback
  )
