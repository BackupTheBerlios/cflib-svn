/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @file cf.h
 *
 * C Header File for CFLIB Flexible Configuration Library
 *
 * Public Functions and Definitions
 *
 * @note      Include this file in the source code and link with the library
 *            executable, usually referring to @e libcf.a by calling
 *            @c "(g)cc -lcf ..."
 *
 * @version   SVN: \$Id: cf.h 127 2009-01-28 16:50:14Z stefan $
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,1997,1998,2006,2007,2008,2009 Stefan Habermehl
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
 * 2009-01-24 [sh] Final Revision PL 20
 *
 ******************************************************************************/

#ifndef __CF_H__

/**
 * @defgroup cflib_core CFLIB Core Features
 *
 * Basic CFLIB Setup
 *
 * - General Defines
 * - Parsing Levels
 * - Configuration Entries (aka Parameters)
 * - Configuration Database: Array of Configuration Entries
 * - Initialization of the Configuration Database
 * - Resetting the Configuration Database
 * @{
 */

#define Patchlevel "20"          /**< CFLIB Identification */

/*
 * UTILITIES
 */

/* Dimensions of Arrays and Initializations of Variables */

#define MAXCONF         4096     /**< Maximum number of entries in configuration database */
#define CF_MAXERRSTR     512     /**< Maximum string length for error message */
#define CF_MAXLINE     20480     /**< Maximum string length for setfile and form parsing */
#define CF_MAXQLINE      512     /**< Maximum string length for query */
#define CF_MAXUSAGE     1024     /**< Maximum string length for usage string */
#define CF_USG_DEFCOLS    80     /**< Default terminal width for usage string */
#define CF_MAXTIMEBUF    256     /**< Buffer size for time and day */

/* Boolean, pointer and flag macros */

#ifndef TRUE
#define TRUE  1                  /**< TRUE, if not defined */
#define FALSE 0                  /**< FALSE, if not defined */
#endif

#ifndef NULL
#define NULL (void *)(0L)        /**< NULL, if not defined */
#endif

#define CF_FLAG_ON      "\1"     /**< Flag is set */
#define CF_FLAG_OFF     ""       /**< Flag is not set */
#define CF_NO_OPTION    ' '      /**< Option is not set */
#define TABLEN          8        /**< TAB length */

/* Parsing Levels */
#define CFP_PUT         0	/**< Function Call or Automatic Initialization */
#define CFP_ARG         1	/**< Commandline */
#define CFP_ENV         2   /**< Environment */
#define CFP_PRIV        3   /**< Private Configuration File */
#define CFP_SYS         4   /**< System Configuration File */
#define CFP_DEF         5   /**< Built-in Default */
#define CFP_QRY         6   /**< Standard Input Channel */
#define CFP_RESERVED    7   /**< Reserved for Subprojects */

/**
 * @}
 * @addtogroup report_generation
 * @{
 */
#define CF_DEF_VARDELIM "$()"    /**< Default variable delimiter for cfform() */
#define CF_MAXINC          8	 /**< Maximum number of nested includes for cfform() */

/**
 * @}
 * @addtogroup utilities
 * @{
 */
#define CF_BACKBUFLEN 102400     /**< File copy buffer for BackupFile() */

/**
 * @}
 * @defgroup special_options_mask Special Options Mask
 *
 * The @b "Special Options Mask" is a Bitmask of type @ref CFFLAGTYP in
 * Configuration Entry Structure Member CONFIG::flag containing Type,
 * Instruction and Information Flags for a Parameter
 *
 * - Special CFLIB properties: @ref cflib_parameters
 * - Source/Origin: @ref parsing_levels
 * - Initialization options: @ref initialization_process
 * - Type/Interpretation Flags: @ref parameter_datatypes
 * - @ref processing_instructions
 * - @ref handling_instructions
 * - Information/Status markers:
 *   @li @ref CF_LAST must appear in the @ref config_initializer
 *   @li @ref CF_RESID marks one of the @ref residuals
 *   @li @ref CF_MALLOC and @ref CF_FORCED are for library internal use
 *
 * @{
 */

/**
 * Special Options Mask Type
 */
typedef unsigned long CFFLAGTYP;

/* CFLIB Function flags */
#define CF_LAST         0x0001  /**< Last entry in settings array */
#define CF_PATH         0x0002  /**< Search Path (for FindFile feature) */
#define CF_SETFILE      0x0004  /**< Entry is Private Configuration File */
#define CF_SYS_SETFILE  0x0008  /**< Entry is System Configuration File */

#define CF_SECTION      0x0010  /**< Section in Configuration File */
#define CF_PRGNAME      0x0020  /**< Running Program's Name from commandline */
#define CF_TIME         0x0040  /**< Time string */
#define CF_DATE         0x0080  /**< Date string */

