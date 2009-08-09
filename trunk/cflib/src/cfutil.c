/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file cfutil.c
 *
 * Utility Functions
 *
 * The functions cfform(), cfstart() and cfhomexp() deliver standard
 * functionality, but they are not necessary for CFLIB basic functionality.
 *
 * The functions RemoveCR(), RemoveTrailSpace(), EatWhiteSpace()
 * and IsATerminal() belong to the obligatory CFLIB core functions.
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @package   CFLIB
 * @subpackage Library_Utilities
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
 * 2008-08-04 [sh] RemoveCR() strips now also CR character "\r"
 * 2009-08-22 [sh] Documentation Update
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __MINT__
#include <stat.h>
#include <types.h>
#endif
#ifdef _HAS_PWD
#include <pwd.h>
#endif

#include "cflib.h"

/***************************************************************************//**
 *
 * @ingroup report_generation
 *
 * Process a Template from file or @c stdin and write generated Report to
 * File or @c stdout
 *
 * The Report Template contains @b Variables like '\$(@e ident)' or whatever
 * you set in @e vd. @e ident may refer to a:
 *
 * - Parameter Name like '\$(@e name)' :
 *   Substitute variable by parameter's value (from DB)
 * - Include File Path like '\$(FILE:@e /my/path/to/incfile)' :
 *   Substitute variable by content of include file
 * - File Path and Section Include like '\$(FILE:@e incfile#@e sect)' :
 *   Include section from include file between [@e sect] and next [@e ...]
 * - Variable File Include like '\$(FILE::@e varname)' :
 *   Include file from location given in parameter @e varname
 *
 * @see @ref advanced_example
 *
 * @todo Make cfform() work with buffers instead of files
 *
 * @param file      Name of Template File, @c NULL for @c stdin
 *
 * @param outfile   Name of Output File, @c NULL for @c stdout
 *
 * @param vd        Variable Delimiters:
 *                  string containing the three variable delimiters in the
 *                  first three chars: to have variables like '\$(@e name)',
 *                  let the string be '\$()'. This is also the default,
 *                  if @e vd is @c NULL or string length < 3
 *
 * @param mode      Mode Mask:
 *                  @arg @c 0 : Normal
 *                  @arg @c 1 : Query for unresolved variables,
 *                              (try to) include them in database
 *                  @arg @c 2 : Unset @ref CF_NOSAVE flag for parsed vars
 *                  @arg @c 4 : Outfile Write @e append, else @e write
 *
 * @return
 *      @arg @c 0   : Configuration has been saved to file successfully
 *      @arg @c !=0 : An error occurred:
 *                    - @ref CFE_NCA : @copybrief CFE_NCA
 *                    - @ref CFE_FNF : @copybrief CFE_FNF
 *                    - @ref CFE_WAE : @copybrief CFE_WAE
 *
 ******************************************************************************/

