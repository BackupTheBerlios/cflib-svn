/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 *//**
 *
 * @file cf_minimal.c
 *
 * Standalone Minimal Replacement ;-) for CFLIB Library (Functions)
 *
 * You can use CFLIB but still be able to compile and link your programs
 * without the library, e.g. for specific applications or unsupported platforms.
 * Just use this file instead of the library and tailor it to your need.
 *
 * The functions and variables defined here are a full replacement of the C API
 * of CFLIB. Most functions are dummies or simple substitutes, some are copies
 * of small functions, others are special adaptions - don't expect too much! ;-)
 *
 * The original CFLIB public header file cf.h should be included without change!
 *
 * @version   SVN: \$Id: cf_minimal.c 103 2009-01-27 16:08:02Z stefan $
 * @since     28.07.2008
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @category  Tools
 * @package   CFLIB
 * @subpackage Library_Replacement
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
 * 2008-07-28 [sh] Created
 * 2009-01-22 [sh] Introduced under examples
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

/**
 * List of Configuration Variable Pointers
 */
CONFIG *_conf[MAXCONF+1];

/**
 * Initialisierung der Konfiguration
 *
 * @param set   Liste der Konfigurationsparameter-Strukturen
 * @param ac    Anzahl der (Kommandozeilen-)Argumente
 * @param av    Argument-Struktur wie bei main()
 *
 * @return      Status/Error Code
 */
int cfinit( CONFIG *set, int ac, char **av ){

    register int i, j;
    char line[CF_MAXQLINE+1];
    CFFLAGTYP ret;
    char **avt, fat_err=FALSE;

    if((_conf[0]=malloc(sizeof(struct _cf)))==NULL) return CFE_MCF;
    _conf[0]->name="CF_PRGNAME"; _conf[0]->inhalt=""; _conf[0]->option='0';
    _conf[0]->flag=(CF_PRGNAME|CF_SET_PUT|CF_NO_OPT_ARG);

    /**
     * Initialize config parameters with hardcoded settings
     */
    for(i=1;i<MAXCONF;i++){
        if((_conf[i]=malloc(sizeof(struct _cf)))==NULL) return CFE_MCF;
        _conf[i]->name=set->name; _conf[i]->inhalt=set->inhalt;
        _conf[i]->option=set->option; _conf[i]->flag=set->flag;
        /* if(_conf[i]->flag&CF_SET_PUT_ARG) SetFlag(_conf[i]->flag,CF_FORCED); */
        if(DEBUG) fprintf(stderr,"Standalone cfinit Init Loop %d: %s=%s\n",i,_conf[i]->name,_conf[i]->inhalt); fflush(stderr);
        if(_conf[i]->flag&CF_LAST) break;
        set++;
    }

    /**
     * Get config parameters from command line arguments
     */
    for(j=ac, avt=av; j > 0; j--, avt++ ){
        if(**avt!='-') continue;
        if(DEBUG) fprintf(stderr,"Standalone cfinit Argument Loop %d: %s\n",j,avt[0]); fflush(stderr);
        for(i=0;i<MAXCONF;i++){
            if( avt[0][1]==_conf[i]->option
            	&& !(_conf[i]->flag&CF_NO_OPT_ARG) ){
            	avt++; j--;
            	if(DEBUG) fprintf(stderr,"Standalone cfinit Argument Value: %s\n",avt[0]); fflush(stderr);
                if(*avt==NULL){
                	/* Ignore fat_err=TRUE; */
                	break;
                } else {
                	_conf[i]->inhalt=*avt;
                    SetFlag(_conf[i]->flag,CF_SET_ARG);
                    break;
                }
            }
        }
        if(_conf[i]->flag&CF_LAST) break;
    }

    if(DEBUG) fprintf(stderr,"Standalone cfinit running\n"); fflush(stderr);

    /**
     * Ask for config parameters, get from stdin
     */
    for(i=0;i<MAXCONF;i++){
        if( _conf[i]->flag&CF_SET_QRY ||
        	(_conf[i]->inhalt==NULL && _conf[i]->flag&CF_QUERY)
          ){
        	if(DEBUG) fprintf(stderr,"Enter %s: ",_conf[i]->name); fflush(stderr);
            if(fgets(line,CF_MAXQLINE,stdin)){
                RemoveCR(line);
                if(_conf[i]->flag&CF_MALLOC) free(_conf[i]->inhalt);
                if( (_conf[i]->inhalt=malloc(strlen(line)+1)) == NULL) return CFE_MCF;
                else SetFlag(_conf[i]->flag,CF_MALLOC);
                strcpy(_conf[i]->inhalt,line);
                _conf[i]->flag=(_conf[i]->flag&(~(CF_SET_PUT|CF_SET_ARG|CF_SET_ENV|CF_SET_PRIV|CF_SET_SYS|CF_SET_DEF|CF_SET_QRY)))|CF_SET_QRY;
            }
        }
        if( _conf[i]->flag & CF_MUST
            && !( _conf[i]->inhalt != NULL && strlen(_conf[i]->inhalt) )
          ){
        	/* Error CFE_URI,"You must specifiy something for %s" */
        }
        if(_conf[i]->flag&CF_LAST) break;
    }
    ret=1;
    return (fat_err ? -ret : ret);
}

/**
 * General retrieval of configuration value
 *
 * @param string    Configuration Variable Name
 * @param typ       CFLIB Flag Type
 * @return          Configuration Variable Value (Pointer)
 */
