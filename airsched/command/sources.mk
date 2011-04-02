airsched_cmd_h_sources = \
	$(top_srcdir)/airsched/command/Simulator.hpp \
	$(top_srcdir)/airsched/command/ScheduleParser.hpp \
	$(top_srcdir)/airsched/command/ScheduleParserHelper.hpp \
	$(top_srcdir)/airsched/command/OnDParser.hpp \
	$(top_srcdir)/airsched/command/OnDParserHelper.hpp \
	$(top_srcdir)/airsched/command/TravelSolutionParser.hpp \
	$(top_srcdir)/airsched/command/SegmentPathGenerator.hpp \
	$(top_srcdir)/airsched/command/InventoryGenerator.hpp \
	$(top_srcdir)/airsched/command/OnDPeriodGenerator.hpp \
	$(top_srcdir)/airsched/command/SegmentPathProvider.hpp
airsched_cmd_cc_sources = \
	$(top_srcdir)/airsched/command/Simulator.cpp \
	$(top_srcdir)/airsched/command/ScheduleParser.cpp \
	$(top_srcdir)/airsched/command/ScheduleParserHelper.cpp \
	$(top_srcdir)/airsched/command/SegmentPathGenerator.cpp \
	$(top_srcdir)/airsched/command/OnDParser.cpp \
	$(top_srcdir)/airsched/command/OnDParserHelper.cpp \
	$(top_srcdir)/airsched/command/TravelSolutionParser.cpp \
	$(top_srcdir)/airsched/command/InventoryGenerator.cpp \
	$(top_srcdir)/airsched/command/OnDPeriodGenerator.cpp \
	$(top_srcdir)/airsched/command/SegmentPathProvider.cpp
