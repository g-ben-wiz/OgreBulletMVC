noinst_HEADERS = Controller.h Datum.h View.h
 
bin_PROGRAMS = OgreBulletMVC
 OgreBulletMVC_CPPFLAGS = -I$(top_srcdir)
 OgreBulletMVC_SOURCES  = Datum.cpp View.cpp Controller.cpp MVCDemo.cpp
 OgreBulletMVC_CXXFLAGS = $(DEPENDENCIES_CFLAGS) 
 OgreBulletMVC_LDADD    = $(DEPENDENCIES_LIBS) -lboost_system
 
 AUTOMAKE_OPTIONS = foreign
