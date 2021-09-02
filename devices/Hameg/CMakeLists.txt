add_library(TkModLabHameg SHARED
        HO820ComHandler.cpp
        VHameg8143.cpp
        Hameg8143Fake.cpp
        Hameg8143.cpp
)

set_target_properties(TkModLabHameg PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabHameg PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabHameg_test test.cc)
target_link_libraries(TkModLabHameg_test LINK_PUBLIC TkModLabHameg)
set_target_properties(TkModLabHameg_test PROPERTIES OUTPUT_NAME test)