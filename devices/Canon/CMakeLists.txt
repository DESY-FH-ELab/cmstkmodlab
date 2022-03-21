set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(TkModLabCanon SHARED
      CameraComHandler.cc
      VEOS550D.cc
      EOS550D.cc
      EOS550DFake.cc
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")

set_target_properties(TkModLabCanon PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabCanon PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

target_include_directories(TkModLabCanon SYSTEM PRIVATE ${OPENCV4_INCLUDE_DIRS})
target_link_libraries(TkModLabCanon PkgConfig::LIBGPHOTO2 PkgConfig::EXIV2 ${OPENCV4_LIBRARIES})
target_compile_features(TkModLabCanon PRIVATE cxx_std_11)

#add_executable(TkModLabAgilent_test test.cc)
#target_link_libraries(TkModLabAgilent_test LINK_PUBLIC TkModLabAgilent)
#set_target_properties(TkModLabAgilent_test PROPERTIES OUTPUT_NAME test)

add_subdirectory(test)
