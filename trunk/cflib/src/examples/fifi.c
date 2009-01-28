/**
 * @file fifi.c
 *
 * Find a File in Path
 *
 * Usage example for CFLIB - Flexible Configuration Library
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1995-2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License
 * @package   CFLIB
 * @subpackage Examples
 *
 */
/*
 *    Find a File in Path
 *
 *    Input:  Commandline or Environment
 *
 *    Output: File path, Help message or Error message
 *
 *    Compile: gcc fifi.c -o fifi -lcf -L../../lib<platform>
 *    Run:     ./fifi -h
 *    Test:    ./fifi gcc
 */

#include <stdio.h>

#include "../../include/cf.h"

int main( int argc, char **argv )
{
	int ret; char *tmp;

	CONFIG setting[] =
	{
        /* Name, Default Content, Commandline Option, Special Option Flags */
		{ "PROGNAME", NULL,        '1', CF_FINDFILE|CF_SET_ARG|CF_NO_OPT_ARG, },
		{ "FF_PATH",  NULL,        ' ', CF_PATH,                              },
		{ "help",     CF_FLAG_OFF, 'h', CF_FLAG|CF_LAST,                      },
	};

	if ( !( ret = cfstart( setting, argc, argv, "help", CFS_USG ) ) ){

		if( ( tmp = cfgetstr("PROGNAME") ) != NULL )

			fprintf( stdout, "%s\n", tmp );
	}

	return ret;
}
