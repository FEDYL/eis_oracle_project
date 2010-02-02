if portbuild
SUBDIRS=common mysql oracle
else
SUBDIRS=common mysql oracle mocks
endif

DIST_SUBDIRS=$(SUBDIRS)


dbincludedir=$(includedir)/libdb

dbinclude_HEADERS=libdb.hpp
