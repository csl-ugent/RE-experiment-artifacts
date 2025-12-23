file(GLOB cpp_src "*.cpp")
file(GLOB src "*.c")

add_library(CppWrapper SHARED
  ${cpp_src})

add_library(Shared STATIC
  ${src})
target_include_directories(Shared
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  )
target_link_libraries(Shared
  PUBLIC CppWrapper)
