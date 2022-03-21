add_executable(TkModLabCanon_test test2.cc)
target_link_libraries(TkModLabCanon_test LINK_PUBLIC TkModLabCanon PkgConfig::LIBGPHOTO2 PkgConfig::EXIV2 ${OPENCV4_LIBRARIES})
set_target_properties(TkModLabCanon_test PROPERTIES OUTPUT_NAME test)

