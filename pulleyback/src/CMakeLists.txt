find_package (Steamworks 0.8 REQUIRED)

add_library(pulleyback_erl MODULE
  pulleyback_erl.c
)
set_target_properties(pulleyback_erl PROPERTIES PREFIX "")

install(TARGETS pulleyback_erl
  LIBRARY DESTINATION share/steamworks/pulleyback
  )
