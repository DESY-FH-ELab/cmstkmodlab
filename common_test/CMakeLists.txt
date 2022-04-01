set(CMAKE_AUTOMOC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)

add_executable(common_test
	main.cc
	CommonTestMainWindow.cc
)

target_link_libraries(common_test
	PRIVATE Qt5::Core
	PRIVATE Qt5::Widgets
	PRIVATE Common
)

INSTALL(
    TARGETS common_test
    EXPORT CommonTest
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

add_executable(testApplicationConfig testApplicationConfig.cc)
target_link_libraries (testApplicationConfig
	PRIVATE Qt5::Core
	PRIVATE Qt5::Widgets
	PRIVATE Common
)

add_executable(testRingbuffer testRingbuffer.cc)
target_link_libraries (testRingbuffer
	PRIVATE Qt5::Core
	PRIVATE Qt5::Widgets
	PRIVATE Common
)

add_executable(testFifo testFifo.cc)
target_link_libraries (testFifo
	PRIVATE Qt5::Core
	PRIVATE Qt5::Widgets
	PRIVATE Common
)

add_executable(testHistoryFifo testHistoryFifo.cc)
target_link_libraries (testHistoryFifo
	PRIVATE Qt5::Core
	PRIVATE Qt5::Widgets
	PRIVATE Common
)
