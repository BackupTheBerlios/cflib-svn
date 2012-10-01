/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file cfdebug.c
 *
 * Dump and Debugging Functions
 *
 * The functions cfdump() and cfdinichk() in this file are useful,
 * but not necessary for regular CFLIB functionality.
 * Define @b _CF_NODEBUGGING to compile a library version without support
 * for these functions.
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009, 2012 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @package   CFLIB
 * @subpackage Library_Debugging
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
 *******************************************************************************
 *
 * Version history
 *
 * 2008-08-03 [sh] header with meta info and license
 * 2009-08-22 [sh] Documentation Update
 * 2012-09-21 [sh] Fixed source flags description not accessible
 * 2012-09-21 [sh] Added CFD_FLAGS dump option and some fix in cfdump()
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cflib.h"

#ifndef _CF_NODEBUGGING

/***************************************************************************//**
 *
 * @ingroup advanced_features
 *
 * Dump CFLIB DB content to @c fout
 *
 * Debugging Function
 *
 * Configuration options:
 *
 * Set integer bitmask CFLIB variable @b "CF_DUMPVERB" to
 *
 *              @arg = @ref CFD_CFDUMP  : Minimal
 *              @arg & @ref CFD_LIBHEAD : CFLIB header
 *              @arg & @ref CFD_COLHEAD : Column headers
 *              @arg & @ref CFD_SRCFLAGS : Source flag description
 *              @arg = @ref CFD_DEFAULT : Default dump verbosity
 *
 * @param fout	Pointer to File opened for writing or @c stdout/@c stderr/...
 *
 * @return
 *          @arg @c >0 : Number of entries in CFLIB DB
 *          @arg @c <0 : An error occurred:
 *                       - @ref CFE_NCA : No configuration database available
 *
 ******************************************************************************/

