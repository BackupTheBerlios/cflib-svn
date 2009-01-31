/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file cfio.c
 *
 * Functions for Input and Output of Configuration items
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @package   CFLIB
 * @subpackage Library_Core
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
 * 2009-01-22 [sh] adaption to doxygen docu tool
 * 2009-01-23 [sh] some small improvements
 * 2009-01-30 [sh] switched time format to "%d.%m.%Y"
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "cflib.h"

/***************************************************************************//**
 *
 * @ingroup retrieval
 *
 * Library internal function, use appropriate Macro functions!
 *
 * Inquire configuration database for content of entry name
 *
 * @param name      Entry's name
 * @param typ       Expected/required type of content:
 *                  @arg @c 0 : Get type from entry's flag
 *                  @arg @ref CF_INT    : Integer
 *                  @arg @ref CF_REAL   : Real
 *                  @arg @ref CF_FLAG   : Flag
 *                  @arg @ref CF_STR    : String
 *                  @arg @ref CF_SRC    : Source
 *                  @arg @ref CF_FLGINQ : Bit in entry flag
 *                                        (bit no. in lowest bytes)
 *                  @arg @ref CF_RESID  : Residual argument
 *                  @arg Anything else  : String
 *
 * @return
 *          @li for @ref CF_STR, @ref CF_RESID or default:
 *
 *            @arg @c NULL : An error occurred or nothing available:
 *                           - No configuration database available
 *                           - No entry of this name available
 *                           - The entry's content is really @c NULL,
 *                             Check that with cfgetflag() !!
 *                           - No more residual argument (for @ref CF_RESID)
 *
 *            @arg any other   pointer to string in content of entry name
 *
 *          @li for others:
 *            Pointer to return values of the corresponding macro function
 *
 ******************************************************************************/

void *cfgetent( char *name, CFFLAGTYP typ )
{
	register int i, j;
	static int ret = 0;
	static float retf = 0.0;

	/* ready to work? */

	if( _conf[0] == NULL ){

		switch( typ & CF_ALLGET ){

			case CF_INT:
				ret=CFE_INF;
				break;
			case CF_REAL:
				retf=CFE_RNF;
				break;
			case CF_FLAG:
			case CF_FLGINQ:
			case CF_SRC:
				ret=CFE_NCA;
				break;
			case CF_STR:
			case CF_RESID:
			default:
				return NULL;
				break;
		}

		if( typ & CF_REAL ) return &retf;
		else return &ret;
	}

	/* get and interprete entry */

	for(i=0;i<MAXCONF;i++){

		if( (typ & CF_RESID & _conf[i]->flag) && _conf[i]->option == ' ' ){

			_conf[i]->option = '-';
			return _conf[i]->inhalt;

		} else if( !strcmp(_conf[i]->name, name) ){

			if( !typ ) typ = _conf[i]->flag & CF_TYPE;
			if( !typ ) typ = CF_STR;

			switch( typ & CF_ALLGET ){

				case CF_INT:
					if( _conf[i]->inhalt == NULL )
						ret = CFE_INF;
					else if( isdigit( *_conf[i]->inhalt )
							 || *_conf[i]->inhalt == '+'
							 ||	*_conf[i]->inhalt == '-' )
						ret = atoi( _conf[i]->inhalt );
					else
						ret=CFE_INF;
					break;

				case CF_REAL:
					if( _conf[i]->inhalt == NULL )
						retf = CFE_RNF;
					else if( isdigit( *_conf[i]->inhalt )
							 || *_conf[i]->inhalt == '+'
							 || *_conf[i]->inhalt == '-' )
						retf = atof( _conf[i]->inhalt );
					else
						retf = CFE_RNF;
						break;

				case CF_FLAG:
					if( _conf[i]->inhalt == NULL )
						ret = CFE_ECP;
					else
						ret = ( _conf[i]->inhalt[0] ? TRUE : FALSE );
					break;

				case CF_FLGINQ:
					ret = ( ( (_conf[i]->flag >> (31&typ) ) & 1 ) ? TRUE : FALSE );
					break;

				case CF_SRC:
					/* Attention!: borders are dependent on CF_SET! */
					for( j=CF_SET_1STBIT; j <= CF_SET_LASTBIT; j++ ){
						if( ((_conf[i]->flag&CF_SET)>>j)&1)
							ret = ( j - CF_SET_1STBIT );
						else continue;
						break;
					}
					if( j > CF_SET_LASTBIT )
						ret = CFE_NSS;
					break;

				case CF_STR:
				default:
					return (_conf[i]->inhalt);
					break;
			}

		} else if( _conf[i]->flag & CF_LAST ){ /* last entry? */

			switch( typ & CF_ALLGET ){

				case CF_INT:
					ret = CFE_INF;
					break;
				case CF_REAL:
					retf = CFE_RNF;
					break;
				case CF_FLAG:
				case CF_FLGINQ:
				case CF_SRC:
					ret = CFE_NEA;
					break;
#ifdef _CF_RESID_FREE
				case CF_RESID:
					for( j=i; _conf[j]->flag&CF_RESID; j-- ){
						_conf[j-1]->flag |= CF_LAST;
						free( _conf[j] );
					}
#endif
				case CF_STR:
				default:
					return NULL;
					break;
			}

		} else continue;

		if( typ == CF_REAL ) return &retf;
		return &ret;
	}

	return NULL;
}


