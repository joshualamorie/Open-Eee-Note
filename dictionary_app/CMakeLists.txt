CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(PLATFORM arm)
SET(CMAKE_C_COMPILER arm-linux-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-g++)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=iwmmxt -mtune=iwmmxt -mabi=aapcs-linux")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -shared-libgcc -L/usr/local/arm/lib/ -L/usr/local/arm/usr/lib/ -L/usr/gstlib/lib/ -L${PROJECT_SOURCE_DIR}/libs/")

SET(GLIBPATH /usr/gstlib/include/glib-2.0/ /usr/lib/glib-2.0/include)
SET(DICT_INSTALL_DIR ${PROJECT_SOURCE_DIR}/install/)
SET(CPP_SRCS dictionary.cpp homedict.cpp readerdict.cpp Config.cpp scandict.cpp CDictionaryCollectorItem.cpp)
SET(MOC_HDRS dictionary.h readerdict.h homedict.h CDictionaryCollectorItem.h)

ADD_SUBDIRECTORY(stardict_src)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${GLIBPATH} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/stardict_src)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})
QT4_WRAP_UI(UI_SRCS dictionary_home.ui dictionary_reader.ui)
QT4_ADD_RESOURCES(QRC_SRCS data/dicres.qrc)

ADD_EXECUTABLE(dictionary ${CPP_SRCS} ${MOC_SRCS} ${UI_SRCS} ${QRC_SRCS})

TARGET_LINK_LIBRARIES(dictionary ${QT_LIBRARIES} stardict commondlg bottombar glib-2.0 iconv z ts sqlite3)

INSTALL(TARGETS dictionary RUNTIME DESTINATION ${DICT_INSTALL_DIR})
