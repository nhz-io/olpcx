#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libOPLC_Link], [0.1], [gogamoga@gmail.com], [libOLPC_Link])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([usb], [libusb_init],[LDFLAGS="$LDFLAGS -lusb"], [
	AC_CHECK_LIB([usb-1.0], [libusb_init],
		[LDFLAGS="$LDFLAGS -lusb-1.0"],
		[AC_MSG_ERROR(["Couldn't find libusb"])]
	)]
)

# Checks for header files.
AC_CHECK_HEADERS([stdint.h string.h])
AC_CHECK_HEADER([libusb.h],,
	[AC_CHECK_HEADER([libusb-1.0/libusb.h],
		[CPPFLAGS="$CPPFLAGS -I/usr/include/libusb-1.0"],
		[AC_MSG_ERROR(["Couldn't find libusb.h"])]
	)]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
