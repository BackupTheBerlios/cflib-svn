/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file cfwrite.c
 *
 * Saving of configuration data and making Backup copies
 *
 * The functions cfsave() and FileBackup() deliver standard
 * functionality, but they are not used IN the library!
 *
 * Define @b _CF_NOSAVING to compile a library version without support
 * for these functions, if you want to reduce CFLIB code to a minimum
 * that is needed for configuration input and use in a program.
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU Lesser General Public License version 3.0 (LGPLv3)
 * @package   CFLIB
 * @subpackage Library_UI
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
 * 2008-08-03 [sh] header with meta info and license
 * 2009-08-22 [sh] Documentation Update
 * 2009-01-24 [sh] Renamed cfbackup() to BackupFile() (not working with DB!)
 *                 and use BackupFile for backup in cfsave()
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cflib.h"

#ifndef _CF_NOSAVING

/***************************************************************************//**
 *
 * @ingroup setting_saving
 *
 * Write configuration data to a Configuration File or @c stdout
 *
 * Entries with the @ref CF_NOSAVE flag will be excluded from the output.
 * Use cfnosave() to inquire or alter that flag for an entry.
 *
 * @param file
 *              @arg <@e string> : Name of regular file to be opened
 *              @arg @c ""       : Write configuration to @c stdout
 *              @arg @c NULL     : Private configuration file will be
 *                                 overwritten or created, if an appropriate
 *                                 entry exists
 *
 * @param savemode	File open mode:
 *                  @arg @c "w" : Overwrite
 *                  @arg @c "a" : Append
 *
 * @return
 *          @arg @c 0   : Configuration has been saved to file successfully
 *          @arg @c !=0 : An error occurred:
 *                        - @ref CFE_NCA : @copydoc CFE_NCA
 *                        - @ref CFE_IFP : @copydoc CFE_IFP
 *                        - @ref CFE_WAE : @copydoc CFE_WAE
 *
 ******************************************************************************/

int cfsave( char *file, const char *savemode ){

    int i;
    char *fname;
    FILE *fp;

    /* ready to work? */

    if(!_conf[0]) return CFE_NCA;

    /* fname=NULL -> save to priv. setfile if valid name in DB */

    if( (fname = file) == NULL ){

        for(i=0;i<MAXCONF;i++){
            if(_conf[i]->flag&CF_SETFILE){
                if ((fname=_conf[i]->inhalt) && *_conf[i]->inhalt ) break;
                else return CFE_IFP; /* priv. setfile is NULL or "" */
            }
            if(_conf[i]->flag&CF_LAST) return CFE_NSC;
        }

		/* Backup old private setfile */

		if( (i = BackupFile(fname, "r")) != 0 ) cfputerr(i, NULL);
    }

    /* fname="" -> write to stdout */

    if( *fname == '\0' ) fp = stdout;

    else if( !(*savemode=='a' || *savemode=='w')
    		 || savemode[1] != '\0'
             || !(fp = fopen(fname, savemode)) ) return CFE_WAE;

    /* Save configuration */

    for(i=0;i<MAXCONF;i++){
#ifdef DEBUG_NOSAVE
	fprintf(stderr,"entry %s: NOSAVE-flag=%s\n",_conf[i]->name,
                 (_conf[i]->flag&CF_NOSAVE)?"ON":"OFF");
#endif
        if(_conf[i]->inhalt && !(_conf[i]->flag&(CFG_SFILES|CF_NOSAVE))){
            fprintf(fp,"%s = ",_conf[i]->name);
            if(_conf[i]->flag&CF_FLAG)
                 fprintf(fp,"%s\n",(_conf[i]->inhalt[0])?"ON":"OFF");
            else fprintf(fp,"%s\n",_conf[i]->inhalt);
            fflush(fp);
        }

        if(_conf[i]->flag&CF_LAST) break;
    }

    if( fp != stdout ) fclose(fp);
    return CFE_OK;
}


/***************************************************************************//**
 *
 * @ingroup utilities
 *
 * Copy or Rename File @c "file" to Backup File @c "file~" or @c "file.bak"
 *
 * @param file  Name of regular file to be opened
 * @param modus	Backup Mode:
 *              @arg @c "r" Rename
 *              @arg @c "d" Duplicate, Copy
 *              @arg @c "@e ?" Default @c "d"
 *              @arg @c "@e ?d" docs-style (@c .bak)
 *              @arg @c "@e ?c", @c "??" Un*x Style (@c ~)
 *
 * @return
 *          @arg @c 0   : Configuration has been saved to file successfully
 *
 *          @arg @c !=0 : An error occurred:
 *                        - @ref CFE_BOF : Invalid filename / open error
 *                        - @ref CFE_BMF : @copydoc CFE_BMF
 *                        - @ref CFE_BRF : Rename file failed
 *                        - @ref CFE_BBF : Source file open error
 *                        - @ref CFE_BWF : Target file write error
 *
 ******************************************************************************/

int BackupFile( const char *file, char *modus )
{
	FILE *fin = NULL, *fb = NULL;
	char *buffer, *backfile, *tmp;
	int ret = 0, rbytes, wbytes;

	if( file == NULL || *file == '\0' ) return CFE_BOF;

	buffer = (modus[1] == 'd') ? ".bak" : "~";

	if( (backfile = malloc( (rbytes = strlen( file )) + strlen( buffer ) + 1L ))
		== NULL ) return CFE_BMF;

	strcpy( backfile, file );
	if( (modus[1] == 'd') && (tmp = strrchr( backfile, '.' )) ) *tmp = '\0';
	strcat( backfile, buffer );
	buffer = NULL;

	switch( *modus ){

		case 'r':
#ifdef DEBUG_BACKUP
			fprintf(stderr,"renaming %s -> %s\n",file, backfile);
#endif
			if( rename( file, backfile ) ) ret = CFE_BRF;
			break;

		case 'd':
		default:
			if( (buffer = malloc( CF_BACKBUFLEN )) == NULL )
				ret = CFE_BMF;
			else if( (fin = fopen( file, "rb" )) == NULL )
				ret = CFE_BOF;
			else if( (fb = fopen( backfile, "wb" )) == NULL )
				ret = CFE_BBF;
			else {
#ifdef DEBUG_BACKUP
				fprintf(stderr,"copying %s -> %s\n",file, backfile);
#endif
				while( (rbytes = fread( buffer, sizeof(unsigned char),
				                        CF_BACKBUFLEN, fin )) > 0 ){
					if( (wbytes = fwrite( buffer, sizeof(unsigned char),
					                      rbytes, fb )) != rbytes )
						ret = CFE_BWF;
					break;
				}
			}
			fclose( fin );
			fclose( fb );
			free( buffer );
			break;

	} /* end switch */

	free( backfile );

	return ret;
}

#endif

/******************************************************************************/
