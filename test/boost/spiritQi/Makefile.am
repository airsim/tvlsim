## command sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = employee_parser action_parser complex_parser \
	num_list1_parser roman_parser sum_parser \
	schedule_parser stream_iterator_parser \
	date_parser

employee_parser_SOURCES = employee_parser.cpp
employee_parser_CXXFLAGS = $(BOOST_CFLAGS)
employee_parser_LDFLAGS = $(BOOST_LIBS)

action_parser_SOURCES = action_parser.cpp
action_parser_CXXFLAGS = $(BOOST_CFLAGS)
action_parser_LDFLAGS = $(BOOST_LIBS)

complex_parser_SOURCES = complex_parser.cpp
complex_parser_CXXFLAGS = $(BOOST_CFLAGS)
complex_parser_LDFLAGS = $(BOOST_LIBS)

num_list1_parser_SOURCES = num_list1_parser.cpp
num_list1_parser_CXXFLAGS = $(BOOST_CFLAGS)
num_list1_parser_LDFLAGS = $(BOOST_LIBS)

roman_parser_SOURCES = roman_parser.cpp
roman_parser_CXXFLAGS = $(BOOST_CFLAGS)
roman_parser_LDFLAGS = $(BOOST_LIBS)

sum_parser_SOURCES = sum_parser.cpp
sum_parser_CXXFLAGS = $(BOOST_CFLAGS)
sum_parser_LDFLAGS = $(BOOST_LIBS)

schedule_parser_SOURCES = schedule_parser.cpp
schedule_parser_CXXFLAGS = $(BOOST_CFLAGS)
schedule_parser_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)

stream_iterator_parser_SOURCES = stream_iterator_parser.cpp
stream_iterator_parser_CXXFLAGS = $(BOOST_CFLAGS)
stream_iterator_parser_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)

date_parser_SOURCES = date_parser.cpp
date_parser_CXXFLAGS = $(BOOST_CFLAGS)
date_parser_LDADD = 
date_parser_LDFLAGS = $(BOOST_LIBS) $(BOOST_DATE_TIME_LIB)

