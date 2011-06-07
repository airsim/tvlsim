## airinv/server sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Binaries (client/server)
ZEROMQ_PRG = airinvServer airinvClient
ASIO_PRG = airinvServer_ASIO airinvClient_ASIO
bin_PROGRAMS = $(ZEROMQ_PRG) $(ASIO_PRG)

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${bin_PROGRAMS:%=%.log}

# ZeroMQ Server
airinvServer_SOURCES = $(airinv_svr_zmq_h_sources) $(airinv_svr_zmq_cc_sources)
airinvServer_CXXFLAGS = $(BOOST_CFLAGS) $(ZEROMQ_CFLAGS) $(STDAIR_CFLAGS)
airinvServer_LDADD =
airinvServer_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_PROGRAM_OPTIONS_LIB) $(ZEROMQ_LIBS) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la

# ZeroMQ Client
airinvClient_SOURCES = $(airinv_clt_zmq_h_sources) $(airinv_clt_zmq_cc_sources)
airinvClient_CXXFLAGS = $(BOOST_CFLAGS) $(ZEROMQ_CFLAGS) $(STDAIR_CFLAGS)
airinvClient_LDADD =
airinvClient_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_PROGRAM_OPTIONS_LIB) $(ZEROMQ_LIBS) $(STDAIR_LIBS)

# Boost.ASIO Server
airinvServer_ASIO_SOURCES = \
	$(airinv_svr_asio_h_sources) $(airinv_svr_asio_cc_sources)
airinvServer_ASIO_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinvServer_ASIO_LDADD =
airinvServer_ASIO_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_ASIO_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/airinv/core/libairinv.la

# Boost.ASIO Client
airinvClient_ASIO_SOURCES = \
	$(airinv_clt_asio_h_sources) $(airinv_clt_asio_cc_sources)
airinvClient_ASIO_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
airinvClient_ASIO_LDADD =
airinvClient_ASIO_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB) \
	$(BOOST_ASIO_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)  $(STDAIR_LIBS)