/***************************************************************************//**
 *
 * @ingroup retrieval
 *
 * Get Library Version/Patchlevel
 *
 * @return
 *          @arg @c >0  : Libary Patchlevel
 *          @arg @c <=0 : Error
 *
 ******************************************************************************/

int cfgetvers(void){

	int i;

	if( (i=atoi(Patchlevel)) <= 0 ) return -1;
	return i;
}

/***************************************************************************//**
 *
 * @ingroup retrieval
 *
 * Get Library Subversion Details
 *
 * The Patchlevel returned by this function should match @ref Patchlevel
 * in the public include file cf.h
 *
 * @return   Libary Patchlevel and Subversion (Source Revision marked by
 *           library internal header file @e cflib.h.
 *
 * The return value is "burned" into the library executable and looks like:
 * @verbatim CFLIB PL 20 $LastChangedRevision$ @endverbatim
 *
 ******************************************************************************/

char *cfgetsubvers(void){

	return Subversion;
}

/***************************************************************************//**
 *
 * @ingroup setting_saving
 *
 * Update or Add Parameter @e name with string @e content
 *
 * @param name      @ref parameter_name
 * @param content   New (String) Content
 *
 * @return
 *          @arg @ref CFE_NEP : @copybrief CFE_NEP
 *          @arg @c 0  : Entry updated successfully
 *          @arg @c <0 : An error occurred:
 *                       - @ref CFE_NCA : @copybrief CFE_NCA
 *                       - @ref CFE_MCF : @copybrief CFE_MCF
 *
 ******************************************************************************/

int cfputstr( char *name, char *content ){

    register int i;

    /* Ready to work? */

    if(!_conf[0]) return CFE_NCA;

    /* Put entry */

    for(i=0;i<MAXCONF;i++){

        if( !strcmp(_conf[i]->name,name) ){

            if( _conf[i]->flag&CF_MALLOC ) free(_conf[i]->inhalt);

            if(content){
                if( (_conf[i]->inhalt = malloc(strlen(content)+1)) == NULL )
                	return CFE_MCF;
                strcpy(_conf[i]->inhalt, content);
                SetFlag(_conf[i]->flag, CF_MALLOC);
            }
            else _conf[i]->inhalt = NULL;

            SetFlag(_conf[i]->flag, CF_SET_PUT);
            return CFE_OK;
        }

        if(_conf[i]->flag&CF_LAST) break;
    }

    /* New entry */

    if( (_conf[++i] = malloc(sizeof(struct _cf))) == NULL ) return CFE_MCF;
    if( (_conf[i]->name = malloc(strlen(name)+1)) == NULL ) return CFE_MCF;
    strcpy(_conf[i]->name, name);
    if( (_conf[i]->inhalt = malloc(strlen(content)+1)) == NULL) return CFE_MCF;
    strcpy(_conf[i]->inhalt, content);

    _conf[i]->option = 'x';
    _conf[i]->flag = CF_SET_PUT | CF_RESID | CF_MALLOC | CF_LAST;
    DelFlag(_conf[i-1]->flag, CF_LAST);

    return CFE_NEP;
}

