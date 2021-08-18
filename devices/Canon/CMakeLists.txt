add_library(TkModLabCanon SHARED
      CameraComHandler.cc
      VEOS550D.cc
      EOS550D.cc
      EOS550DFake.cc
)

set_target_properties(TkModLabCanon PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabCanon PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

#add_executable(TkModLabAgilent_test test.cc)
#target_link_libraries(TkModLabAgilent_test LINK_PUBLIC TkModLabAgilent)
#set_target_properties(TkModLabAgilent_test PROPERTIES OUTPUT_NAME test)

add_subdirectory(test)
