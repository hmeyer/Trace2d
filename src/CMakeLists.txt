SET(MYLIBS "~/devel/myLibs")
SET(SRC imageArea.h imageArea.cpp itkimagetypes.h
	watershed.cpp.h watershed.h
	glade/traceUI_glade.cc  glade/traceUI_glade.hh glade/traceUI.hh
	${MYLIBS}/itkbasics.h ${MYLIBS}/toString.h)
FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ENDIF(ITK_FOUND)


FIND_PACKAGE(Boost COMPONENTS signals REQUIRED)
INCLUDE(${Boost_INCLUDE_DIR})

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories( ${MYLIBS}
    ${GTKMM_INCLUDE_DIRS}  )

SET(Libraries
 ITKCommon
 ITKAlgorithms
 ITKIO
 ${GTKMM_LIBRARIES}
 ${Boost_LIBRARIES}
)


SET(CurrentExe "trace2d")
add_executable(${CurrentExe}
    main.cpp ${SRC}
    )

target_link_libraries(${CurrentExe}
    ${Libraries} )