int cfform( char *file, char *outfile, char *vd, int mode )
{
	register int i;
	char found, incinit;
	int varincount=-1, inccount=-1, noincsect=0, incfiles=0;
	char line[CF_MAXLINE+1], qbuf[CF_MAXQLINE+1], incbuf[CF_MAXQLINE+1];
	char linesav[CF_MAXINC+1][CF_MAXLINE+1], linesavinc[CF_MAXINC+1][CF_MAXLINE+1];
	char *start_output_poi, *search_var_poi, *var_esc_poi, *var_poi, *var_end_poi;
	char *section=NULL, *inc_section=NULL, *infile, *tmp1;
	FILE *fp, *fout, *fpsav[CF_MAXINC+1];

#ifdef BIN_FORM1
	size_t blen;
	char buf[CF_MAXLINE+1];
#endif

	if(!_conf[0]) return CFE_NCA;   /* ready to work? */

	if( file == NULL ) fp = stdin;  /* read from stdin (usually pipe) */

	else {							/* Get template stream from file */

		strcpy( qbuf, file );
		infile = qbuf;
		if( ( section = strchr(qbuf,'#') ) != NULL ){
			*section = '\0';
			section++;
#ifdef DEBUG_FORM
			fprintf(stderr,"required section <%s> in file <%s>\n",section,var_poi);
#endif
		}

		if( (fp=fopen(infile,"r")) == NULL ) return CFE_FNF;

		if( section != NULL ){
			noincsect = 1;
			while( !feof(fp) ){
				if (fgets(line,CF_MAXLINE,fp)==NULL) break;
				/* section start in line ? */
				if( *line == '[' && (tmp1=strchr(line,']')) != NULL ){
					*tmp1='\0';
#ifdef DEBUG_FORM
					fprintf(stderr,"found section <%s>\n",&line[1]);
#endif
					/* section matches required one ? */
					if( ( strlen(&line[1]) == strlen(section) )
						&& !strcmp(&line[1],section) ) break;
				}
			}
		}
	}

	if( outfile == NULL ) fout = stdout;
	else if( ( fout = fopen( outfile, (mode&4) ? "a" : "w" ) ) == NULL ) return CFE_WAE;

	if( vd == NULL || strlen(vd) < 3 ) vd = CF_DEF_VARDELIM;

#ifdef BIN_FORM1
	*buf='\0';
#endif

	/* Main loop */

	while( TRUE ){

		incinit = FALSE;

		if( !feof(fp) ){

#ifdef BIN_FORM
			/* if there's something in the buffer, include it at beginning of line */
#ifdef DEBUG_FORM
			fprintf(stderr,"blen=%ld, strlen(buf)=%ld\n",blen,strlen(buf));
#endif
			blen=strlen(buf);
			if(blen){
				strncpy(line,buf,blen);
				*buf='\0';
			}
			if(fgets(&line[(int)blen],CF_MAXLINE-(int)blen,fp)==NULL) continue;
#else
			/* Read new line */

			if( varincount >= 0 ){

#ifdef DEBUG_FORM
				fprintf(stderr,"\nvarincount=%d, linesavinc[%d]=%s\n",varincount,varincount,linesavinc[varincount]);
#endif
				strcpy( line, linesavinc[varincount] );
				varincount--;

			} else if( fgets(line,CF_MAXLINE,fp) == NULL ) continue;

			if( ( section != NULL || inc_section != NULL )
				&& *line == '[' && strchr(line,']') != NULL ){
				fclose(fp);
				if( inc_section != NULL ){
					inc_section = NULL;
				} else {
					section = NULL;
					if(noincsect) break;
				}
				fp = fpsav[inccount];
				strcpy( line, linesav[inccount] );
				inccount--;
			}
#endif
		} else if( feof(fp) && (inccount < 0) ) break; /* end of main template */

		else { /* feof and actually including file */

			if( !noincsect ) section = NULL;
			fclose(fp);
			fp = fpsav[inccount];
			strcpy(line,linesav[inccount]);
			inccount--;
		}

		search_var_poi = start_output_poi = line;

		/* Loop over variables */

		while( (var_esc_poi = strchr( search_var_poi, vd[0] )) != NULL ){

			var_poi = var_esc_poi;
			var_poi++;

			if( *var_poi == vd[1] ){

				if((var_end_poi=strchr(var_poi,vd[2]))!=NULL){

					/* valid variable encountered */
					*var_end_poi='\0';
					var_end_poi++;
					var_poi++;
					*var_esc_poi='\0';
					fputs(start_output_poi,fout);
					fflush(fout);

					/* File Include Variable */

					if( ( inccount < CF_MAXINC ) && ( strncmp(var_poi,"FILE:",5) == 0 ) ){

						inccount++;
						var_poi += 5;

						/* save old file pointer */
						fpsav[inccount]=fp;
#ifdef DEBUG_FORM
						fprintf(stderr,"include from file <%s>\n",var_poi);
#endif
						/* Section Specification */
						if( (tmp1=strchr(var_poi,'#')) != NULL ){
							*tmp1 = '\0';
							tmp1++;
							if( inc_section != NULL ) break;
							strcpy( qbuf, tmp1 );
							inc_section = qbuf;
#ifdef DEBUG_FORM
							fprintf(stderr,"required section <%s> in file <%s>\n",section,var_poi);
#endif
						}

						/* variable file include - lookup! */
						if( *var_poi == ':' ){
							var_poi++;
							found=FALSE;
							for(i=0;i<MAXCONF;i++){
								if( !strcmp(_conf[i]->name,var_poi) ){
									if( _conf[i]->inhalt != NULL ){
										var_poi = _conf[i]->inhalt;
									}
									found = TRUE;
									break;
								}
								else if( _conf[i]->flag & CF_LAST ) break;
							}
							if( !found ) var_poi = NULL;
						}

						/* save dependencies list in configuration */

						sprintf( incbuf, "INCLUDE_FILE_%2.2d", ++incfiles );
						cfputstr( incbuf, var_poi );
						if( mode&2 ) cfnosave( incbuf, CF_FLAG_OFF );

						/* file open */

						if((fp=fopen(var_poi,"r"))==NULL){

							fprintf(stderr,"file <%s> open error!\n",var_poi);

							fp=fpsav[inccount];
							inccount--;

						} else { /* include file */

							/*save rest of line */
							strcpy( linesav[inccount], var_end_poi );
							incinit = TRUE;

							if( noincsect<2 && inc_section != NULL ){

								/* forward to section start */
								while( !feof(fp) ){
									if( fgets(line,CF_MAXLINE,fp) == NULL ) break;
									/* section start in line ? */
									if( *line=='[' && (tmp1=strchr(line,']')) != NULL ){
										*tmp1 = '\0';
#ifdef DEBUG_FORM
										fprintf(stderr,"found included section <%s>\n",&line[1]);
#endif
										/* section matches required one ? */
										if( ( strlen(&line[1]) == strlen(inc_section) )
											&& !strcmp(&line[1],inc_section) ) break;
									}
								} /* end while (!feof(fp)) */
							} /* end if (inc_section) */

						} /* end else - file opened */

					/* end if file include */

					} else { /* search entry for variable */

						found = FALSE;

						for(i=0;i<MAXCONF;i++){
							if( !strcmp(_conf[i]->name,var_poi) ){
								if( _conf[i]->inhalt != NULL ){
									if( varincount < CF_MAXINC ){
										strcpy( linesavinc[++varincount], var_end_poi );
										strcpy( line, _conf[i]->inhalt );
										var_end_poi = line;
									} else {
										fputs(_conf[i]->inhalt,fout);
									}
									if( mode&2 ) cfnosave( _conf[i]->name, CF_FLAG_OFF );
									fflush(fout);
								}
								found = TRUE;
								break;
							} else if(_conf[i]->flag&CF_LAST) break;
						}

						if( !found && (mode&1) ){

							if( IsATerminal(stdin) ){
								fprintf( stdout, "Enter %s: ", var_poi );
								fflush(stdout);
							}
							/* Console or stream input */
							fgets( qbuf, CF_MAXQLINE, stdin );

							RemoveCR( qbuf );
							fputs( qbuf, fout );
							fflush(fout);
							cfputstr( var_poi, qbuf );
							if( mode&2 ) cfnosave( var_poi, CF_FLAG_OFF );
						}
					}
				} /* end if variable encountered */

#ifdef BIN_FORM1
				else if (strchr(var_end_poi,'\n')==NULL){
					strcpy(buf,var_esc_poi);
					*var_esc_poi = '\0';
				}
#else
				else break;
#endif
			} /* end if(*var_poi==vd[1]) */

			else {
#ifdef BIN_FORM1
				if(*var_poi=='\0'){
					ungetc((int)(*var_esc_poi),fp);
					*var_esc_poi='\0';
				}
#endif
				/* continue searching if this was'nt a variable */
				search_var_poi = var_poi;
				continue;
			}

			if( incinit == TRUE ) break;
			search_var_poi = start_output_poi = var_end_poi;

		} /* end while variable include loop */

		if( incinit == TRUE ) continue;
		fputs( start_output_poi, fout );
		fflush(fout);

	} /* end while(TRUE) - infile parsing */

	if( fp != stdin) fclose(fp);
	if( fout != stdout) fclose(fout);

	return CFE_OK;
}


