/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal
 *
 * @file cfmain.c
 *
 * Initialization and Exit of CFLIB functionality
 *
 * @version   SVN: \$Id: cfmain.c 128 2009-01-28 16:51:43Z stefan $
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
#include <ctype.h>
#include <time.h>

#ifdef atarist
#include <support.h>
#endif

#include "cflib.h"

/***************************************************************************//**
 *
 * @ingroup cflib_core
 *
 * Initialize CFLIB Configuration Database and parse possible sources for
 * database entries according to the settings in @e set
 *
 * @param set   @ref config_initializer :
 *              Pointer to Array of @ref CONFIG items containing parameter name,
 *              default value, @ref parameter_option and
 *              @ref special_options_mask
 *
 * @param ac    Argument Count from commandline or compatible
 *
 * @param av    Argument String array from commandline or compatible structure
 *
 * @return
 *          @arg @c  0 : configuration database initialized successfully
 *          @arg @c >0 : Count of non fatal errors
 *          @arg @c <0 : Fatal error(s) occurred:
 *                       - @ref CFE_MCF : @copybrief CFE_MCF
 *                       - @ref CFE_MEF : @copybrief CFE_MEF
 *                       - any other absolute value is total error count
 *
 * @see @ref config_levels
 *
 ******************************************************************************/

