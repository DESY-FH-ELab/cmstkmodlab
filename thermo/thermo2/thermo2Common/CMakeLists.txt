set(CMAKE_AUTOMOC OFF)

add_library(Thermo2Common  SHARED
        Thermo2ThroughPlaneFitter.cc
)

set_target_properties(Thermo2Common PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(Thermo2Common PRIVATE PkgConfig::GSL)

add_executable(Thermo2Common_test test.cc)
target_link_libraries(Thermo2Common_test
	PRIVATE Thermo2Common
	PRIVATE PkgConfig::GSL)
set_target_properties(Thermo2Common_test PROPERTIES OUTPUT_NAME test)