int cfdump( FILE *fout )
{
	register int i;
	int verb, j;
	char buffer[9];

	if(!_conf[0]) return CFE_NCA;

	/**
	 * Internal configuration option via config variable "CF_DUMPVERB"
	 */
	if( (verb = cfgetnum("CF_DUMPVERB")) == CFE_INF ) verb = CFD_DEFAULT;

	if( verb & CFD_LIBHEAD )
		fprintf( fout, "\nCFLIB Configuration Library PL %d - %s\n\n",
					   cfgetvers(), cfgetcpr() );

	if( verb & CFD_COLHEAD )
		fputs( "Flags     Source Option Variable            \tContent\n", fout );

	for(i=0;i<MAXCONF;i++){

		sprintf(buffer,"%08lx",_conf[i]->flag);
		for(j=0;j<8;j++) if( buffer[j]=='0' ) buffer[j]='.';
		fprintf( fout, "[%s] [%02lx]  [%c]  %-20s\t%s\n",
						buffer,
						(_conf[i]->flag&CF_SET)>>8,
						_conf[i]->option,
						_conf[i]->name,
						(_conf[i]->flag&CF_FLAG) ?
							((_conf[i]->inhalt[0])?"ON":"OFF") : _conf[i]->inhalt );

		if(_conf[i]->flag&CF_LAST) break;
	}

	if( verb & CFD_SRCFLAGS ){
		fputs( "Sources: 1=update function call, 2=commandline argument, ", fout);
		fputs( "4=environment variable,\n         8=private setfile, ", fout);
		fputs( "10=system setfile, 20=built-in default, ", fout);
		fputs( "40=interactive terminal input\n", fout);
	}

	if( verb & CFD_FLAGS ){
		fputs( "Flags:\n", fout);
		/* CFLIB Function flags */
		fputs( "[.......1] CF_LAST        Last entry in settings array\n", fout);
		fputs( "[.......2] CF_PATH        Search Path (for FindFile feature)\n", fout);
		fputs( "[.......4] CF_SETFILE     Entry is Private Configuration File\n", fout);
		fputs( "[.......8] CF_SYS_SETFILE Entry is System Configuration File\n", fout);
		fputs( "[......1.] CF_SECTION     Section in Configuration File\n", fout);
		fputs( "[......2.] CF_PRGNAME     Running Program's Name from commandline\n", fout);
		fputs( "[......4.] CF_TIME        Time string\n", fout);
		fputs( "[......8.] CF_DATE        Date string\n", fout);
		/* Source flags - 'Origin Window' */
		fputs( "[.....1..] CF_SET_PUT     Source: Function Call\n", fout);
		fputs( "[.....2..] CF_SET_ARG     Source: Commandline Argument\n", fout);
		fputs( "[.....4..] CF_SET_ENV     Source: Environment Variable\n", fout);
		fputs( "[.....8..] CF_SET_PRIV    Source: Private Configuration File\n", fout);
		fputs( "[....1...] CF_SET_SYS     Source: System Configuration File\n", fout);
		fputs( "[....2...] CF_SET_DEF     Source: Built-in Default\n", fout);
		fputs( "[....4...] CF_SET_QRY     Source: Interactive Terminal Input\n", fout);
		/* Parsing Options */
		fputs( "[...1....] CF_NO_OPT_ARG  Commandline argument not following an option\n", fout);
		fputs( "[...2....] CF_CONCAT      Argument is concatenated to option\n", fout);
		fputs( "[...4....] CF_IGN_ENV     Do not check environment for variable\n", fout);
		fputs( "[...8....] CF_QUERY       Ask the user for unresolved item after configuration parsing\n", fout);
		/* Type/Interpretation Flags */
		fputs( "[..1.....] CF_STR         Entry is String\n", fout);
		fputs( "[..2.....] CF_INT         Entry is Integer\n", fout);
		fputs( "[..4.....] CF_FLAG        Entry is Flag\n", fout);
		fputs( "[..8.....] CF_REAL        Entry is Float\n", fout);
		/* Special Processing Options */
		fputs( "[.1......] CF_FINDFILE    Entry is filename to be searched for in the path\n", fout);
		fputs( "[.2......] CF_MUST        Entry may not be empty (NULL or \"\")\n", fout);
		fputs( "[.4......] CF_RESID       Residual/additional entry from commandline/setfile/cfput\n", fout);
		fputs( "[.8......] CF_USAGE       Usage Message format string\n", fout);
		fputs( "[1.......] CF_EXPHOME     Expand Home Directory\n", fout);
		/* Information/Status Flags */
		fputs( "[2.......] CF_MALLOC      Space for entry's content was mallocated\n", fout);
		fputs( "[4.......] CF_FORCED      Setting has been forced (already)\n", fout);
		fputs( "[8.......] CF_NOSAVE      Don't include in savefile / mark entry\n", fout);
	}

	if( i < MAXCONF ) return (i+1); /* number of entries */
	return CFE_NLE; /* no last entry flag */
}

/***************************************************************************//**
 *
 * @ingroup advanced_features
 *
 * Debugging Function (@b experimental)
 *
 * Initialize CFLIB DB using the default given by @c set, checking validity
 *                     and plausibility of entries
 *
 * @param set   Pointer to initializing CONFIG-Array
 *
 * @return
 *          @arg @c 0 : No error occurred
 *          @arg @c !=0 : An error occurred
 *
 * @todo Make cfdinichk() work as reliable, complete tool
 *       with much more testing and @ref special_options_mask validation!
 *
 ******************************************************************************/