/***************************************************************************//**
 *
 * @ingroup cflib_core
 *
 * Setup Configuration Database
 *
 * Check errors and output usage message, if required
 *
 * @param setting   pointer to initializer
 *
 * @param ac        argument count from commandline or compatible
 *
 * @param av        argument string array from comandline or compatible
 *
 * @param help      Help/Usage String
 *                  - Name of a @ref parameter_type_flag entry to cause a usage
 *                    message, if flag is set
 *                  - Omit, if @c NULL
 *
 * @param mode      How to handle cfinit() return:
 *                  @arg @ref CFS_NOT   - @copybrief CFS_NOT
 *                  @arg @ref CFS_ALL   - @copybrief CFS_ALL
 *                  @arg @ref CFS_NEG   - @copybrief CFS_NEG
 *                  @arg @ref CFS_USG   - @copybrief CFS_USG
 *                  @arg @ref CFS_DEBUG - @copybrief CFS_DEBUG
 *
 * @return
 *          @arg @c  0 : configuration database has been initialized
 *                       successfully, no help or usage message required,
 *                       no fatal errors
 *          @arg @c  1 : no fatal errors occurred, usage message displayed
 *          @arg @c -1 : fatal error(s) occurred
 *
 ******************************************************************************/

int cfstart( CONFIG *setting, int ac, char **av, char *help, int mode )
{
	int cfret, ecode;
	char line[CF_MAXERRSTR+1],usg=FALSE, err=FALSE;

	cfret = cfinit(setting,ac,av);

	switch(mode){

#ifndef _CF_NODEBUGGING
		case CFS_DEBUG:
			cfdump(stderr);
#endif
		case CFS_USG:
			usg=TRUE;
		case CFS_NEG:
			if(cfret<0) err=TRUE;
			break;
		case CFS_ALL:
			if(cfret!=0) err=TRUE;
			break;
		case CFS_NOT:
		default:
			break;
	}

	if( err ){

		while((ecode=cfgeterr(line,0))){
			if(mode==CFS_NEG && ecode>0) continue;
			fprintf(stderr,"error %d: %s",ecode,line);
		}

	} else cfclearerr();

	if (cfgetflag(help) || (err && usg)){
		fputs(cfgetusg(),stderr);
		return ((cfret<0)?-1:1);
	}

	return ( (cfret < 0) ? -1 : 0 );
}

