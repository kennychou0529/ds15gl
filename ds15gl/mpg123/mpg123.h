/*
	mpg123_msvc: MPEG Audio Decoder library wrapper header for MS VC++ 2005

	copyright 2008 by the mpg123 project - free software under the terms of the LGPL 2.1
	initially written by Patrick Dehne and Thomas Orgis.
*/
#ifndef MPG123_MSVC_H
#define MPG123_MSVC_H

#include <tchar.h>
#include <stdlib.h>
#include <sys/types.h>

typedef long ssize_t;

#define MPG123_NO_CONFIGURE
#include "mpg123.h.in" /* Yes, .h.in; we include the configure template! */

#ifdef __cplusplus
extern "C" {
#endif

	// Wrapper around mpg123_open that supports path names with unicode
	// characters
	EXPORT int mpg123_topen(mpg123_handle *fr, const _TCHAR *path);

#ifdef __cplusplus
}
#endif

#endif
