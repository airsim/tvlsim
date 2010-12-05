## airinv/server sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

# Binaries (client/server)
bin_PROGRAMS = airinvServer airinvClient

airinvServer_SOURCES = $(airinv_svr_h_sources) $(airinv_svr_cc_sources)
airinvServer_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinvServer_LDADD =
airinvServer_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_ASIO_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la

airinvClient_SOURCES = $(airinv_clt_h_sources) $(airinv_clt_cc_sources)
airinvClient_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinvClient_LDADD =
airinvClient_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_ASIO_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)  $(STDAIR_LIBS)