/***************************************************************************//**
 *
 * @ingroup advanced_features CFLIB Advanced Features
 *
 * Expand @c ~ or @c ~user in parameter content
 *
 * This function is used by default when reading @ref config_files and on
 * initialization of parameters with the Special Option Flag @ref CF_EXPHOME set.
 *
 * - The environment is checked for the variables @c LOGNAME or @c USER,
 *   if no user name is given ("~/....")
 * - The @c passwd file is searched for the users home directory, if possible
 * - Otherwise the environment variable @c HOME is checked
 * - If all that fails, @c ~ will be omitted, @c ~user will expand to "./user"
 *
 * @param   name    Entry's name
 *
 * @return
 * 			@arg @c NULL : an error occurred:
 *                       - @copybrief CFE_NCA
 *                       - @copybrief CFE_ECP
 *                       - @copybrief CFE_MCF
 *
 *          @arg @c !=NULL : String pointer to original or expanded filename
 *
 ******************************************************************************/

char *cfhomexp( char *name )
{
	register int i;
	int j=1;
	unsigned char other=FALSE, envhome=FALSE;
	char *tmp1, *tmp2=NULL, line[CF_MAXLINE+1], *user=NULL;

#ifdef _HAS_PWD
	struct passwd *pw;
#endif

	if(_conf[0]==NULL) return NULL;

	for(i=0;i<MAXCONF;i++){

		if (!strcmp(_conf[i]->name,name)){

			if(_conf[i]->inhalt == NULL) return NULL;
			if (_conf[i]->inhalt[0]!='~') return _conf[i]->inhalt;

			/* is it logged on user or another? */

			if( !( _conf[i]->inhalt[1] == '/' || _conf[i]->inhalt[1]=='\\' || _conf[i]->inhalt[1]=='\0' ) ){

				/* is other, get other user's name */

				other=TRUE;
				strcpy(line,&_conf[i]->inhalt[1]);
				tmp1=line;
				while(*tmp1!='\0'){
					if (*tmp1=='/' || *tmp1=='\\') break;
					tmp1++;
					j++;
				}
				*tmp1='\0';

				user=line;
			}

			else{

				/* get logged on users name from environment */

				user=getenv("LOGNAME");
				if(user==NULL || *user=='\0') user=getenv("USER");
			}

			/* check the password file for users home directory */

			tmp1=NULL;

#ifdef _HAS_PWD
			if(!(user==NULL || *user=='\0')){
				if((pw=getpwnam(user))!=NULL) tmp1=pw->pw_dir;
			}
#endif
			/* if this was not successfull, check the HOME environment variable */

			if(tmp1==NULL || *tmp1=='\0'){
				tmp1=getenv("HOME");
				envhome=TRUE;
			}

			/* a home was found */

			if(tmp1!=NULL && *tmp1!='\0'){
				strcpy(line,tmp1);

				/* if other user, assume his/her home directory on the same level */

				if(envhome && other){
					tmp2=tmp1=line;
					while(*tmp1!='\0'){
						if (*tmp1=='/' || *tmp1=='\\') tmp2=tmp1;
						tmp1++;
					}
					if(tmp2!=line) *++tmp2='\0';
					j=1;

				}

			} else { /* no home found: assume HOME = working dir. */

				/* if logged on user: just omit ~/ and return */
				if(!other) return (_conf[i]->inhalt+=((_conf[i]->inhalt[1]=='\0')?1:2));
				j=1;
				strcpy(line,"./");
			}

			/* any kind of home is already in string line, now concatenate the rest */

			strcat(line,&_conf[i]->inhalt[j]);

			/* update the CF DB entry */

			if(_conf[i]->flag&CF_MALLOC) free(_conf[i]->inhalt);
			if( (_conf[i]->inhalt=malloc(strlen(line)+1))==NULL) return NULL;
			else SetFlag(_conf[i]->flag,CF_MALLOC);
			strcpy(_conf[i]->inhalt,line);
			break;

		} /* end if (!strcmp(_conf[i]->name,name)) */

		else if(_conf[i]->flag&CF_LAST) return NULL;

	} /* end for-loop */

	return _conf[i]->inhalt;
}

