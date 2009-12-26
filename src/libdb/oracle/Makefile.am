include $(top_srcdir)/defs.mk

INCLUDES = -I$(top_srcdir)/src \
           -I$(top_srcdir)/src/libdb \
           -I$(IS_MTINCLUDES) \
           -I$(top_srcdir)/src \
           -I$(IS_COREINCLUDEDIR) \
           -I$(IS_ILUINCLUDE) \
           -I$(OCCI_INCLUDES) \
           -I$(BOOST_INCLUDES) \
		   $(FIXED_INCLUDES)

lib_LTLIBRARIES = liboracle.la
liboracle_la_SOURCES = factory.cpp connection.cpp resultset.cpp field.cpp sequence.cpp transaction.cpp oracleconnection.cpp
liboracle_la_LDFLAGS = $(IS_ILULDFLAGS) $(IS_MTLDFLAGS)
liboracle_la_LIBADD = $(IS_ILUCPPLDADD) $(IS_MTLIBS) $(FIXED_LIBS)

oracledbincludedir=$(includedir)/libdb/oracle

oracledbinclude_HEADERS=connection.hpp factory.hpp field.hpp internalconnection.hpp resultset.hpp sequence.hpp transaction.hpp oracleconnection.hpp fielddef.hpp
