AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

lib_LTLIBRARIES = libfapolicyd-analyzer.la
libfapolicyd_analyzer_la_SOURCES = \
	libfapolicyd-analyzer/detail/macros.h \
	libfapolicyd-analyzer/detail/utarray.h \
	libfapolicyd-analyzer/detail/uthash.h \
	libfapolicyd-analyzer/error_codes.h \
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

# This is the Python module interfacing with libfapolicyd-analyzer.
# fapolicyanalyzer.py and fapolicyanalyzer_wrap.c are created by SWIG from bindings/fapolicyanalyzer.i.
# Put these in the build tree.
BUILT_SOURCES = $(builddir)/fapolicyanalyzer.py $(builddir)/fapolicyanalyzer_wrap.c

CLEANFILES =  $(builddir)/fapolicyanalyzer.py $(builddir)/fapolicyanalyzer_wrap.c

# Tell Make how to create the generated SWIG files
$(builddir)/fapolicyanalyzer.py $(builddir)/fapolicyanalyzer_wrap.c: bindings/fapolicyanalyzer.i
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -I$(srcdir) -o $@ $<

# Ask autotools to install the generated Python file.
python_PYTHON = $(builddir)/fapolicyanalyzer.py

# This is the Python module.
pyexec_LTLIBRARIES = _fapolicyanalyzer.la

# It is composed of the generated C file and has other dependencies.
_fapolicyanalyzer_la_SOURCES = \
	$(builddir)/fapolicyanalyzer_wrap.c \
	bindings/fapolicyanalyzer.i \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/match.h \
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
