include $(top_srcdir)/defs.mk

SRCDIR=$(top_srcdir)/src
LIBDBDIR=$(SRCDIR)/libdb

INCLUDES = -I$(IS_MTINCLUDES) -I$(SRCDIR) -I$(top_srcdir)/build/include -I$(IS_ILUINCLUDE) $(FIXED_INCLUDES) -I$(OCCI_INCLUDES)

test_PROGRAMS=Test
Test_SOURCES=Test.cpp Tests.h TestForeignKey.cpp TestDataBase.cpp TestOCCI.cpp TestIterator.cpp

Test_LDFLAGS=$(IS_ILU_CPPFLAGS)
Test_LDADD=\
	$(IS_ILU_CPPLDADD) \
	$(FIXED_LIBS) \
	$(OCCI_LIBS) \
	$(IS_CORELIBDIR)/libcommon_c.la \
	$(SRCDIR)/common/misc/libdatetime.la \
	$(LIBDBDIR)/oracle/liboracle.la \
	$(LIBDBDIR)/common/libdb.la \
	$(SRCDIR)/common/misc/libBase64.la \
	$(SRCDIR)/logger/liblogger.la \
	$(SRCDIR)/common/misc/libNumericAnyConversions.la
