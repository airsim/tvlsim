# ZeroMQ Server
airinv_svr_zmq_h_sources =
airinv_svr_zmq_cc_sources = \
	$(top_srcdir)/airinv/server/AirInvServer.cpp

# ZeroMQ Client
airinv_clt_zmq_h_sources =
airinv_clt_zmq_cc_sources = \
	$(top_srcdir)/airinv/server/AirInvClient.cpp

# Boost.ASIO Server
airinv_svr_asio_h_sources = \
	$(top_srcdir)/airinv/server/header.hpp \
	$(top_srcdir)/airinv/server/Connection.hpp \
	$(top_srcdir)/airinv/server/Request.hpp \
	$(top_srcdir)/airinv/server/Reply.hpp \
	$(top_srcdir)/airinv/server/RequestHandler.hpp \
	$(top_srcdir)/airinv/server/AirInvServer.hpp
airinv_svr_asio_cc_sources = \
	$(top_srcdir)/airinv/server/Connection.cpp \
	$(top_srcdir)/airinv/server/Request.cpp \
	$(top_srcdir)/airinv/server/RequestHandler.cpp \
	$(top_srcdir)/airinv/server/Reply.cpp \
	$(top_srcdir)/airinv/server/win_main.cpp \
	$(top_srcdir)/airinv/server/posix_main.cpp \
	$(top_srcdir)/airinv/server/AirInvServer_ASIO.cpp

# Boost.ASIO Client
airinv_clt_asio_h_sources =
airinv_clt_asio_cc_sources = \
	$(top_srcdir)/airinv/server/AirInvClient_ASIO.cpp
