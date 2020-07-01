AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

lib_LTLIBRARIES = libfapolicyd-analyzer.la
libfapolicyd_analyzer_la_SOURCES = \
	libfapolicyd-analyzer/detail/macros.h \
	libfapolicyd-analyzer/detail/utarray.h \
	libfapolicyd-analyzer/detail/uthash.h \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/field_array.c \
	libfapolicyd-analyzer/field_array.h \
	libfapolicyd-analyzer/match.c \
	libfapolicyd-analyzer/match.h \
	libfapolicyd-analyzer/parse.c \
	libfapolicyd-analyzer/parse.h \
	libfapolicyd-analyzer/rpm.c \
	libfapolicyd-analyzer/rpm.h \
	libfapolicyd-analyzer/sha256.c \
	libfapolicyd-analyzer/sha256.h \
	libfapolicyd-analyzer/table.c \
	libfapolicyd-analyzer/table.h

# Ask autotools to install the generated Python file.
python_PYTHON = $(srcdir)/bindings/python/fapolicyanalyzer.py

# This is the Python module.
pyexec_LTLIBRARIES = _fapolicyanalyzer.la

# It is composed of the wrapper C file and has other dependencies.
_fapolicyanalyzer_la_SOURCES = \
	$(srcdir)/bindings/python/fapolicyanalyzer_wrap.c \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/match.h \
	libfapolicyd-analyzer/field_array.h \
	libfapolicyd-analyzer/parse.h \
	libfapolicyd-analyzer/rpm.h \
	libfapolicyd-analyzer/sha256.h \
	libfapolicyd-analyzer/table.h

# -module tells libtool we don't want a 'lib' prefix.
_fapolicyanalyzer_la_LDFLAGS = -module ${PYTHON_LDFLAGS}

# C preprocessing flags to build the Python module.
_fapolicyanalyzer_la_CPPFLAGS = -I$(srcdir) ${PYTHON_CPPFLAGS}

# The Python module needs to link to the libfapolicyd-analyzer library.
_fapolicyanalyzer_la_LIBADD = libfapolicyd-analyzer.la

#sbin_PROGRAMS = fapolicyd-analyzer
#fapolicyd_analyzer_SOURCES = cli/fapolicyd-analyzer.c
#fapolicyd_analyzer_LDADD = libfapolicyd-analyzer.la
