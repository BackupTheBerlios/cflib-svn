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
 * @version   SVN: \$Id: cfdebug.c 128 2009-01-28 16:51:43Z stefan $
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
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
 *
 *******************************************************************************
 */

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
 * Debugging Function
 *
 * Dump CFLIB DB content to @c fout
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
	int verb;

	if(!_conf[0]) return CFE_NCA;

	/**
	 * Internal configuration option via config variable "CF_DUMPVERB"
	 */
	if( (verb = cfgetnum("CF_DUMPVERB")) == CFE_INF ) verb = CFD_DEFAULT;

	if( verb & CFD_LIBHEAD )
		fprintf( fout, "\nCFLIB Configuration Library PL %d - %s\n\n",
					   cfgetvers(), cfgetcpr() );

	if( verb & CFD_COLHEAD )
		fputs( "Source Flags    Option  Variable           \tContent\n", fout );

	for(i=0;i<MAXCONF;i++){

		fprintf( fout, "[%02lx]   [%08lx]  [%c]  %-20s\t%s\n",
						(_conf[i]->flag&CF_SET)>>8,
						_conf[i]->flag,
						_conf[i]->option,
						_conf[i]->name,
						_conf[i]->inhalt );

		if(_conf[i]->flag&CF_LAST) return (i+1);
	}

	if( verb & CFD_SRCFLAGS ){
		fputs( "Sources: 1=update function call, 2=commandline argument, ", fout);
		fputs( "4=environment variable,\n         8=private setfile, ", fout);
		fputs( "10=system setfile, 20=built-in default, ", fout);
		fputs( "40=interactive terminal input\n", fout);
	}

	return CFE_NLE;
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