/***************************************************************************//**
 *
 * @ingroup setting_saving
 *
 * Set all Time and/or Date entries in CFLIB DB to @e now or @e today
 *
 * @param td    Target Selection Mask:
 *              @arg @ref CF_TIME : Set Time
 *              @arg @ref CF_DATE : Set Date
 *              @arg @ref CF_TD : Set Time and Date
 *
 * @return
 *          @arg @c >0  : Number of entries updated successfully
 *          @arg @c <=0 : An error occurred:
 *                        - @ref CFE_NCA : @copybrief CFE_NCA
 *                        - @ref CFE_MCF : @copybrief CFE_MCF
 *                        - @ref CFE_NEA : @copybrief CFE_NEA
 *
 ******************************************************************************/

int cfputtime( CFFLAGTYP td ){

    int i, ret, count=0;
    time_t stime1;
    struct tm *stime2;
    char *buffer;

    /* ready to work? */

    if(_conf[0]==NULL) return CFE_NCA;

    /* get time */

    stime1 = time(NULL);
    stime2 = localtime(&stime1);

    for(i=0;i<MAXCONF;i++){

        if(_conf[i]->flag & CF_TD & td){

            if( (buffer=malloc(CF_MAXTIMEBUF)) == NULL) return CFE_MCF;
#ifdef DEBUG_TIME
            fprintf("cfputtime: current time is %s\n",asctime(stime2));
#endif
            if(_conf[i]->flag&CF_TIME)
				strftime(buffer,CF_MAXTIMEBUF,"%H:%M:%S",stime2);
            else if(_conf[i]->flag&CF_DATE)
            	strftime(buffer,CF_MAXTIMEBUF,"%d.%m.%Y",stime2);
            if((ret=cfputstr(_conf[i]->name,buffer))!=0) return ret;
            count++;
            free(buffer);
        }
        if(_conf[i]->flag&CF_LAST) break;
    }

    return count;
}


/***************************************************************************//**
 *
 * @ingroup setting_saving
 *
 * Alter or query the @ref CF_NOSAVE Flag of Parameter @e name
 *
 * When the configuration database is saved to a configuration file, the
 * function cfsave() will exclude all items with the @ref CF_NOSAVE flag set
 * from the output.
 *
 * The @ref CF_NOSAVE flag can be set in the Special Option Mask CONFIG::flag
 * for every entry in the @ref config_initializer given to cfinit() or cfstart()
 * or later be set with this function for parameters in the current
 * configuration database @ref _conf
 *
 * @ref residuals will have the @ref CF_NOSAVE flag set by default.
 *
 * @param name  Entry's name
 *              - @c "" : All entries
 *              - @c NULL : All hardcoded entries
 *
 * @param onoff
 *              @arg @ref CF_FLAG_ON  : Set Flag
 *              @arg @ref CF_FLAG_OFF : Delete Flag
 *              @arg @c   "i"         : Inquire Flag
 *
 * @return
 *          @arg @c 0 : Entry updated successfully / Flag is OFF (for @c "i")
 *          @arg @c 1 : Flag is ON (for @c "i")
 *          @arg @c !=0 or @c 1 : An Error occurred:
 *                      - @ref CFE_NCA : @copybrief CFE_NCA
 *                      - @ref CFE_NEA : @copybrief CFE_NEA
 *
 ******************************************************************************/

int cfnosave( char *name, const char *onoff ){

    register int i;
    char *alter, *leave=CF_FLAG_OFF;

#ifdef DEBUG_NOSAVE
	fprintf(stderr,"cfnosave: name=%s, flag=%s\n",name, (*onoff)?"ON":"OFF");
#endif

	/* ready to work? */

    if(!_conf[0]) return CFE_NCA;

    /* alter entry flag */

    for(i=0;i<MAXCONF;i++){

        if((name == NULL) && (_conf[i]->flag&CF_RESID)) alter = CF_FLAG_OFF;
        else if (name == NULL) alter = CF_FLAG_ON;
        else if(*name == '\0') alter = CF_FLAG_ON;
        else if( !strcmp(_conf[i]->name,name) ) leave = alter = CF_FLAG_ON;
        else alter = CF_FLAG_OFF;

        if( *alter ){
            switch(*onoff){
              case 'i':
                if(_conf[i]->flag&CF_NOSAVE) return(1);
                else return(0);
               break;
              case '\0':
                DelFlag(_conf[i]->flag,CF_NOSAVE);
               break;
              default:
                SetFlag(_conf[i]->flag,CF_NOSAVE);
               break;
            }
            if(*leave) return CFE_OK;
        }

        if(_conf[i]->flag&CF_LAST) break;
    }

    return CFE_NEA;
}


