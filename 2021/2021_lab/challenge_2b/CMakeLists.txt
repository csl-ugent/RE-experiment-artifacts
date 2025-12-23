set(TASK challenge_2b)

add_executable(${TASK}
  $<$<CONFIG:Debug>:main.c>
  $<$<CONFIG:Release>:obf-edited.c>
  )
target_include_directories(${TASK}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  )
target_link_libraries(${TASK}
  PRIVATE Shared
  )
