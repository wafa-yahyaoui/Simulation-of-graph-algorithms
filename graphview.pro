QT += widgets

HEADERS += \
        edge.h \
        node.h \
        graphwidget.h \
    window.h \
    window_delete_edge.h \
    wait.h

SOURCES += \
        edge.cpp \
        main.cpp \
        node.cpp \
        graphwidget.cpp \
    window.cpp \
    window_delete_edge.cpp \
    wait.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target
