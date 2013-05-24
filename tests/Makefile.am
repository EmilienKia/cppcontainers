## Process this file with automake to produce Makefile.in

# List of src files for CppUnit tests
CPPUNITTESTSRC = tape.cpp


if HAVE_CPPUNIT

check_PROGRAMS = unittests

unittests_CXXFLAGS = $(CPPUNIT_CFLAGS) -I$(top_srcdir)/include -DTOP_SRCDIR="\"$(top_srcdir)\""
unittests_LDFLAGS = $(CPPUNIT_LIBS)

unittests_SOURCES = $(CPPUNITTESTSRC) unittests.cpp

else !HAVE_CPPUNIT

EXTRA_DIST = $(CPPUNITTESTSRC) unittests.cpp

endif !HAVE_CPPUNIT