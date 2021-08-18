add_library(TkModLabLang SHARED
        LStepExpressComHandler.cpp
        VLStepExpress.cpp
        LStepExpressFake.cpp
        LStepExpress.cpp
)

set_target_properties(TkModLabLang PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabLang PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabLang_test test.cc)
target_link_libraries(TkModLabLang_test LINK_PUBLIC TkModLabLang)
set_target_properties(TkModLabLang_test PROPERTIES OUTPUT_NAME test)