CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(qtruov)
 
SET(CMAKE_BUILD_TYPE Release)
 
SET(CMAKE_CXX_FLAGS "-Wall")
 
# QT4 Handling
FIND_PACKAGE(Qt4 COMPONENTS QtGui QtCore QtOpengl QtSvg REQUIRED)
 
INCLUDE(${QT_USE_FILE})
 
INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
)

QT4_WRAP_UI( qtrucov_Qt_files
   proj_dia.ui
   mainwindow.ui
)
 
QT4_WRAP_CPP( qtrucov_Qt_files
   codeeditor.h
   highlighter.h
   mainwindow.h
   proj_dia.h
   svgview.h
   svgwindow.h
)

ADD_EXECUTABLE( qtrucov
   codeeditor.cpp
   highlighter.cpp
   main.cpp
   mainwindow.cpp
   proj_dia.cpp
   svgview.cpp
   svgwindow.cpp
	${qtrucov_Qt_files}
)

TARGET_LINK_LIBRARIES( qtrucov
   ${QT_LIBRARIES}
)

INSTALL( TARGETS qtrucov DESTINATION bin )