int cfinit( CONFIG *set, int ac, char **av )
{
    register int i, j;
    int acount=0, res_arg, ecount;
    CFFLAGTYP cfglob = 0, ret;
    CONFIG *setsave = set;
    char **avt, fat_err = FALSE, *tmp;
    char *path = NULL;

    /**
     * Clear error list
     */
    cfclearerr();

    /**
     * Configuration Database structure initialization
     */
    if( (_conf[0]=malloc(sizeof(struct _cf))) == NULL ) return CFE_MCF;

    /**
     * @ref CF_PRGNAME will be set from first element of argv in
     * arguments parsing section, if present
     */
    _conf[0]->option = '0';
    _conf[0]->name   = "CF_PRGNAME";
    if( ac > 0 ){
		_conf[0]->inhalt = NULL;
		_conf[0]->flag   = CF_PRGNAME | CF_SET_ARG | CF_NO_OPT_ARG;
	} else {
		_conf[0]->inhalt = "";
		_conf[0]->flag   = CF_PRGNAME | CF_SET_PUT | CF_NO_OPT_ARG;
	}

    for(i=1;i<MAXCONF;i++){

        if( (_conf[i]=malloc(sizeof(struct _cf))) == NULL ) return CFE_MCF;

        _conf[i]->name   = set->name;
        _conf[i]->inhalt = NULL; /* content init with NULL */
        _conf[i]->option = set->option;
        _conf[i]->flag   = set->flag;

        /* remember existence */
        cfglob |= _conf[i]->flag & CFG_ALL;

        if( _conf[i]->flag & CF_SET_PUT_ARG )
        	SetFlag(  _conf[i]->flag, CF_FORCED );

        if( _conf[i]->flag&CF_LAST ) break;

        set++;
    };

    res_arg = ecount = i;

    /**
     * Parse argument line: options and residual arguments
     */
    for( j=ac, avt=av; j > 0; j--, avt++ ){

        for(i=0;i<MAXCONF;i++){

            if( **avt != '-' ){

                if( _conf[i]->flag & CF_NO_OPT_ARG
                	&& ( (_conf[i]->option-'0') == acount ) ){

                    _conf[i]->inhalt = *avt;
                    acount++;
                    SetFlag(_conf[i]->flag,CF_SET_ARG);
                    break;

                } else if( _conf[i]->flag&CF_LAST ){

                	/* Residual argument */
                    if( (_conf[++res_arg]=malloc(sizeof(struct _cf)))
                    	== NULL ) return CFE_MCF;
                    _conf[res_arg]->name = CF_FLAG_ON;
                    _conf[res_arg]->inhalt = *avt;
                    _conf[res_arg]->option = ' ';
                    _conf[res_arg]->flag = CF_SET_ARG|CF_RESID|CF_NOSAVE;
                    break;
                }

            } else {

            	/* Option */
                if( avt[0][1] == _conf[i]->option
                	&& !(_conf[i]->flag & CF_NO_OPT_ARG) ){

                    if( _conf[i]->flag & CF_NO_ARG_OPT ){

                        _conf[i]->inhalt=&avt[0][2];
                        _conf[i]->flag|=CF_SET_ARG;
                        break;

                    } else {

                        avt++;
                        j--;

                        if( *avt == NULL ){

                            if( cfputerr( CFE_ORA,
                                 "Option \"-%c\" ignored! (Needs an argument!)",
                                 _conf[i]->option )
                                == CFE_MEF ) fat_err = TRUE;

                        } else {

                            _conf[i]->inhalt = *avt;
                            SetFlag(_conf[i]->flag,CF_SET_ARG);
                            break;
                        }
                    }

                } else if( _conf[i]->flag & CF_LAST ){

                    if( cfputerr( CFE_UKO, "Unknown option: \"%s\"\n",
                                  avt[0] ) == CFE_MEF ) fat_err = TRUE;
                }
            }
            if( _conf[i]->flag & CF_LAST || j < 0 ) break;
        }
    }

    /**
     * Parse environment
     */
    for( i=0; i < MAXCONF; i++ ){

        if( ( !(_conf[i]->flag&CF_SET_ARG) || _conf[i]->flag&CF_SET_ENV )
        	  && !(_conf[i]->flag&CF_IGN_ENV) ){

        	_conf[i]->inhalt = getenv(_conf[i]->name);
            if( _conf[i]->inhalt ) SetFlag(_conf[i]->flag,CF_SET_ENV);
        }
        if(_conf[i]->flag&CF_LAST) break;
    }

    /* #ifdef CGI_SUPPORT moved to SVN branch CGI */

    /**
     * Parse private setfile, if exists
     */
    if( (ret = cfreadfile( CFG_FPRIV, &res_arg )) == CFE_MCF )
		fat_err = TRUE;
	else SetFlag(cfglob,ret);

	if( DEBUG )	fprintf( stderr,
	    "cfreadfile(private file) returned %lx, cfglob=%lx\n", ret, cfglob );

	/**
	 * Parse system setfile, if exists
	 */
    if( (ret=cfreadfile(CFG_FSYS,&res_arg)) == CFE_MCF ) fat_err = TRUE;
    else SetFlag(cfglob,ret);

	if( DEBUG ) fprintf( stderr,
	    "sys cfread returned %lx, cfglob=%lx\n",ret,cfglob );

	/**
	 * Apply default settings where necessary
	 */
    for( i=1; i < MAXCONF; i++, setsave++ ){

        if( ( !_conf[i]->inhalt && !(_conf[i]->flag&CF_SET_PUT_ARG) )
        	|| _conf[i]->flag&CF_SET_DEF ){

            if( _conf[i]->flag&CF_MALLOC ){
                free(_conf[i]->inhalt);
                DelFlag(_conf[i]->flag,CF_MALLOC);
            }
            _conf[i]->inhalt = setsave->inhalt;
            if( _conf[i]->flag & CF_SET_DEF ) SetFlag(_conf[i]->flag,CF_FORCED);

            /* _conf[i]->flag=(_conf[i]->flag&(~CF_SET))|CF_SET_DEF;    */
            /* removed ~CF_SET, cause overwrites CF_SET_SYS-Instruction */

            _conf[i]->flag |= CF_SET_DEF;
        }
        if(_conf[i]->flag&CF_LAST) break;
    }

    /**
     * Try again system configuration file, if indicated
     */
    if( cfglob & CFG_SAGAIN ){

        if( (ret=cfreadfile(CFG_FSYS,&res_arg)) == CFE_MCF ) fat_err = TRUE;
        else SetFlag(cfglob,ret);

        if( DEBUG ) fprintf( stderr,
            "priv cfread returned %lx, cfglob=%lx\n", ret, cfglob );
    }

    /**
     * Try again system configuration file, if indicated
     */
    if( cfglob & CFG_PAGAIN ){

        if( (ret=cfreadfile(CFG_FPRIV,&res_arg)) == CFE_MCF ) fat_err = TRUE;
        else SetFlag(cfglob,ret);

        if( DEBUG ) fprintf( stderr,
            "sys cfread returned %lx, cfglob=%lx\n", ret, cfglob );
    }

    /**
     * Get entry interactively, if indicated
     */
    if( cfreadterm() < 0 ) fat_err = TRUE;

    /**
     * Flag processing
     */
    if( cfglob & CF_FLAG ){

        for(i=0;i<MAXCONF;i++){

            if( (_conf[i]->flag & CF_FLAG)
            	&& !(_conf[i]->flag & CF_SET_DEF) ){

                tmp = _conf[i]->inhalt;

                if( (_conf[i]->inhalt)
                	&& !( (_conf[i]->inhalt[0] == '-')
                		  || (!strncmp(_conf[i]->inhalt,"OFF",2))
                		  || (!strncmp(_conf[i]->inhalt,"FALSE",2))
                		  || (!strncmp(_conf[i]->inhalt,"off",2))
                		  || (!strncmp(_conf[i]->inhalt,"false",2))
                		  || (!strncmp(_conf[i]->inhalt,"NEIN",2))
                		  || (!strncmp(_conf[i]->inhalt,"nein",2))
						)
                ) _conf[i]->inhalt = CF_FLAG_ON;
                else _conf[i]->inhalt = CF_FLAG_OFF;

                if( _conf[i]->flag & CF_MALLOC ){
                    free(tmp);
                    DelFlag(_conf[i]->flag,CF_MALLOC);
                }
            }
            if(_conf[i]->flag&CF_LAST) break;
        }
    }

    /**
     * Filename, Date/Time and Path processing
     */

    /* remove path from program name */
    if( (tmp=strrchr(_conf[0]->inhalt,'\\') ) ) _conf[0]->inhalt = ++tmp;
    if( (tmp=strrchr(_conf[0]->inhalt,'/')  ) ) _conf[0]->inhalt = ++tmp;

    /* get path */
    if( cfglob & CF_PATH ){

        for(i=0;i<MAXCONF;i++){

            if( _conf[i]->flag & CF_PATH ){

              if( _conf[i]->inhalt != NULL && _conf[i]->inhalt[0] ){

                if( _conf[i]->flag & CF_EXPHOME
                	&& _conf[i]->inhalt[0] == '~' ){

                	/* Home directory expansion */
                	if( cfhomexp(_conf[i]->name) == NULL ) fat_err = TRUE;
                }
                path = _conf[i]->inhalt;
              }
              break;
            }

            if(_conf[i]->flag&CF_LAST) break;
        }
    }
    if( path == NULL ) path = getenv( "PATH" ); /* Default from environment */

    for(i=0;i<MAXCONF;i++){

        if( _conf[i]->flag & CF_TD && _conf[i]->flag & CF_SET_PUT ){

        	/**
        	 * Set date or time
        	 */
            cfputtime( _conf[i]->flag & CF_TD );

        } else {

        	/**
        	 * Home directory expansion
        	 */
        	if( _conf[i]->flag & CF_EXPHOME
        		&& _conf[i]->inhalt != NULL	&& _conf[i]->inhalt[0] == '~' ){

        		if( cfhomexp(_conf[i]->name) == NULL ) fat_err = TRUE;
        	}

#ifndef _CF_NOFINDFILE
        	/**
			 * FindFile function
			 */
			if( _conf[i]->flag & CF_FINDFILE && _conf[i]->inhalt != NULL ){

				if( (tmp = FindFile( (const char *)_conf[i]->inhalt,
				                     (const char *)path, NULL ) ) != NULL ){
#ifdef DEBUG_FINDFILE
					fprintf( stderr, "path=%s\n", path);
					fprintf( stderr, "findfile for: %s\n",_conf[i]->inhalt );
					fprintf( stderr, "found: %s\n", tmp);
#endif
					if( _conf[i]->flag & CF_MALLOC ) free( _conf[i]->inhalt );

					if( (_conf[i]->inhalt = malloc( strlen( tmp ) + 1 ))
						== NULL ) fat_err = TRUE;
					else SetFlag(_conf[i]->flag,CF_MALLOC);

					strcpy( _conf[i]->inhalt, tmp );

				    /*
				     * Moved to SVN branch CGI:
				     * [trying that for quick soultion to cgi-security support:
				     * cfreadfile( CFG_FSYS, &res_arg ) == CFE_MCF -> fat_err
				     * cfreadfile( CFG_FPRIV, &res_arg ) == CFE_MCF -> fat_err]
				     */
				}
			}
#endif

        }
		if( _conf[i]->flag & CF_LAST ) break;
	}

    /**
     * Move "CF_LAST-mark"
     */
    if( ecount != res_arg ){

    	DelFlag(_conf[ecount]->flag,CF_LAST);
        SetFlag(_conf[res_arg]->flag,CF_LAST);
    }

    /**
     * Return with indication of fatality and number of errors
     */

    ret = cfputerr( 0, NULL );

    return ( fat_err ? -ret : ret );
}


/***************************************************************************//**
 *
 * @ingroup cflib_core
 *
 * Free allocated memory and reset the configuration database and error stack
 *
 * This function should be used before a repeated call to cfinit() or cfstart().
 * Furthermore, it may be desired to call cfexit() when the configuration
 * database occupies a lot of memory and is no longer needed.
 *
 ******************************************************************************/

void cfexit( void )
{
    register int i;

    cfclearerr();

    for(i=0;i<MAXCONF;i++){

        if( _conf[i]->flag & CF_MALLOC ) free(_conf[i]->inhalt);
        if( _conf[i]->flag & CF_LAST ){
            free(_conf[i]);
            break;
        } else free(_conf[i]);
        _conf[i] = NULL;
    }
}

/******************************************************************************/
