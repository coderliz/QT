load(qttest_p4)
SOURCES  += tst_qimage.cpp

wince*: {
   addImages.sources = images/*
   addImages.path = images
   DEPLOYMENT += addImages
   DEFINES += SRCDIR=\\\".\\\"
} else {
   contains(QT_CONFIG, qt3support): QT += qt3support
   DEFINES += SRCDIR=\\\"$$PWD\\\"
}
