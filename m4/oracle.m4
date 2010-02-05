dnl ==========================================================================
dnl Oracle C++ Call Interface configuration. 
dnl ==========================================================================

AC_SUBST(OCCI_INCLUDES)
AC_SUBST(OCCI_LIBS)

OCCI_INCLUDES=
OCCI_LIBS=

AC_DEFUN([AC_PROG_OCCI], [

dnl===============================================================================
dnl occi-headers
AC_ARG_WITH(occi-headers,
[  --with-occi-headers    root directory where OCCI's headers installed],
if test "$withval" != "no" -a "$withval" != "yes"
then
    occi_headers_dir=$withval
fi
)

found=no
AC_MSG_CHECKING(for OCCI headers)
if test -z "$occi_headers_dir"
then
  for occi_headers_dir in /usr /usr/local /usr/lib /usr/lib/oracle/xe /usr/lib/oracle/xe/app/oracle/product/10.2.0/
  do
    if test -f "$occi_headers_dir/occi.h"
    then
      break
    fi
  done
fi

if test -f "$occi_headers_dir/occi.h"
then
  AC_MSG_RESULT(${occi_headers_dir})
else
  AC_MSG_RESULT(none)
  AC_MSG_ERROR("Cannot find occi.h directory! Use --with-occi-headers=path option.")
fi

OCCI_INCLUDES="${occi_headers_dir}"
dnl===============================================================================
dnl occi-libs (libocci.so.11.1)
AC_ARG_WITH(occi-libs,
[  --with-occi-libs    root directory where OCCI's libs installed],
if test "$withval" != "no" -a "$withval" != "yes"
then
    occi_libs_dir=$withval
fi
)

found=no
AC_MSG_CHECKING(for OCCI library)
if test -z "$occi_libs_dir"
then
  for occi_libs_dir in /usr /usr/local /usr/lib
  do
    if test -f "$occi_libs_dir/libociei.so"
    then
      break
    fi
  done
fi

if test -f "$occi_libs_dir/libociei.so"
then
  occi_lib=`echo $occi_libs_dir"/libclntsh.so*" $occi_libs_dir"/libocci.so*" $occi_libs_dir"/libociei.so*" $occi_libs_dir"/libnnz*.so*"`
  AC_MSG_RESULT($occi_libs_dir)
else
  AC_MSG_RESULT(none)
  AC_MSG_ERROR("Cannot find OCCI library directory! Use --with-occi-libs=path option.")
fi

OCCI_LIBS="${occi_lib}"
])