void *cfgetent(char *string, CFFLAGTYP typ){

    register int i;
    static int ret=0;
    static float retf=0.0;

    if( _conf[0] == NULL ) return NULL;

	for(i=0;i<MAXCONF;i++){
		if( !strcmp(_conf[i]->name,string) ){

			switch( typ & (CF_STR|CF_INT|CF_FLAG|CF_REAL|CF_RESID) ){
				case CF_INT: ret=atoi(_conf[i]->inhalt);	break;
				case CF_REAL: retf=atof(_conf[i]->inhalt); break;
				case CF_FLAG: ret=(_conf[i]->inhalt[0]?TRUE:FALSE); break;
				case CF_STR: default: return (_conf[i]->inhalt); break;
			}; break;

		} else if( _conf[i]->flag & CF_LAST || i+1==MAXCONF ){

			switch( typ & (CF_STR|CF_INT|CF_FLAG|CF_REAL|CF_RESID) ){
				case CF_INT: ret=CFE_INF; break;
				case CF_REAL: retf=CFE_RNF;	break;
				case CF_FLAG: ret=CFE_NEA; break;
				case CF_STR: default: return NULL; break;
			}; break;
		}
	}
	if( typ & CF_REAL ) return &retf;
    return &ret;
}

/**
 * Debugging Function
 *
 * @param fout	Output File Pointer
 * @return
 *            - Positive count of configuration items
 *            - CFE_NCA no config available error
 *            - CFE_NLE no last entry found error
 *
 * @todo Known Bug:
 *       Error return will be ambigous if we have (more than) 119 Config Variables
 */
int cfdump(FILE *fout){

    register int i;

    if(!_conf[0]) return CFE_NCA;

    fputs( "Source Flags    Option  Variable           \tContent\n", fout );
    for(i=0;i<MAXCONF;i++){
        fprintf( fout, "[%03lx]  [%08lx]  [%c]  %-20s\t%s\n",
						(_conf[i]->flag&(CF_SET_PUT|CF_SET_ARG|CF_SET_ENV|CF_SET_PRIV|CF_SET_SYS|CF_SET_DEF|CF_SET_QRY))>>8,
						_conf[i]->flag, _conf[i]->option,
						_conf[i]->name, _conf[i]->inhalt );
        if(_conf[i]->flag&CF_LAST) return (i+1);
    }
	return CFE_NLE;
}

/**
 * Get CFLIB SubVersion
 *
 * @return CFLIB Version based on current SVN Keyword Id
 */
char *cfgetsubvers(){
	return "CFLIB Minimalversion $Id: cf_minimal.c 103 2009-01-27 16:08:02Z stefan $";
}

/**
 * Get CFLIB Version
 *
 * @return @c -1 Not CFLIB but minimal Substitute
 */
int cfgetvers(){
	return (-1);
}

/**
 * Get Usage Message Function Dummy
 *
 * @return Dummy-Output
 */
char *cfgetusg(){
	return "CFLIB nicht verfuegbar - keine Hilfsmeldung :-(";
}

/**
 * Function Dummy
 * @return @c 0
 */
int cfputstr(char *name, char *content){
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfputtime(CFFLAGTYP td){
	return (0);
}
/**
 * Function Dummy
 */
void cfexit(){
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfstart(CONFIG *set, int ac, char **av, char *help, int mode){
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfgeterr(char *string, size_t len){
	return (0);
}
/**
 * Function Dummy
 */
void cfclearerr(){
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfreverr(){
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfsave(char *fname, const char *savemode){
	if(DEBUG) fputs("CFLIB nicht verfuegbar - kein Abspeichern der Konfiguration moeglich :-(\n",stderr);
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfnosave(char *name, const char *onoff){
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfform(char *infile, char *outfile, char *vardelim, int mode){
	if(DEBUG) fputs("CFLIB nicht verfuegbar - keine Berichtserstellung moeglich :-(\n",stderr);
	return (0);
}
/**
 * Function Dummy
 * @return  Input unchanged
 */
char *cfhomexp( char *name ){
	return (name);
}
/**
 * Function Dummy
 * @return @c 0
 */
int cfdinichk(CONFIG *set){
	return (0);
}
/**
 * Function Dummy
 * @return @c 0
 */
int FileBackup(const char *file, char *modus){
	return (0);
}


/***************************************************************************
 *
 *//** @fn void RemoveCR(char *ptr)
 *
 * String Utility Function
 *
 * Strip Carriage Return at end of string (after fgets) by introducing
 * zero byte at CR position
 *
 * @param   ptr	pointer to beginning of string
 *
 **************************************************************************/

void RemoveCR(char *ptr){

    char *tmp;
    if((tmp=strchr(ptr,'\n')) != NULL) *tmp = '\0';
}

/***************************************************************************
 *
 *//** @fn void RemoveTrailSpace(char *ptr)
 *
 * String Utility Function
 *
 * Strip whitespaces at end of string by introducing zero byte after
 * last non-whitespace character
 *
 * @param   ptr	pointer to beginning of string
 *
 **************************************************************************/

void RemoveTrailSpace(char *ptr){

    char *tmp;
    tmp=ptr;
    while(*tmp) tmp++;
    tmp--;
    while(*tmp == ' ' || *tmp == '\t') tmp--;
    tmp++;
    *tmp='\0';
}

/***************************************************************************
 *
 *//** @fn char *EatWhiteSpace(char *ptr)
 *
 * String Utility Function
 *
 * Set pointer to next non-whitespace-character in string
 *
 * @param   ptr	pointer to beginning of string
 * @return	    pointer to next non-whitespace-character in string
 *
 **************************************************************************/

char *EatWhiteSpace(char *ptr){

    while(*ptr == ' ' || *ptr == '\t') ptr++;
    return ptr;
}

/* END */
