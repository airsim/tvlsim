## simfqt/command sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

#
noinst_LTLIBRARIES = libsimfqtcmd.la

libsimfqtcmd_la_SOURCES= $(simfqt_cmd_h_sources) $(simfqt_cmd_cc_sources) 
libsimfqtcmd_la_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libsimfqtcmd_la_LIBADD =
libsimfqtcmd_la_LDFLAGS =

#
#pkgincludedir = $(includedir)/@PACKAGE@/command
#pkginclude_HEADERS = $(simfqt_cmd_h_sources)
