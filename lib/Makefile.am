AM_CPPFLAGS = -I.. -I$(srcdir)/../ondragc

lib_LTLIBRARIES = libondragc.la
libondragc_la_SOURCES = \
    history.cpp \
    history.h \
    manager.cpp \
    object.cpp \
    objectptr.cpp \
    pointer.cpp \
    protoobject.cpp \
    referent.cpp
libondragc_la_LDFLAGS = -version-info `echo $(PACKAGE_VERSION) | tr . :`
includelibondragc_ladir = $(includedir)/ondragc
includelibondragc_la_HEADERS = \
    ../ondragc/asgctypeptrdecl.h \
    ../ondragc/asgctypeptrimpl.h \
    ../ondragc/ctor.h \
    ../ondragc/gcassert.h \
    ../ondragc/manager.h \
    ../ondragc/object.h \
    ../ondragc/objectptr.h \
    ../ondragc/pointer.h \
    ../ondragc/pointerdecl.h \
    ../ondragc/pointerimpl.h \
    ../ondragc/protoobject.h \
    ../ondragc/referent.h
