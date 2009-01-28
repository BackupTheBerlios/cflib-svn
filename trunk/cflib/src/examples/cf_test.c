/******************************************************************************/
/*                                                                            */
/* cf_test.c - Test Basic CFLIB functionality                                 */
/*                                                                            */
/* CFLIB demo                                                                 */
/* (c) 1995-2008 S. Habermehl                                                 */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* #include <cf.h> */
#include "../../include/cf.h"

int main(int argc, char **argv)
{
	char  line[CF_MAXERRSTR + 1];
	int   ret, verb, flag, ecode, bufs;
	char *edi, *outfile, *sys, *priv, *prog, *name, *bufn;

	/**
	 * Definition and Initialization Values of Basic Configuration Setup
	 */
	CONFIG setting[] =
	{
	 /* Name              Default      Option  Typ             Command Line    Sources       Special settings */
	  { "Buffer_Size",    "512",          '1', CF_INT        | CF_NO_OPT_ARG | CF_SET_ARG, },
	  { "Buffer_Name",    "mypuffer",     '2',                 CF_NO_OPT_ARG | CF_SET_ARG, },
	  { "Verbosity",      "1",            'v',                 CF_CONCAT     | CF_SET_ARG, },
	  { "My_Flag",        CF_FLAG_ON,     'f', CF_FLAG,                                    },
	  { "Search_Path",    "p:;p:\\batch", 'p', CF_PATH       | CF_CONCAT,                  },
	  { "Home_Dir_Test",  NULL,           'n',                 CF_CONCAT     | CF_QUERY    | CF_EXPHOME, },
	  { "Editor_Path",    NULL,           'e', CF_FINDFILE   |                 CF_MUST,    },
	  { "Priv_Conf_File", "cf_test.cnf",  's', CF_SETFILE    |                 CF_SET_ARG, },
	  { "TIME",           NULL,           't', CF_TIME       |                 CF_SET_PUT, },
	  { "DATE",           NULL,           'd', CF_DATE       |                 CF_SET_PUT, },
	  { "Sys_Conf_File",  "/etc/test.cnf",'y', CF_SYS_SETFILE,                             },
	  { "Section",        "section",      'c', CF_SECTION    | CF_CONCAT,                  },
	  { "Outfile",        "contest.out",  'o',                                 CF_SET_DEF  | CF_LAST, },
	};

	printf( "\nCFLIB Test and Demonstration Tool %s\n\n", "starting ..." );

#ifdef DEBUG
	/**
	 * Check Configuration
	 */
	ret = cfdinichk( setting );
#else

	/**
	 * Init Configuration
	 */
	ret = cfinit(setting, argc, argv);

	printf("\ncfinit returned %d\n\n", ret); fflush(stdout);

	/**
	 * Dump Configuration
	 */
	cfdump(stderr);  fflush(stderr);

#endif

	if( ret ){
		/**
		 * Read and output errors
		 */
		while( (ecode = cfgeterr(line, 0)) )
			printf("error %d: %s", ecode, line);

		puts( cfgetusg() ); fflush(stdout);
	}

	/**
	 * CFLIB Information
	 */
	fprintf(stderr, "CFLIB Patchlevel: %d\n", cfgetvers()    );
	fprintf(stderr, "CFLIB Subversion: %s\n", cfgetsubvers() );
	fprintf(stderr, "%s\n",                   cfgetcpr()     );
	fflush(stderr);

	/**
	 * Get Configuration Variables
	 */
	bufs    = *(int *) cfget(     "Buffer_Size"   );
	bufn    = (char *) cfget(     "Buffer_Name"   );
	verb    =          cfgetnum(  "Verbosity"     );
	flag    =          cfgetflag( "My_Flag"       );
	outfile =          cfgetstr(  "Outfile"       );
	edi     =          cfgetstr(  "Editor"        );
	name    =          cfgetstr(  "Home_Dir_Test" );
	sys     =          cfgetstr(  "Sys_Conf_File" );
	priv    =          cfgetstr(  "Priv_Conf_File");
	prog    =          cfgetstr(  "CF_PRGNAME"    );

	/**
	 * Put Configuration Variables
	 */
	ret     = cfputstr( "Outfile", "set_by_cfputstr.txt");
	outfile = cfgetstr( "Outfile" );

	/* Output Automatic Variables */
	printf("CFLIB Test on %s at %s:\n\n", cfgetstr("DATE"), cfgetstr("TIME") );

	/**
	 * Get Source of Configuration Variable Values
	 */
	printf(" program name=%s from %d\n",    prog, cfgetsrc("CF_PRGNAME")    );
	printf(" system setfile=%s from %d\n",   sys, cfgetsrc("Sys_Conf_File") );
	printf(" private setfile=%s from %d\n", priv, cfgetsrc("Priv_Conf_File"));
	printf(" bufsize=%d from %d\n",         bufs, cfgetsrc("Buffer_Size")   );
	printf(" bufname=%s from %d\n",         bufn, cfgetsrc("Buffer_Name")   );
	printf(" editor=%s from %d\n",           edi, cfgetsrc("Editor")        );
	printf(" outfile=%s from %d\n",      outfile, cfgetsrc("Outfile")       );
	printf(" verbosity=%d from %d\n",       verb, cfgetsrc("Verbosity")     );
	printf(" homedir expanded=%s from %d\n",name, cfgetsrc("Home_Dir_Test") );
	printf(" flag=%s from %d\n", ((flag == TRUE) ? "TRUE" : "FALSE"),
	                                              cfgetsrc("My_Flag")       );
	fflush(stdout);

	/**
	 * Fill a template with variables
	 */
	ret = cfform( "in.frm", "out.frm", NULL, 0 );

	printf( "Template filtering: cfform('in.frm','out.frm',NULL,0) returned %d\n",
			ret );
	fflush(stdout);

	/**
	 * Save configuration values to (new) configuration file (private style)
	 */
	ret = cfsave( "save.cnf", "w" );

	printf( "Configuration saving: cfsave('save.cnf','w') returned %d\n",
			ret );

	if( flag ){

		/**
		 * Save configuration values (back) to private configuration file
		 */
		ret = cfsave( NULL, "w" );

		printf( "Configuration saving in '%s': cfsave(NULL,'w') returned %d\n",
				priv, ret);
	}

	/**
	 * Exit Configuration
	 */
	cfexit();

	return 0;
}

