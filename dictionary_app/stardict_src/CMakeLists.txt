INCLUDE_DIRECTORIES(${GLIBPATH})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS(-DHAVE_MMAP -DARM)

ADD_LIBRARY(stardict
collation.cpp
common.cpp
ctype-mb.cpp
ctype-uca.cpp
ctype-utf8.cpp
data.cpp
dictziplib.cpp
distance.cpp
kmp.cpp
lib.cpp
stddict.cpp
storage.cpp
)

TARGET_LINK_LIBRARIES(stardict z)
