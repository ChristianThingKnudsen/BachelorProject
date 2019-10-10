cmake_minimum_required(VERSION 3.5)

project(BachelorProjekt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(DCMTK_wrap)

find_package(OpenCL)

find_package(ITK REQUIRED COMPONENTS RTK)

include(${ITK_USE_FILE})

find_package(Plastimatch REQUIRED)

set(PLM_TARGETS
  plmbase
  plmutil
  plmdose
  plmbase
  plmregister
  plmreconstruct
  plmopencl
  plmsegment
)

add_executable(BachelorProjekt
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  qyklabel.cpp
  qyklabel.h
  progressbar.cpp
  progressbar.h
  progressbar.ui
  informationwindow.cpp
  informationwindow.h
  informationwindow.ui
  cbctregistration_test.cpp
  cbctregistration_test.hpp
  MyQComboBox.hxx
)

target_link_libraries(BachelorProjekt PRIVATE Qt5::Widgets)
target_link_libraries(BachelorProjekt PRIVATE ${ITK_LIBRARIES})

find_library(CBCTRECON_LIB CbctReconLibd.lib REQUIRED)
target_link_libraries(BachelorProjekt
    PRIVATE ${CBCTRECON_LIB}
    PRIVATE ${PLM_TARGETS}
    PRIVATE ${DCMTK_LIBRARIES}
    PRIVATE OpenCL::OpenCL
    )

set(CbctReconLib_INCLUDE_DIR "C:/Users/ct-10/CbctRecon/Library/CbctReconLib/" CACHE PATH "Path to include files of CbctReconLib")
set(CbctReconLib_BUILD_INCLUDE_DIR "C:/Users/ct-10/CbctRecon/build-vs19-mt/Library/CbctReconLib" CACHE PATH "Path to configuration files of CbctReconLib")
include_directories(
    ${CbctReconLib_INCLUDE_DIR}
    ${CbctReconLib_BUILD_INCLUDE_DIR}
    ${ITK_INCLUDE_DIRS}
    )
