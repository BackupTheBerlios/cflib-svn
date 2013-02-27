/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal
 *
 * @file cfread.c
 *
 * Functions for reading input from a File or Terminal
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009, 2013 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU Lesser General Public License version 3.0 (LGPLv3)
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
 * 2013-02-25 [sh] Added option of sysfile entries overriding privfile config
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cflib.h"

/***************************************************************************//**
 *
 * @private
 *
 * Library internal function: Parse Configuration File
 *
 * @param setfile   Config File Identification:
 *                  @arg @ref CFG_FPRIV Private Configuration File
 *                  @arg @ref CFG_FSYS  System Configuration File
 *
 * @param res_arg   Last Residual Item Mark
 *
 * @return
 *          - @c 0    Configuration file parsed successfully
 *          - @c >0   Error / Message:
 *                    @arg @ref CFG_PAGAIN
 *                        Could not parse private setfile, try again later
 *                    @arg @ref CFG_SAGAIN
 *                        Could not parse system setfile, try again later
 *                    @arg @ref CFG_NOPFILE
 *                        Could not access private setfile, because none
 *                        configured, bad entry or file not found
 *                    @arg @ref CFG_NOSFILE
 *                        Could not access system setfile, because: see above
 *          - @c <0   Fatal Error(s) occurred:
 *                    @arg @ref CFE_MCF malloc() for database entry failed
 *                    @arg @ref CFE_MEF malloc() for error list failed
 *
 ******************************************************************************/

CFFLAGTYP cfreadfile( CFFLAGTYP setfile, int *res_arg )
{
    FILE *fp;
    char line[CF_MAXLINE], *tmp1, *inhalt;
    char *name, *fname=NULL, *section=NULL;
    register int i;
    int secread=1, nmatch=-1;
    CFFLAGTYP overrides = CF_OVRD_SETFILE;

    /* search entry with appropriate flag and check content */

    for(i=0;i<MAXCONF;i++){

        /* setfile entry ? */

        if(_conf[i]->flag&(setfile&CFG_SFILES)){
            /* return if no content for setfile entry ? */
            if(_conf[i]->inhalt==NULL) return (setfile&CFG_AGAIN);
            /* ... or empty string in content */
            else if (*_conf[i]->inhalt=='\0'){
                if(cfputerr(CFE_EFE,"Empty string in filename entry for %s configuration file\n",(setfile==CFG_FPRIV)?"private":"system")
                ==CFE_MEF) return CFE_MEF;
                return (setfile&CFG_NOFILE);
            }

            fname=_conf[i]->inhalt;	/* filename */
            nmatch=i;		     	/* remember setfile index for ~-expansion */

            /* if system setfile and no section entry continue section search */
            if(setfile&CFG_FPRIV || section) break;
        }

        /* section entry for sys setfile ? */

        else if (setfile&CFG_FSYS && _conf[i]->flag&CF_SECTION){

            /* no section specified -> try again later */
            if((section=_conf[i]->inhalt)==NULL) return CFG_SAGAIN;
            /* turn off secread for section search */
            secread=0;
            /* include private file in overrides unless special sysfile priority flag is set */
            if( !(_conf[i]->flag&CF_SET_SYS) ) overrides |= CF_SET_PRIV;
            if(fname!=NULL) break; /* sysfile and section known */
        }

        if(_conf[i]->flag&CF_LAST){
            if (fname!=NULL) break; /* if sysfile: no section entry configured */
            else return (CFG_NOFILE&setfile); /* no setfile entry configured */
        }
    }

    /* expand home directory, if neccessary */

    if(*fname=='~'){
    	if(nmatch==-1) fname=NULL; /* no setfile found */
		fname=cfhomexp(_conf[nmatch]->name);
    }
    if(fname==NULL) return CFE_MCF;

    /* open setfile */

    if((fp=fopen(fname,"r"))==NULL){
        if(cfputerr(CFE_FNF,"File %s could not be opened for reading\n",fname)
        ==CFE_MEF) return CFE_MEF;

        return (setfile&CFG_NOFILE);
    }

    /* parse file content */

    while(!feof(fp)){
        if (fgets(line,CF_MAXLINE,fp)==NULL) break;

        /* section entry in sysfile ? */

        if(section && (*line=='[')){

            if (secread) break;   /* end of section just been read */
            else if ((tmp1=strchr(line,']'))!=NULL){
                *tmp1='\0';
                /* section matches required one ? */
                if( (strlen(&line[1])==strlen(section)) &&
                !strcmp(&line[1],section) ) secread++; /* turn on secread */
            }

            else if(cfputerr(CFE_NSE,"Section specification without ']' in %s: %s\n",fname,line)
            ==CFE_MEF){
                fclose(fp);
                return CFE_MEF;
            }
        } /* if section entry */

        /* possible configuration entry ? */

        else if ( !(*line=='#') && secread ){

            if ((tmp1=strchr(line,'='))!=NULL){
                /* taylor name and content from line */
                RemoveCR(line);
                *tmp1=' ';
                inhalt=EatWhiteSpace(tmp1);
				RemoveTrailSpace(inhalt);
                name=EatWhiteSpace(line);
                if((tmp1=strchr(name,' '))!=NULL) *tmp1='\0';

                /* compare with setting */
                nmatch=FALSE;
                for(i=0;i<MAXCONF;i++){

                    if(!strcmp(_conf[i]->name, name)){         /* name matches */
                        nmatch=TRUE;

                        if(                                     /* and */

                        !(_conf[i]->flag&CF_FORCED)       /* nothing forced before */

                        &&                                      /* and */
                        ( !( _conf[i]->flag & overrides ) /* no setting with higher priority */
                         ||                                     /* or */
                         _conf[i]->flag&(CF_SET_FIL&setfile) /* force is set */
                        )
                        ){
                            /* prepare conf->inhalt */
                            if(_conf[i]->flag&CF_MALLOC) free(_conf[i]->inhalt);
                            if( (_conf[i]->inhalt=malloc(strlen(inhalt)+1))
                            ==NULL){
                                fclose(fp);
                                return CFE_MCF;
                            }
                            else SetFlag(_conf[i]->flag,CF_MALLOC);

                            /* copy entry content */
                            strcpy(_conf[i]->inhalt,inhalt);

                            /* set sourceflag */
                            if( _conf[i]->flag & (CF_SET_FIL&setfile) )
                            	SetFlag(_conf[i]->flag, CF_FORCED);
                            _conf[i]->flag =
                            	(_conf[i]->flag&(~CF_SET))|(CF_SET_FIL&setfile);
                        }
                    }

                    if( _conf[i]->flag & CF_LAST ){

                        /* put residual item into cfbase */
                        if(!nmatch){
                            if( (_conf[++(*res_arg)]=malloc(sizeof(struct _cf)))
                            	== NULL) return CFE_MCF;
                            if( (_conf[*res_arg]->inhalt=malloc(strlen(inhalt)+1))
                            	== NULL ){
                                fclose(fp);
                                return CFE_MCF;
                            }
                            strcpy(_conf[*res_arg]->inhalt,inhalt);
                            if( (_conf[*res_arg]->name=malloc(strlen(name)+1))
                            	== NULL ){
                                fclose(fp);
                                return CFE_MCF;
                            }
                            strcpy(_conf[*res_arg]->name,name);
                            _conf[*res_arg]->option='x';
                            _conf[*res_arg]->flag=
                            	((CF_SET_FIL&setfile)|CF_RESID|CF_MALLOC);
                        } /* end if(nmatch==FALSE) */

                        break;
                    } /* end if(_conf[i]->flag&CF_LAST) */

                } /* end for(i=0,i<MAXCONF;i++) */

            } /* if '=' in line */

        } /* else if possible configuration entry */

    } /* while (!feof(fp)) */
    fclose(fp);

    return CFE_OK;
}

