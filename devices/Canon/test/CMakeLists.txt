add_executable(TkModLabCanon_test test2.cc)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
      target_link_libraries(TkModLabCanon_test LINK_PUBLIC TkModLabCanon PkgConfig::LIBGPHOTO2 ${OpenCV_LIBS} PkgConfig::EXIV2)
else()
      target_link_libraries(TkModLabCanon_test LINK_PUBLIC TkModLabCanon PkgConfig::LIBGPHOTO2 PkgConfig::EXIV2 ${OPENCV4_LIBRARIES})
endif()

set_target_properties(TkModLabCanon_test PROPERTIES OUTPUT_NAME test)
