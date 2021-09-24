set(CMAKE_AUTOMOC OFF)

set(CMAKE_SHARED_MODULE_PREFIX "")

include_directories(${PROJECT_SOURCE_DIR})

add_library(PyTkModLabNanotec MODULE PyTkModLabNanotec.cc)
target_include_directories(PyTkModLabNanotec PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(PyTkModLabNanotec ${Boost_LIBRARIES} ${Python3_LIBRARIES} TkModLabNanotec)

add_library(PyTkModLabHameg MODULE PyTkModLabHameg.cc)
target_include_directories(PyTkModLabHameg PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(PyTkModLabHameg ${Boost_LIBRARIES} ${Python3_LIBRARIES} TkModLabHameg)

add_library(PyTkModLabConrad MODULE PyTkModLabConrad.cc)
target_include_directories(PyTkModLabConrad PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(PyTkModLabConrad ${Boost_LIBRARIES} ${Python3_LIBRARIES} TkModLabConrad)

add_library(PyTkModLabLeybold MODULE PyTkModLabLeybold.cc)
target_include_directories(PyTkModLabLeybold PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(PyTkModLabLeybold ${Boost_LIBRARIES} ${Python3_LIBRARIES} TkModLabLeybold)
