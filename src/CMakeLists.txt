SET(MYLIBS "~/devel/myLibs")
SET(SRC main.cpp
	imageArea.h imageArea.cpp  BitmapImageArea.cpp VectorImageArea.cpp
	itkimagetypes.h VectorShape.cpp
	watershed.cpp watershed.h
	colormapper.cpp colormapper.h
	labelSelectorPixelAccessor.cpp labelSelectorPixelAccessor.h
	trace.cpp trace.h
	glade/traceUI_glade.cc  glade/traceUI_glade.hh glade/traceUI.hh
	${MYLIBS}/itkbasics.h ${MYLIBS}/toString.h)
FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ENDIF(ITK_FOUND)


FIND_PACKAGE(Boost COMPONENTS signals REQUIRED)
INCLUDE(${Boost_INCLUDE_DIR})


FIND_PATH(POTRACE_PATH potracelib.h
	../../potrace/potrace-1.8/src
)

link_directories(
    ${GTKMM_LIBRARY_DIRS} ${POTRACE_PATH} )

include_directories( ${MYLIBS}
    ${GTKMM_INCLUDE_DIRS} ${POTRACE_PATH}  )

SET(Libraries
 ITKCommon
 ITKAlgorithms
 ITKIO
 ${GTKMM_LIBRARIES}
 ${Boost_LIBRARIES}
 potrace
)


SET(CurrentExe "trace2d")
add_executable(${CurrentExe}
    ${SRC}
    )

target_link_libraries(${CurrentExe}
    ${Libraries} )



