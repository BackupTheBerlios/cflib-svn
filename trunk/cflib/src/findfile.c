/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file findfile.c
 *
 * Search a file, given Search Path and (optional) Extensions
 *
 * The function FindFile() is a useful extension of CFLIB core features,
 * but the feature, usually triggered by @ref CF_FINDFILE in the configuration
 * flag of a config variable, is not needed for CFLIB core functionality.
 *
 * Define @b _CF_NOFINDFILE to compile a library version without support
 * for this function, if you want to reduce CFLIB code to a minimum
 * that is needed for configuration input and use in a program.
 *
 * The functions in this file were originally part of the Mintlibs (PL 44),
 * a Un*x style environment for the Atari ST developed by Eric R. Smith and
 * others. They are still part of the MiNTLib distribution as of PL 58
 * (29-Apr-2008):
 *
 * @see http://sparemint.org/sparemint/mint/mintlib/
 *
 * The original code is placed in the public domain and has been adapted to
 * serve as a general utility function within CFLIB.
 *
 * @version   SVN: \$Id$
 * @author    Eric R. Smith
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @package   CFLIB
 * @subpackage Library_Extension
 *
 *//****************************************************************************
 *
 * This file is part of the CFLIB library.
 * CFLIB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CFLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CFLIB (see LICENSE.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 *//****************************************************************************
 *
 * Version history
 *
 * 1993-04-28 [uo] Rehacked by Uwe Ohse to support reentrant spawn/popen.
 * 1993-12-19 [as] Andreas Schwab, clean up namespace: findfile moved
 *                 to separate file, buffindfile renamed to _buffindfile
 * 1995-05-17 [sh] Support other systems than TOS/MiNT:
 *                 - reunited findfile and _buffindfile
 *                 - define unix or linux for these systems, nothing for others
 * 2008-08-03 [sh] header with meta info and license
 * 2008-08-04 [sh] constant default extensions array moved here from cfmain()
 * 2009-08-22 [sh] Documentation Update
 * 2009-01-23 [sh] Comparison with MiNTLib PL58, minor changes, docu
 * 2009-01-24 [sh] Renamed findfile to FindFile and move to public space cf.h
 *
 ******************************************************************************/

#ifndef _CF_NOFINDFILE

#ifdef _HAS_LIMITS
#include <limits.h>     /**< Needed for PATH_MAX */
#endif
#ifdef __MINT__
#include <compiler.h>
#include <support.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#ifndef linux
#include <io.h>
#endif
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX    200     /**< Path Array max size unless already defined */
#endif

#ifdef _PATHSEP_SEMICOLON
#define PATHSEP1    ';'
#else
#define PATHSEP1    ':'     /**< Path List Component Separator */
#endif
#ifdef _PATHSEP_COMMA
#define PATHSEP2    ','
#else
#define PATHSEP2    '\0'    /**< Alternative Path List Component Separator
                                 (optional) */
#endif

#ifdef _PREFER_BACKSLASH
#define DIRSEP1     '\\'
#define DIRSEP2     '/'
#else
#define DIRSEP1     '/'     /**< Native OS Directory Separator (Default) */
#define DIRSEP2     '\\'    /**< Directory Separator
                                 for emulating another OS (Default) */
#endif

/**
 * @private Empty Extensions Array
 */
static const char *const _cf_nullext[] = { NULL };

/***************************************************************************//**
 *
 * @private
 *
 * Check whether file exists
 *
 * @author Eric R. Smith
 * @license Public Domain
 *
 * @param name
 * @return @c 1 if file exists, else @c 0
 *
 ******************************************************************************/
static int EXISTS( const char *name )
{
	struct stat dummy;
	if( stat(name, &dummy) != 0 ) return 0;
	if( (dummy.st_mode & S_IFMT) != S_IFREG ) return 0;
	return 1;
}

/***************************************************************************//**
 *
 * @private
 *
 * Find a File
 *
 * Given a base filename, a list of directories, and a list
 * of possible extensions to the filename, attempts to find a file.
 * Useful for doing, e.g. spawnvp. Note that the current directory
 * is always searched first! If the filename already contains a
 * path specification (or extension) then the given path list
 * (or extension list) is ignored.
 *
 * @author Eric R. Smith
 * @license Public Domain
 *
 * @param fname File name
 * @param fpath Search Path: String of Directories
 *                           separated by @ref PATHSEP1 or @ref PATHSEP2
 * @param fext  Array of possible File Extensions
 * @param try   Current file path to check
 *
 * @return The name by which the file was found or @c NULL
 */
char *_buffindfile( const char *fname, const char *fpath,
                    const char *const *fext, char *try )
{
    char *s, *extplace, c;
    const char *const *nextext;
    const char *t;
    int  hasext = 0, haspath = 0;

	if (!fname || !*fname) return NULL;

	s = try;
	t = fname;

	/*
	 * copy the file in, checking to see if
	 * a path and/or extension are already given
	 */
	while( (c = *t++) != 0 ){

		if( c == DIRSEP1 || c == DIRSEP2 ){

			haspath = 1;
			hasext = 0;

		} else if (c == '.') hasext = 1;

		*s++ = c;
	}
	extplace = s;
	*s = 0;

	if( haspath || !fpath ) fpath = "";
	if( hasext || !fext ) fext = _cf_nullext;

	for( ;; ){ /* loop on path elements */

		nextext = fext;
		if (!hasext) {
			extplace[0] = 0;
			extplace[1] = 0;
		}

		if (EXISTS(try)) return try;

		extplace[0] = '.';

		while( *nextext ){ /* loop on extensions */

			(void)strcpy(&extplace[1], *nextext++);
			if( EXISTS(try) ) return try;
		}
		if( !*fpath ) break;  /* no more places to look */

		/* copy in next element of path list */
		s = try;

		/* an attempt to accomodate within reason TOS specs  -- mj */
		if( *(fpath + 1) == ':' ){

			if( ( *(fpath + 2) == DIRSEP1 || *(fpath + 2) == DIRSEP2)
				&& ( *fpath != '.' && *fpath != ':' )
			){
				*s++ = *fpath++;
				*s++ = *fpath++;
				*s++ = *fpath++;
			}
		}
		while( (c = *fpath) != 0 && c != PATHSEP1 && c != PATHSEP2 ){

			*s++ = c;
			fpath++;
		}
		if( c ) fpath++;
		if( s == try || (s[-1] != DIRSEP1 && s[-1] != DIRSEP2 ) )
			*s++ = DIRSEP1;     /* preferred separator */
		t = fname;
		while( (*s++ = *t++) != 0 );
		extplace = --s ;        /* where the extension gets written */
	}
	return NULL;
}

/***************************************************************************//**
 *
 * @ingroup utilities
 *
 * Find a File in Path trying Extensions
 *
 * @author Eric R. Smith
 * @license Public Domain
 *
 * @param fname File Name
 * @param fpath Search Path: String of Directories
 *                           separated by @b PATHSEP1 (':' or ';')
 *                           or @b PATHSEP2 (nothing or ',')
 * @param fext  Array of possible Extensions (optional, default is OS dependent)
 *
 * @return The name by which the file was found or @c NULL
 *
 ******************************************************************************/

char *FindFile( const char *fname, const char *fpath, const char *const *fext )
{
	/**
	 * Prepare (default) arguments and call _buffindfile
	 */
	static char try[PATH_MAX];
	/**
	 * Array of executable extensions to be used in function FindFile()
	 * when searching executables along a path list
	 */
	static const char *const _cf_exec_ext[] =
#ifdef atarist
		{ "ttp", "tos", "prg", "app", "gtp", NULL, };
#else
#ifdef unx
		{ "sh", "pl", NULL, };
#else
		{ "exe", "com", "bat", "cmd", NULL, };
#endif
#endif

	if( !fext ) fext = _cf_exec_ext;

	return ( _buffindfile( fname, fpath, fext, try) );
}

#endif

/******************************************************************************/