/***************************************************************************//**
 *
 * @private
 *
 * Library internal function: Query for unresolved items, if configured
 *
 * @return
 *          - @c >=0 Number of successful queries
 *          - @c <0  Fatal Error occurred:
 *                   @arg @ref CFE_MCF malloc() for database entry failed
 *                   @arg @ref CFE_MEF malloc() for error list failed
 *
 ******************************************************************************/

int cfreadterm( void )
{
	register int i;
	int count = 0;
	char line[CF_MAXQLINE+1];

	for(i=0;i<MAXCONF;i++){

		/* Force setting from query or ask for unresolved item? */

		if( _conf[i]->flag & CF_SET_QRY
			|| ( _conf[i]->inhalt == NULL && _conf[i]->flag & CF_QUERY )
		){ /* no question, if not a terminal */
			if( IsATerminal(stdin) ){
				fprintf( stdout, "Enter %s: ", _conf[i]->name ); fflush(stdout);
			}

			if( !fgets(line,CF_MAXQLINE,stdin) ){

				if( cfputerr( CFE_TIN, "Could not read %s from standard input\n",
										_conf[i]->name )
					== CFE_MEF ) return CFE_MEF;

			} else{  /* interactive input processing */

				RemoveCR(line);
				/* prepare conf->inhalt */
				if(_conf[i]->flag&CF_MALLOC) free(_conf[i]->inhalt);
				if( (_conf[i]->inhalt=malloc(strlen(line)+1)) == NULL) return CFE_MCF;
				else SetFlag(_conf[i]->flag,CF_MALLOC);
				/* copy content */
				strcpy(_conf[i]->inhalt,line);
				/* set sourceflag */
				_conf[i]->flag=(_conf[i]->flag&(~CF_SET))|CF_SET_QRY;
				count++; /* query count */
			}
		} /* query required? */

		/* Still NULL pointer in entry that should be set from commandline ? */

		if(_conf[i]->inhalt==NULL && _conf[i]->flag&CF_SET_ARG){

			if( cfputerr( CFE_USG, "missing argument: %s%c%s%s%s<%s>%s\n",
									(_conf[i]->flag&CF_NO_OPT_ARG) ? "" : "-",
									_conf[i]->option,
									(_conf[i]->flag&CF_NO_OPT_ARG) ? "." : "",
									(_conf[i]->flag&(CF_CONCAT|CF_FLAG)) ? "" : " ",
									(_conf[i]->flag&CF_FLAG) ? "[-][" : "",
									_conf[i]->name,
									(_conf[i]->flag&CF_FLAG) ? "]" : "" )
				== CFE_MEF ) return CFE_MEF;
		}

		if(_conf[i]->flag&CF_MUST){
			if (!(_conf[i]->inhalt!=NULL && strlen(_conf[i]->inhalt))){
				if( cfputerr( CFE_URI, "You must specifiy something for %s\n",
							  _conf[i]->name )
					== CFE_MEF ) return CFE_MEF;
			}
		}

		if( _conf[i]->flag & CF_LAST ) break;

	} /* end loop over all entries */

	return count; /* >=0 */
}

/******************************************************************************/
