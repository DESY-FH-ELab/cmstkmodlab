if(CMSTKMODLAB_THERMO1)
        add_subdirectory(thermo1)
endif()

if(CMSTKMODLAB_THERMO2)
        add_subdirectory(thermo2)
endif()

add_subdirectory(thermoDAQ2Root)
add_subdirectory(thermoDAQ2RootConverter)
add_subdirectory(thermoDAQ2Log)
