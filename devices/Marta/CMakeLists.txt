add_library(TkModLabMarta SHARED
        MartaComHandler.cpp
        VMarta.cpp
        MartaFake.cpp
        Marta.cpp
)

set_target_properties(TkModLabMarta PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabMarta PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

if(CMSTKMODLAB_MARTA)
        target_link_libraries(TkModLabMarta PkgConfig::MODBUS)
endif()

add_executable(TkModLabMarta_test test.cc)
target_link_libraries(TkModLabMarta_test LINK_PUBLIC TkModLabMarta)
set_target_properties(TkModLabMarta_test PROPERTIES OUTPUT_NAME test)
