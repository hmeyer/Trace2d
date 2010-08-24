SET(MYLIBS "~/devel/myLibs")
SET(SRC itk2graphicsscene.cpp 
	main.cpp
	tracewindow.cpp
	VectorShape.cpp
	ImageTracer.cpp
	colormapper.cpp
	labelSelectorPixelAccessor.cpp
	trace.cpp
	)


link_directories(
    ${GTKMM_LIBRARY_DIRS} ${POTRACE_PATH} )

INCLUDE(${ITK_USE_FILE})
INCLUDE(${Boost_INCLUDE_DIR})
INCLUDE(${QT_USE_FILE})
include_directories( ${MYLIBS}
    ${POTRACE_PATH}  )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


SET( CLIENT_MOC_SRCS
  tracewindow.h
)

# build ui_XXX files from the XML-style .ui files
FILE(GLOB QT4_UIS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
QT4_WRAP_UI(SRC ${QT4_UIS})

# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(SRC ${CLIENT_MOC_SRCS})


SET(Libraries
 ITKCommon
 ITKAlgorithms
 ITKIO
 ${Boost_LIBRARIES}
 ${QT_LIBRARIES}
 potrace
)


SET(CurrentExe "trace2d")
add_executable(${CurrentExe}
    ${SRC}
    ${QT4_UIS_H}
    )

target_link_libraries(${CurrentExe}
    ${Libraries}
    )



