/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal
 *
 * @file cferr.c
 *
 * Functions for Error Handling
 *
 * The functions cfputerr(), cfgeterr(), cfclearerr() and cfreverr()
 * deliver core functionality of CFLIB, but they can be substituted by
 * dumb versions if you want to reduce CFLIB code to a minimum ;-(
 *
 * @version   SVN: \$Id: cferr.c 128 2009-01-28 16:51:43Z stefan $
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
 * 2009-08-22 [sh] Documentation Update
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define __CF_ERROR__
#include "cflib.h"
#undef __CF_ERROR__

/**
 * @addtogroup errors
 *
 * @bug There are still errors without entry in error stack
 *
 * - Library Internal Error Variables
 *
 * - Error Functions
 *
 * @{
 */

CONFERR *_cferr = NULL;     /**< Library Internal: Error List Pointer */
int     _errcnt = 0;        /**< Library Internal: Error List Counter */

/***************************************************************************//**
 *
 * Init, exit or append to Error List
 *
 * @param ecode		Error Code of the error that occurred
 *
 * @param string    Error message format string.
 *                  Giving it a @c NULL pointer results in an empty error
 *                  message string
 *
 * @param ...       Arguments list according to format string
 *
 * @return
 *        	@arg @c <0 : A fatal error occurred (malloc failed)
 *          @arg @c >0 : Number of errors in error list
 *
 ******************************************************************************/

int  cfputerr(int ecode, char *string,...){

    va_list vp;
    CONFERR *_cfenew;

    /* malloc space for entry */

    if( ( _cfenew=malloc(sizeof(struct _cfe)) ) == NULL) return CFE_MEF;
    _cfenew->next=_cferr;
    _cferr=_cfenew;
    _errcnt++;

    /* put error in list */

    _cferr->errcode=ecode;

    if(string!=NULL){
        va_start(vp,string);
        vsprintf(_cferr->errstr,string,vp);
        va_end(vp);
    }
    else _cferr->errstr[0]='\0';

#ifdef DEBUG_ERROR
        fprintf(stderr,"DEBUG_ERROR: cfputerr() - code: %d, estr: %s\n",_cferr->errcode,_cferr->errstr);
        fflush(stderr);
#endif

    return _errcnt;
}


/***************************************************************************//**
 *
 * Error Code and Message Inquiry Function
 *
 * @param string    Pointer to a string, to which the error message should be
 *                  copied. Giving it a @c NULL pointer will omit message return
 *
 * @param len       Size of string, if @c 0 @ref CF_MAXERRSTR will be used
 *
 * @return
 *          @arg @c 0   : if no error is available, everything is alright
 *          @arg @c !=0 : error code of the next error in list
 *
 ******************************************************************************/

int cfgeterr( char *string, size_t len ){

    int ecode = CFE_OK;
    CONFERR *_cfeold;

    if( _cferr == NULL ){

        if( string != NULL ) *string='\0';

    } else {

        ecode = _cferr->errcode;
        if( string != NULL )
			strncpy( string, _cferr->errstr, (len?len:CF_MAXERRSTR) );
        _cfeold = _cferr;
        _cferr = _cferr->next;
        free( _cfeold );
        _errcnt--;
    }

    return ecode;
}


/***************************************************************************//**
 *
 * Free all entries in error list
 *
 ******************************************************************************/

void cfclearerr( void ){

    CONFERR *_cfeold;

    while(_cferr!=NULL){
        _cfeold=_cferr;
        _cferr=_cferr->next;
        _errcnt--;
        free(_cfeold);
    }
}


/***************************************************************************//**
 *
 * Revert order of entries in error list from last->first to first->last
 *
 * @return
 *          @arg @c >=0 : Number of errors in error list
 *          @arg @c <0  : Inconsistency with old error count,
 *                        absolute value is new error counter
 *
 ******************************************************************************/

int cfreverr(void){

     CONFERR *_cfesav, *_cfenext;
     register int count=1;

     if( _cferr == NULL ) return 0;

     _cfenext = _cferr->next;
     _cferr->next = NULL;

     while( _cfenext ){
       count++;
       _cfesav = _cfenext->next;
       _cfenext->next = _cferr;
       _cferr = _cfenext;
       _cfenext = _cfesav;
     }

     return ( _errcnt == count ) ? _errcnt : -(_errcnt=count);
}

/** @} */
/******************************************************************************/
