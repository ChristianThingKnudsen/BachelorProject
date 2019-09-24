cmake_minimum_required(VERSION 3.5)

project(BachelorProjekt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(ITK REQUIRED)

include(${ITK_USE_FILE})

add_executable(BachelorProjekt
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  qyklabel.cpp
  qyklabel.h
)

target_link_libraries(BachelorProjekt PRIVATE Qt5::Widgets)
target_link_libraries(BachelorProjekt PRIVATE ${ITK_LIBRARIES})

find_library(CBCTRECON_LIB CbctReconLibd.lib)
target_link_libraries(BachelorProjekt
    PRIVATE ${CBCTRECON_LIB}
    )
include_directories(
    "C:/Users/ct-10/OneDrive\ -\ Aarhus universitet/7\ Semester\ ST/Bachelor/AndreasKildeKode/cbctrecon/Library/CbctReconLib/"
    "C:/Users/ct-10/cbctrecon-build/Library/CbctReconLib"
    ${ITK_INCLUDE_DIRS}
    )