/***************************************************************************//**
 *
 * @ingroup utilities
 *
 * String Utility Function
 *
 * Strip Carriage Return at end of string (after fgets) by introducing
 * zero byte at CR position.
 * Original source was "STELM" by Kees and Lemmens.
 *
 * @author Kees and Lemmens
 *
 * @param   ptr	pointer to beginning of string
 *
 ******************************************************************************/

void RemoveCR( char *ptr )
{
	char *tmp;
	if( (tmp = strpbrk( ptr, "\n\r" )) != NULL ) *tmp = '\0';
}

/***************************************************************************//**
 *
 * @ingroup utilities String Utility Function
 *
 * Strip whitespaces at end of string by introducing zero byte after
 * last non-whitespace character
 *
 * @param   ptr	Pointer to beginning of string
 *
 ******************************************************************************/

void RemoveTrailSpace( char *ptr )
{
	char *tmp;
	tmp = ptr;
	while(*tmp) tmp++;
	tmp--;
	while( *tmp == ' ' || *tmp == '\t' ) tmp--;
	tmp++;
	*tmp = '\0';
}

/***************************************************************************//**
 *
 * @ingroup utilities String Utility Function
 *
 * Set pointer to next non-whitespace-character in string.
 * Original source was "STELM" by Kees and Lemmens.
 *
 * @author Kees and Lemmens
 *
 * @param   ptr	Pointer to beginning of string
 * @return	    Pointer to next non-whitespace-character in string
 *
 ******************************************************************************/

char *EatWhiteSpace( char *ptr )
{
	while( *ptr == ' ' || *ptr == '\t' ) ptr++;
	return ptr;
}

/***************************************************************************//**
 *
 * @ingroup utilities
 *
 * Test whether stream is a terminal
 *
 * @param fp File/Stream Pointer
 * @return @c TRUE or @c FALSE
 *
 * @bug ANSI C doesn't have function isatty(), we always return TRUE
 *
 ******************************************************************************/

int IsATerminal( FILE *fp )
{
#ifdef __MINT__
	struct stat info;
	if( fstat(fileno(fp),&info) != 0 || S_ISCHR( info.st_mode ) ) return FALSE;
#endif
#ifdef _HAS_ISATTY
	if( !isatty( fileno(stdin) ) ) return FALSE;
#endif
	return TRUE;
}

/******************************************************************************/
