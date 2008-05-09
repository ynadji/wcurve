####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = wcurve

## INCLUDES were found outside kdevelop specific part

wcurve_SOURCES = plotview.cpp sequenceview.cpp displaylist.cpp wcurvedata.cpp vector3d.cpp sequenceparser.cpp repeatgapparser.cpp pentagapparser.cpp origingapparser.cpp nogapparser.cpp wcurveview.cpp wcurvedoc.cpp wcurve.cpp main.cpp 
SOURCES = plotview.cpp sequenceview.cpp displaylist.cpp wcurvedata.cpp vector3d.cpp sequenceparser.cpp repeatgapparser.cpp pentagapparser.cpp origingapparser.cpp nogapparser.cpp wcurveview.cpp wcurvedoc.cpp wcurve.cpp main.cpp 
INTERFACES = 
TRANSLATIONS = 
KDE_OPTIONS = qtonly
wcurve_LDADD   = $(LIB_QPE) /usr/X11R6/lib/libGLU.so $(LIB_QT) $(LIBSOCKET)

SUBDIRS = docs 

EXTRA_DIST = main.cpp wcurve.cpp wcurve.h wcurvedoc.cpp wcurvedoc.h wcurveview.cpp wcurveview.h lo32-app-wcurve.png lo16-app-wcurve.png filenew.xpm filesave.xpm fileopen.xpm globaltypes.h nogapparser.cpp nogapparser.h origingapparser.cpp origingapparser.h pentagapparser.cpp pentagapparser.h repeatgapparser.cpp repeatgapparser.h sequenceparser.cpp sequenceparser.h vector3d.cpp vector3d.h wcurvedata.cpp wcurvedata.h displaylist.cpp displaylist.h sequenceview.cpp sequenceview.h plotview.cpp plotview.h 

####### kdevelop will overwrite this part!!! (end)############


# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

wcurve_METASOURCES = AUTO

# the library search path. 
wcurve_LDFLAGS = $(all_libraries) 

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(wcurve_METASOURCES)

