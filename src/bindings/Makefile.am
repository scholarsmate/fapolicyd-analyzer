
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

BUILT_SOURCES = python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.c
CLEANFILES = python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.c

# Tell Make how to create the generated SWIG files for Python.
python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.c: fapolicyanalyzer.i
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -I${top_srcdir}/src -o $@ $<

