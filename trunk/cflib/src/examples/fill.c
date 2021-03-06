/**
 * @file fill.c
 *
 * Multiple Interface Data Manager and Report Generator ;-)
 *
 * Usage example for CFLIB - Flexible Configuration Library
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1995-2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License
 * @package   CFLIB
 * @subpackage Examples
 */
/*
 *    Generate Report from Template and Configuration
 *
 *    Input:  - Commandline, Environment, Section in System Configuration File,
 *              Private Configuration File, Standard Input for Variables.
 *            - Text Template (Form).
 *
 *    Output: - Generated Report (Filled Form) to File or stdout,
 *            - Configuration to Private Configuration File, Logfile or
 *              Standard Output
 */

#include <stdio.h>

#include "../../include/cf.h"

#define DEF_SETFILE "~/fill.cnf"
#define DEF_SYS_SETFILE "/etc/cfcommon.cnf"

int main( int argc, char **argv )
{
	int ecode;
	char *save, *infile, *outfile, line[CF_MAXERRSTR+1];
	char *savemode = "w";
	FILE *error_log = stderr;

	/* Hardcoded Configuration Setup and Defaults */
	CONFIG setting[] =
	{	{ "FILL_INFILE",   NULL,             'i', 0x0,                        },
		{ "FILL_OUTFILE",  NULL,             'o', 0x0,                        },
		{ "FILL_VARDELIM", "$()",            'l', CF_CONCAT,                  },
		{ "verb",          CF_FLAG_OFF,      'v', CF_IGN_ENV|CF_FLAG,         },
		{ "help",          CF_FLAG_OFF,      'h', CF_IGN_ENV|CF_FLAG|CF_NOSAVE,},
		{ "query",         CF_FLAG_OFF,      'q', CF_IGN_ENV|CF_FLAG,         },
		{ "TIME",          NULL,             't', CF_TIME|CF_SET_PUT,         },
		{ "DATE",          NULL,             'd', CF_DATE|CF_SET_PUT,         },
		{ "FILL_CNF",      DEF_SETFILE,      'p', CF_SETFILE,                 },
		{ "FILL_SAVE",     NULL,             's', CF_IGN_ENV|CF_CONCAT,       },
		{ "SAVE_DATA",     CF_FLAG_OFF,      'x', CF_FLAG|CF_IGN_ENV,         },
		{ "SFILE",         DEF_SYS_SETFILE,  ' ', CF_SYS_SETFILE,             },
		{ "FILL_SECTION",  "fill_default",   'c', CF_SECTION|CF_LAST,         },
	};

	/* Initialize with Error Reporting */
	if( cfinit(setting,argc,argv) < 0 ){
		while( (ecode=cfgeterr(line,0)) == TRUE )
			fprintf( error_log, "error %d: %s", ecode, line );
		fflush(error_log);
		fputs( cfgetusg(), stderr );
		return (-1);
	}

	/* User Help and Configuration Overview */
	if( cfgetflag("help") ){
		fputs( cfgetusg(), stderr );
		if( cfgetflag("verbose") ){
			fprintf( stderr, "\nConfiguration Library PL %d - %s\n\nCurrent Configuration:\n\n",
							 cfgetvers(), cfgetcpr() );
			cfdump(stderr);
			fputs( "\nSources: 1=update function call, 2=commandline argument, 4=environment variable,\n", stderr );
			fputs(   "         8=private setfile, 10=system setfile, 20=built-in default, 40=interactive terminal input\n", stderr );
		}
		return (1);
	}

	/**
	 * The "main application"
	 *
	 * Filter a template stream from file or @c stdin (default)
	 * to file or @c stdout (default)
	 * - resolving variables with values from configuration
	 * - optionally propmt user for values to resolve
	 */
	switch(
	    cfform( infile = cfgetstr("FILL_INFILE"),
				outfile = cfgetstr("FILL_OUTFILE"),
				cfgetstr("FILL_VARDELIM"),
				cfgetflag("query") )
	){
		case CFE_FNF:
			fprintf(stderr,"read access error: %s\n",infile); break;
		case CFE_WAE:
			fprintf(stderr,"write access error: %s\n",outfile); break;
	}

	if( cfgetflag("verb") ){
		if( outfile == NULL ) outfile = "<stdout>";
		fprintf( stderr, "blank form: %s, filled form: %s\n", infile, outfile );
	}

	/**
	 * Dump current configuration (or parts of it):
	 *
	 * - to stdout
	 * - to original configuration file
	 * - to new file
	 * - append to existing file
	 */
	if( cfgetflag("SAVE_DATA") ){
		cfnosave(NULL, CF_FLAG_ON);
		cfnosave("TIME", CF_FLAG_OFF);
		cfnosave("DATE", CF_FLAG_OFF);
	}
	if( (save = cfgetstr("FILL_SAVE")) != NULL ){
		if( cfgetflag("SAVE_DATA") ) savemode = "a";
		if( *save == '\0' )
			cfsave( NULL, savemode );
		else if( *save == '+' )
			cfsave( "", savemode );
		else
			cfsave( save, savemode );
	}

	return (0);
}
