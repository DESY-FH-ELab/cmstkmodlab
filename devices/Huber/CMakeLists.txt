add_library(TkModLabHuber SHARED
        PetiteFleurComHandler.cpp
        VHuberPetiteFleur.cpp
        HuberPetiteFleurFake.cpp
        HuberPetiteFleur.cpp
        PilotOneComHandler.cpp
        VHuberPilotOne.cpp
        HuberPilotOneFake.cpp
        HuberPilotOne.cpp
)

set_target_properties(TkModLabHuber PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabHuber PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabHuber_test test.cc)
target_link_libraries(TkModLabHuber_test LINK_PUBLIC TkModLabHuber)
set_target_properties(TkModLabHuber_test PROPERTIES OUTPUT_NAME test)