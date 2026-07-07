if(CMSTKMODLAB_THERMO1)
        add_subdirectory(thermo1)
endif()

if(CMSTKMODLAB_THERMO2)
        add_subdirectory(thermo2)
endif()

add_subdirectory(thermoDAQ2Root)
if(CMSTKMODLAB_THERMO_HDF5)
		add_subdirectory(thermoDAQ2HDF5)
endif()
add_subdirectory(thermoDAQ2RootConverter)
add_subdirectory(thermoDAQ2Log)