/***************************************************************************//**
 *
 * @ingroup retrieval
 *
 * Get Usage Message for (Terminal) Output
 *
 * There are two flavours:
 *
 * 1. Let CFLIB do the job: Usage message is generated based on settings for
 *    commandline parsing and @ref special_options_mask found in the database
 *
 * 2. Deliver your own Usage Message: Just set the @ref CF_USAGE Flag in the
 *    @ref special_options_mask of one parameter in the database to get a custom
 *    usage message (from hardcoded default, configuration file or environment).
 *    The delivered custom message string is taken as a format string for the
 *    printf() function: Use "%s" in the message string to have the program name
 *    inserted that CFLIB got from the default parameter @b "CF_PRGNAME" which
 *    by default is set to the name of the running program from the commandline
 *    at startup.
 *
 * @return
 *          - Pointer to usage string
 *          - @c NULL : An error occurred:
 *                      @li @copybrief CFE_NCA
 *                      @li malloc() for usage string failed
 *
 * @see @ref CF_MAXUSAGE : @copybrief CF_MAXUSAGE
 * @see @ref CF_USG_DEFCOLS : @copybrief CF_USG_DEFCOLS
 *
 ******************************************************************************/

char *cfgetusg(void){

    register int i;
    int cols;
    char *buffer, *tmppoi=NULL, *begin, *startline;

    /* ready to work? */

    if(!_conf[0]) return NULL;
    if( (begin=startline=buffer=malloc(CF_MAXUSAGE)) == NULL) return NULL;


    /* search for user defined usage string */
    for(i=0;i<MAXCONF;i++){
        if(_conf[i]->flag&CF_USAGE){
            sprintf(buffer,_conf[i]->inhalt,_conf[0]->inhalt);
            tmppoi=buffer;
        }
        if(_conf[i]->flag&CF_LAST) break;
    }

    if( tmppoi == NULL ){

    	/* build usage string in buffer */

    	if( (tmppoi=getenv("COLS")) == NULL || (cols=atoi(tmppoi)) <= 0 ){
    		if( (tmppoi=getenv("COLUMNS")) == NULL || (cols=atoi(tmppoi)) <= 0 )
    			cols = CF_USG_DEFCOLS;
        }

        tmppoi = buffer + sprintf(buffer,"Usage: %s",_conf[0]->inhalt);

        for(i=1;i<MAXCONF;i++){

            /* entry takes argument from commandline? / omit residual items */

            if(_conf[i]->option!=' ' && !(_conf[i]->flag&CF_RESID)){

                begin=tmppoi;
                tmppoi+=sprintf(tmppoi," "); /* removed: \b */
                if(!(_conf[i]->flag&CF_SET_ARG && _conf[i]->flag&CF_FORCED))
                    *tmppoi++='[';

                if(_conf[i]->flag&CF_NO_OPT_ARG)
                    tmppoi+=sprintf(tmppoi,"<%s>", _conf[i]->name);
                else{
                    tmppoi+=sprintf(tmppoi,"-%c%s%s%s%s",
                    _conf[i]->option,
                    (_conf[i]->flag&(CF_CONCAT|CF_FLAG))?"":" ",
                    (_conf[i]->flag&CF_FLAG)?"[-|ON|OFF]":"<",
                    (_conf[i]->flag&CF_FLAG)?"":_conf[i]->name,
                    (_conf[i]->flag&CF_FLAG)?"":">");
                }

                if(!(_conf[i]->flag&CF_SET_ARG && _conf[i]->flag&CF_FORCED))
                    tmppoi+=sprintf(tmppoi,"]");

                if( (tmppoi-startline)>=cols && begin!=buffer){
                    startline=begin;
                    *begin++='\n';
                    *begin='\t';
                }

            }
            if(_conf[i]->flag&CF_LAST) break;
        }

        sprintf(tmppoi,"\n");

    }

    buffer = realloc(buffer, strlen(buffer)+1);
    return buffer;
}

/******************************************************************************/
/**
 * @def cfget(a)
 *
 * Get value (content) of named entry
 *
 * @param a Entry's name
 *
 * @return	Depending on configured type, see cfgetent() and Macro Definitions!
 *
 ******************************************************************************/
