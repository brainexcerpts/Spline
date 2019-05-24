########
# Download freeglut: http://freeglut.sourceforge.net/

win32:CONFIG += win
win64:CONFIG += win
#CONFIG(release, debug|release): DEFINES += NDEBUG
DEFINES += "FREEGLUT_STATIC=1"

# needed by freeglut: -lwinmm  -lgdi32
win:LIBS += -lfreeglut_static -lopengl32 -lglu32 -lwinmm  -lgdi32



INCLUDEPATH += ./ ../

SOURCES += main.cpp

HEADERS += \
    vec3.hpp \
    ../splines.inl \
    ../splines.hpp