/* Source flags - "Origin Window" */
#define CF_SET_PUT      0x0100  /**< Source: Function Call */
#define CF_SET_ARG      0x0200  /**< Source: Commandline Argument */
#define CF_SET_ENV      0x0400  /**< Source: Environment Variable */
#define CF_SET_PRIV     0x0800  /**< Source: Private Configuration File */
#define CF_SET_SYS      0x1000  /**< Source: System Configuration File */
#define CF_SET_DEF      0x2000  /**< Source: Built-in Default */
#define CF_SET_QRY      0x4000  /**< Source: Interactive Terminal Input */
/*      CF_SET_CGI      0x8000       Reserved (other development branch) */

/* Parsing Options */
#define CF_NO_OPT_ARG   0x10000 /**< Commandline argument not followig an option */
#define CF_CONCAT       0x20000 /**< Argument is concatenated to option */
#define CF_IGN_ENV      0x40000 /**< Do not check environment for variable */
#define CF_QUERY        0x80000 /**< Ask the user for unresolved item after
                                     configuration parsing */
/* (see also source flags!) */

/* Type/Interpretation Flags */
#define CF_STR          0x100000   /**< Entry is String */
#define CF_INT          0x200000   /**< Entry is Integer */
#define CF_FLAG         0x400000   /**< Entry is Flag */
#define CF_REAL         0x800000   /**< Entry is Float */

/* Special Processing Options */
#define CF_FINDFILE     0x1000000  /**< Entry is filename to be searched for in the path */
#define CF_MUST         0x2000000  /**< Entry may not be empty (@c NULL or "") */
#define CF_RESID        0x4000000  /**< Residual/additional entry from commandline/setfile/cfput */
#define CF_USAGE        0x8000000  /**< Usage Message format string */
#define CF_EXPHOME      0x10000000 /**< Expand Home Directory */

/* Information/Status Flags */
#define CF_MALLOC       0x20000000 /**< Space for entry's content was mallocated */
#define CF_FORCED       0x40000000 /**< Setting has been forced (already) */
#define CF_NOSAVE       0x80000000 /**< Don't include in savefile / mark entry */

/* Special Flag Combinations */
#define CF_SRC      (CF_INT|CF_FLAG)    /**< Type for source/origin inquiry */
#define CF_FLGINQ   (CF_STR|CF_FLAG)    /**< Type for options mask inquiry */
#define CF_TD       (CF_DATE|CF_TIME)   /**< Date or Time entry */


/**
 * @}
 * @defgroup errors Error Handling
 *
 * Error Codes, Functions and Structures
 *
 * - Error Codes and their mnemonic descriptions
 *
 * @{
 */
#define CFE_INIT           0   /**< INITialize error input */
#define CFE_OK             0   /**< No error / everything OKay */
#define CFE_NEP            1   /**< New Entry successfully Put into DB */
#define CFE_EXIT           1   /**< Finish error input */
#define CFE_ORA           20   /**< Option Requires an Argument */
#define CFE_UKO	          25   /**< UnKnown Option */
#define CFE_FNF           30   /**< File Not Found, read access error */
#define CFE_NSE           40   /**< No Section specifier End found, missing "]" */
#define CFE_NSC           50   /**< No private Setfile Configured */
#define CFE_WAE           60   /**< Write Access Error */
#define CFE_IFP           61   /**< Invalid Filename entry for Private setfile */
#define CFE_EWN           70   /**< Entry Without Name */
#define CFE_ICF           80   /**< Invalid Combination of Flags */
#define CFE_EWC           90   /**< Entry Without Content */
#define CFE_UOS          100   /**< Unlikely Option Specifier */
#define CFE_IFC          110   /**< Invalid Flag Combination */
#define CFE_NLE          120   /**< No Last Entry flag found */
#define CFE_TIN          130   /**< Error reTurn from stdIN query */

#define CFE_EFE          140   /**< Empty string in content for Filename Entry */
#define CFE_USG          200   /**< Entries missing: USaGge advice */
#define CFE_URI          210   /**< UnResolved Item (CF_MUST was set!) */
/*
 *                       300-320    Reserved (other development branches)
 *                       400-440, -450
 */
#define CFE_FBF		-500	   /**< File Backup Failed */
#define CFE_BMF		-510	   /**< Backup: Memory allocation Failed */
#define CFE_BOF		 520	   /**< Backup: Open source file Failed */
#define CFE_BBF		 530	   /**< Backup: open target Backup file Failed */
#define CFE_BRF		 540	   /**< Backup: Rename Failed */
#define CFE_BWF		 550	   /**< Backup: Write Failed */

/* Fatal or severe errors */

#define CFE_NCA          -10   /**< No Configuration database Available */
#define CFE_NEA          -20   /**< No Entry with that name Available */
#define CFE_NSS          -30   /**< No Source/origin is Set */
#define CFE_ECP          -40   /**< Entry's Content is a NULL Pointer */