int cfdinichk( CONFIG *set )
{
	register int i, j;
	int count = 0;

	CFFLAGTYP ivp[][2] = {
			{ CF_FLAG, CF_NO_OPT_ARG, },
			{ CF_CONCAT, CF_NO_OPT_ARG, },
			{ CF_SYS_SETFILE, CF_SETFILE, },
			{ CF_FLAG, CF_SYS_SETFILE, },
			{ CF_FLAG, CF_SETFILE, },
			{ CF_FLAG, CF_SECTION, },
			{ 0, 0, },
	};

	if( cfputerr( CFE_INIT, "Configuration check initialized" )
		== CFE_MEF ) return -CFE_MEF;

	for(i=0;i<MAXCONF;i++){

		if( !(_conf[i]=malloc(sizeof(struct _cf))) ) return -CFE_MCF;

		/* check entry's name */

		for( ; ; set++ ){
			if( !(set->name && *set->name) ){
				if( cfputerr( CFE_EWN, "Ignoring entry without name, content was: %s\n", set->inhalt )
					== CFE_MEF ) return -CFE_MEF;
			}
			else break;
		}
		_conf[i]->name = set->name;


		/* check entry's content */

		if( !(_conf[i]->inhalt=set->inhalt) && !(_conf[i]->flag&CF_QUERY) ){

			if( cfputerr( CFE_EWC, "Entry without content: %s\n", set->name )
				== CFE_MEF ) return -CFE_MEF;
		}

		/* check entry's option */

		_conf[i]->option = set->option;

		if( !isalpha(_conf[i]->option) || iscntrl(_conf[i]->option) ){

			if( cfputerr( CFE_UOS, "Unlikely option specifier in entry %s: '\\%o'=%c\n",
									_conf[i]->name, (int)_conf[i]->option, _conf[i]->option )
				== CFE_MEF ) return -CFE_MEF;
		}

#ifdef DEBUG_DINICHK
		printf("\tNr. %d, Name: %s, Inhalt: %s, Option: %c\n",i,_conf[i]->name,_conf[i]->inhalt,_conf[i]->option);
#endif

		/* check entry's flags */

		if( (_conf[i]->flag=set->flag) == TRUE ){

			for( j=0; (ivp[j][0]) ; j++ ){
				if( (_conf[i]->flag & ivp[j][0]) && (_conf[i]->flag & ivp[j][1]) ){
#ifdef DEBUG_DINICHK
					printf("_conf[i]->flag=%x,ivp[%d][0]=%x,ivp[%d][1]=%x\n",_conf[i]->flag,j,ivp[j][0],j,ivp[j][1]);
#endif
					if( cfputerr( CFE_IFC, "Invalid combination of flags in %s: 0x%x|0x%x, assuming first one is valid\n",
											_conf[i]->name, ivp[j][0], ivp[j][1] )
						== CFE_MEF ) return -CFE_MEF;
					_conf[i]->flag = _conf[i]->flag & (~ivp[j][1]);
				}
#ifdef DEBUG_DINICHK
				printf("j=%d, ivp[j][0]=%x, ivp[j][1]=%x\n",j,ivp[j][0],ivp[j][1]);
#endif
			}

			/* Attention!: borders are dependent on CF_SET! */
			for( j=CF_SET_1STBIT; j <= CF_SET_LASTBIT; j++ ){

				if( ( (_conf[i]->flag & CF_SET) >> j ) & 0x01 ) count++;
			}
			if( count > 1 ){

				if( cfputerr( CFE_IFC, "%d CF_SET_<spec> flags set in %s: Setflags = 0x%x\n",
										count, _conf[i]->name, _conf[i]->flag&CF_SET )
					== CFE_MEF ) return -CFE_MEF;
			}
		}

		_conf[i]->flag = _conf[i]->flag & (~CF_SET);
		_conf[i]->flag |= CF_SET_DEF;

#ifdef DEBUG_DINICHK
		printf("\tFlag: %lx\n",_conf[i]->flag);
#endif

		if( _conf[i]->flag & CF_LAST ) break;
		set++;
	}

	/* no last entry flag encountered ? */

	if( i == MAXCONF ){

		if( cfputerr( CFE_NLE, "No last entry flag found: Not present or entry number > MAXCONF\n" )
			== CFE_MEF ) return -CFE_MEF;
		_conf[i-1]->flag |= CF_LAST;
	}
	/* error exit/return */

	return cfputerr( CFE_EXIT, "Configuration check executed" );
}

#endif

/******************************************************************************/