/**
 * @def cfgetstr(a)
 *
 * Inquire CFLIB DB for String in content of named entry
 *
 * @param a Entry's name
 *
 * @return	@li @c NULL : An error occurred:
 *                        - No configuration database available
 *                        - No entry of this name available
 *                        - Content is really @c NULL,
 *                          Check that with cfgetflag() !!
 *
 *          @li Any other : Pointer to string in content of entry name
 *
 ******************************************************************************/
/**
 * @def cfgetsrc(a)
 *
 * Inquire CFLIB DB for Source of named entry's content
 *
 * @param a Entry's name
 *
 * @return
 *          - @c 0-6 : Source of entry's content:
 *                      @arg @c 0 : cfputstr() call
 *                      @arg @c 1 : Command line / Arguments
 *                      @arg @c 2 : Environment
 *                      @arg @c 3 : Private Configuration File
 *                      @arg @c 4 : System Configuration File
 *                      @arg @c 5 : Default setting
 *                      @arg @c 6 : Interactive input (query)
 *
 *          - @c <0 : An error occurred:
 *                      @arg @ref CFE_NCA : No configuration database available
 *                      @arg @ref CFE_NEA : No entry of this name available
 *                      @arg @ref CFE_NSS : No source set (should not happen!)
 *
 ******************************************************************************/
/**
 * @def cfgetnum(a)
 *
 * Inquire CFLIB DB for Integer value in content of named entry
 *
 * @param a Entry's name
 *
 * @return	@li @ref CFE_INF : Integer inquiry failed because of
 *                        - No configuration database available
 *                        - No entry of this name available
 *                        - Content doesn't begin with digit
 *                        - Content is a @c NULL pointer
 *                        - Content is really @ref CFE_INF,
 *                          Check that with cfgetstr() !!
 *
 *          @li Any other : Integer value for named entry
 *
 ******************************************************************************/
/**
 * @def cfgetreal(a)
 *
 * Inquire CFLIB DB for Float (Real) value in content of named entry
 *
 * @param a Entry's name
 *
 * @return	@li @ref CFE_RNF : Real/float value inquiry failed because of
 *                        - No configuration database available
 *                        - No entry of this name available
 *                        - Content doesn't begin with digit or signum (+/-)
 *                        - Content is a @c NULL pointer
 *                        - Content is really @ref CFE_RNF,
 *                          Check that with cfgetstr() !!
 *
 *          @li Any other : Float value for named entry
 *
 ******************************************************************************/
/**
 * @def cfgetflag(a)
 *
 * Inquire CFLIB DB for Flag value in content of named entry
 *
 * @param a Entry's name
 *
 * @return
 *          - @c TRUE (1)  : Flag is set
 *          - @c FALSE (0) : Flag is not set
 *          - @c <0 : An error occurred:
 *                    @arg @ref CFE_NCA : No configuration database available
 *                    @arg @ref CFE_NEA : No entry of this name available
 *                    @arg @ref CFE_ECP : Entry's content is a @c NULL pointer
 *
 ******************************************************************************/
/**
 * @def cfgetres()
 *
 * Get next Residual Command Line Argument from CFLIB DB
 *
 * @return
 *          @li @c NULL : An error occurred:
 *                        - @copybrief CFE_NCA
 *                        - No more residual arguments available
 *
 *          @li Any other : Pointer to string content
 *
 * @see @ref residuals
 *
 ******************************************************************************/
/**
 * @def cfgetcpr()
 *
 * Get Copyright Notice
 *
 * @return  Pointer to Copyright Message String
 *
 ******************************************************************************/
/**
 * @def cfflaginq(a,b)
 *
 * Inquire CFLIB DB for Bit set in entry's Special Options Fag CONFIG::flag
 *
 * Debugging Function
 *
 * @param a Entry's name
 * @param b Bit Offset, @c 0<=b<=31
 *
 * @return
 *          - @c TRUE (@c 1)  : Flag is set
 *          - @c FALSE (@c 0) : Flag is not set
 *          - @c <0 : An error occurred:
 *                    @arg @ref CFE_NCA : @copybrief CFE_NCA
 *                    @arg @ref CFE_NEA : @copybrief CFE_NEA
 *
 ******************************************************************************/
/**
 * @def cfput(a,b)
 *
 * Update or Add Parameter (Utility Function Macro)
 *
 * @param a @ref parameter_name
 * @param b New Content (Type casted to expected Char Pointer)
 *
 * @return @b int cfputstr()
 *
 ******************************************************************************/