#define CFE_MEF         -100   /**< Memory allocation in Error routine Failed */
#define CFE_MCF         -200   /**< Memory allocation for Configuration Failed */
#define CFE_INF        -9999   /**< Integer iNquiry Failed (?!) */
#define CFE_RNF     -999.999   /**< Real/float iNquiry Failed (?!) */

/**
 * @}
 * @addtogroup cflib_core
 * @{
 *//*
 * Error Response Modes for cfstart()
 */
#define CFS_NOT		  0        /**< Start Mode: No action on error */
#define CFS_ALL		  1        /**< Start Mode: All error messages */
#define CFS_NEG		  2        /**< Start Mode: Only severe errors */
#define CFS_USG		  3        /**< Start Mode: Usage message if error was
                                    negative, error output like @ref CFS_NEG */
#define CFS_DEBUG	  4        /**< Start Mode: Output like @ref CFS_USG plus
                                    raw dump of configuration */
/**
 * @}
 * @defgroup advanced_features  Advanced Features
 *
 * Debugging and Utility Functions
 *
 * @{
 *
 * - Modes for cfdump()
 */
#ifndef _CF_NODEBUGGING
#define CFD_CFDUMP    0            /**< Dump Mask Minimal */
#define CFD_LIBHEAD   1            /**< Dump option CFLIB header */
#define CFD_COLHEAD   2            /**< Dump option Column headers */
#define CFD_SRCFLAGS  4            /**< Dump option Source flag description */
#define CFD_DEFAULT   CFD_COLHEAD|CFD_SRCFLAGS /**< Dump Mask Default */
#endif

/**
 * @}
 * @ingroup cflib_core
 *
 * CFLIB Configuration Database Entry
 */
typedef struct _cf{
    char *name;				/**< @ref parameter_name */
    char *inhalt;           /**< Parameter Content, see @ref parameter_default */
    char option;            /**< @ref parameter_option */
    CFFLAGTYP flag;         /**< @ref special_options_mask */
} CONFIG;

/* Function Macros */

/**
 * @defgroup retrieval Information Retrieval
 *
 * These functions and macros read entries from an initialized CFLIB database.
 * The exact name of the required parameter must be given as argument, where
 * indicated.
 *
 * @{
 */
#define cfget(a)       cfgetent(a,0)
#define cfgetstr(a)    ((char *)cfgetent(a,CF_STR))
#define cfgetnum(a)    (*(int *)cfgetent(a,CF_INT))
#define cfgetreal(a)   (*(float *)cfgetent(a,CF_REAL))
#define cfgetflag(a)   (*(int *)cfgetent(a,CF_FLAG))
#define cfflaginq(a,b) (*(int *)cfgetent(a,CF_FLGINQ|(31&b)))
#define cfgetsrc(a)    (*(int *)cfgetent(a,CF_SRC))
#define cfgetres()     ((char *)cfgetent("",CF_RESID))
#define cfgetcpr()     "CFLIB (c) 1994-2009 Stefan Habermehl"

/**
 * @}
 * @defgroup setting_saving Setting and Saving the Configuration
 *
 * Set/Update Parameter Values or Save a Configuration File
 */
#define cfput(a,b)     cfputstr(a,(char *)b)

/**
 * @defgroup utilities General Utilities
 * @{
 */
#define DelFlag(a,b)   a&=(~b)  /**< Delete bits of Mask @c b from Mask @c a */
#define SetFlag(a,b)   a|=b     /**< Set bits of Mask @c b in Mask @c a */
/** @} */

/* Function Prototypes */

int    cfinit(CONFIG *set,int argc,char **argv);
void   cfexit(void);
int    cfform(char *infile, char *outfile, char *vardelim, int mode);
void  *cfgetent(char *name, CFFLAGTYP typ);
char  *cfhomexp(char *name);
int    cfgetvers(void);
char  *cfgetsubvers(void);
int    cfnosave(char *name, const char *onoff);
int    cfputstr(char *name, char *content);
int    cfputtime(CFFLAGTYP td);
char  *cfgetusg(void);
int    cfgeterr(char *string, size_t len);
int    cfputerr( int ecode, char *string, ... );
void   cfclearerr(void);
int    cfreverr(void);
int    cfstart(CONFIG *set, int ac, char **av, char *help, int mode);

#ifndef _CF_NODEBUGGING
int    cfdinichk(CONFIG *set);
int    cfdump(FILE *fout);
#endif
#ifndef _CF_NOSAVING
int    cfsave(char *fname, const char *savemode);
int    BackupFile(const char *file, char *modus);
#endif

void   RemoveCR(char *string);
void   RemoveTrailSpace(char *string);
char  *EatWhiteSpace(char *string);
int    IsATerminal( FILE *fp );
#ifndef _CF_NOFINDFILE
char *FindFile( const char *fname, const char *fpath, const char *const *fext );
#endif

/** Marker: cf.h has been included */
#define __CF_H__
#endif
