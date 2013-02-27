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
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,1997,1998,2006,2007,2008,2009,2013 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU Lesser General Public License version 3.0 (LGPLv3)
 * @package   CFLIB Flexible Configuration Library
 *
 *//****************************************************************************
 *
 * This file is part of the CFLIB library.
 *
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
 * 2009-01-24 [sh] Final Revision PL 20
 * 2009-02-05 [sh] Revised documentation markers for better user program
 *                 integeration
 * 2012-09-21 [sh] Added CFD_FLAGS dump option
 *
 ***************************************************************************//**
 *
 * @weakgroup cflib Flexible Configuration Library
 * @{
 *
 * CFLIB is a small, simple, flexible and portable ANSI C Library to be used as
 * configuration interface for user programs.
 *
 * CFLIB builds and maintains a compact database structure consisting of a list
 * of parameters with their name, content and some additional information about
 * each parameter.
 *
 * CFLIB targets the basic needs of technical, scientific or other programmers
 * who want to spend minimal time on coding input, output, variable parsing,
 * report generation and the like but still have a simple to use, reliable,
 * flexible and portable configuration interface for their programs.
 *
 * @par Main Features
 *
 * - Commandline, environment and terminal input parsing
 * - Configuration files
 * - File search
 * - Template driven report generation
 * - Automatic time and date update
 *
 * @author   Stefan Habermehl
 * @license  http://www.gnu.org/licenses GNU General Public License v3 or later
 *
 * @par Project Homepage:
 *
 * Detailed information, source code and maybe updates are available from the
 * library homepage:
 *
 * - http://cflib.berlios.de
 *
 * @par Local References:
 *
 * - Include file: @e cf.h
 * - Library file: @e libcf.a
 * - Documentation: @ref cflib
 *
 * @weakgroup cflib_core Core Features
 *
 * Basic CFLIB Setup
 *
 * - General Defines
 * - Parsing Levels
 * - Configuration Entries (aka Parameters)
 * - Configuration Database: Array of Configuration Entries
 * - Initialization of the Configuration Database
 * - Resetting the Configuration Database
 *
 * @{
 ******************************************************************************/

#ifndef __CF_H__

/*
 * DEFINES
 */

#define Patchlevel "21"      /**< CFLIB Identification */

/* Dimensions of Arrays and Initializations of Variables */
#define MAXCONF         4096 /**< Maximum number of entries in configuration database */
#define CF_MAXERRSTR     512 /**< Maximum string length for error message */
#define CF_MAXLINE     20480 /**< Maximum string length for setfile and form parsing */
#define CF_MAXQLINE      512 /**< Maximum string length for query */
#define CF_MAXUSAGE     1024 /**< Maximum string length for usage string */
#define CF_USG_DEFCOLS    80 /**< Default terminal width for usage string */
#define CF_MAXTIMEBUF    256 /**< Buffer size for time and day */

/* Boolean, pointer and flag macros */

#ifndef TRUE
#define TRUE  1             /**< TRUE, if not defined */
#define FALSE 0             /**< FALSE, if not defined */
#endif

#ifndef NULL
#define NULL (void *)(0L)   /**< NULL, if not defined */
#endif

#define CF_FLAG_ON     "\1" /**< Flag is set */
#define CF_FLAG_OFF     ""  /**< Flag is not set */
#define CF_NO_OPTION    ' ' /**< Option is not set */
#define TABLEN          8   /**< TAB length */

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
 * @weakgroup report_generation Report Generation
 * @{
 */
#define CF_DEF_VARDELIM "$()" /**< Default variable delimiter for cfform() */
#define CF_MAXINC          8  /**< Maximum number of nested includes for cfform() */

/**
 * @}
 * @weakgroup utilities General Utilities
 * @{
 */
#define CF_BACKBUFLEN 102400 /**< File copy buffer for BackupFile() */

/**
 * @}
 * @weakgroup special_options_mask Special Options Mask
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
#define CF_NO_OPT_ARG   0x10000 /**< Commandline argument not following an option */
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
#define CF_DOUBLE   (CF_REAL|CF_FLAG)   /**< Type for double inquiry */
#define CF_TD       (CF_DATE|CF_TIME)   /**< Date or Time entry */

/**
 * @}
 * @weakgroup errors Error Handling
 *
 * Error Codes, Functions and Structures
 *
 * - Error Codes and their mnemonic descriptions
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
 * @weakgroup cflib_core
 * @{
 */
/**
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
 * @weakgroup advanced_features Advanced Features
 *
 * Debugging and Utility Functions
 * @{
 *
 * Modes for cfdump()
 */
#ifndef _CF_NODEBUGGING
#define CFD_CFDUMP    0        /**< Dump Mask Minimal */
#define CFD_LIBHEAD   1        /**< Dump option CFLIB header */
#define CFD_COLHEAD   2        /**< Dump option Column headers */
#define CFD_SRCFLAGS  4        /**< Dump option Source flag description */
#define CFD_FLAGS     8        /**< Dump option All Flags description */
#define CFD_DEFAULT   CFD_COLHEAD|CFD_SRCFLAGS  /**< Dump Mask Default */
#endif

/**
 * @}
 * @weakgroup cflib_core
 * @{
 */
/**
 * CFLIB Configuration Database Entry
 */
typedef struct _cf{
    char *name;         /**< @ref parameter_name */
    char *inhalt;       /**< Parameter Content, see @ref parameter_default */
    char option;        /**< @ref parameter_option */
    CFFLAGTYP flag;     /**< @ref special_options_mask */
} CONFIG;
/** @} */

/* FUNCTION MACROS */

/**
 * @weakgroup retrieval Information Retrieval
 * @{
 *
 * These functions and macros read entries from an initialized CFLIB database.
 * The exact name of the required parameter must be given as argument, where
 * indicated.
 */
/** Get value (content) of named entry */
#define cfget(a)       cfgetent(a,0)
/** Inquire CFLIB DB for String in content of named entry */
#define cfgetstr(a)    ((char *)cfgetent(a,CF_STR))
/** Inquire CFLIB DB for Integer value in content of named entry */
#define cfgetnum(a)    (*(int *)cfgetent(a,CF_INT))
/** Inquire CFLIB DB for Float (Real) value in content of named entry */
#define cfgetreal(a)   (*(float *)cfgetent(a,CF_REAL))
/** Inquire CFLIB DB for Double value in content of named entry */
#define cfgetdouble(a)   (*(double *)cfgetent(a,CF_DOUBLE))
/** Inquire CFLIB DB for Flag value in content of named entry */
#define cfgetflag(a)   (*(int *)cfgetent(a,CF_FLAG))
/** Inquire CFLIB DB for Bit set in entry's Special Options Flag CONFIG::flag */
#define cfflaginq(a,b) (*(int *)cfgetent(a,CF_FLGINQ|(31&b)))
/** Inquire CFLIB DB for Source of named entry's content */
#define cfgetsrc(a)    (*(int *)cfgetent(a,CF_SRC))
/** Get next Residual Command Line Argument from CFLIB DB */
#define cfgetres()     ((char *)cfgetent("",CF_RESID))
/** Get CFLIB Copyright Notice */
#define cfgetcpr()     "CFLIB (c) 1994-2012 Stefan Habermehl"
/**
 * @}
 * @weakgroup setting_saving Setting and Saving the Configuration
 *
 * Set/Update Parameter Values or Save a Configuration File
 * @{
 */
#define cfput(a,b) cfputstr(a,(char *)b)    /**< Update or Add a Parameter */
/**
 * @}
 * @weakgroup utilities General Utilities
 *
 * General Utility Macros and Functions
 * @{
 */
#define DelFlag(a,b)   a&=(~b)  /**< Delete bits of Mask @c b from Mask @c a */
#define SetFlag(a,b)   a|=b     /**< Set bits of Mask @c b in Mask @c a */
/** @} */

/*
 * FUNTION PROTOTYPES
 */
/**
 * @weakgroup cflib_core
 *
 * @{ *//**
 *
 * @ref cfinit() Initialize Configuration Database
 */
int    cfinit(CONFIG *set,int argc,char **argv);
/**
 * @ref cfexit()
 * Free allocated memory and reset configuration database and error stack
 */
void   cfexit(void);
/**
 * @ref cfstart()
 * Start Configuration Database (with error reporting and usage message)
 */
int    cfstart(CONFIG *set, int ac, char **av, char *help, int mode);

/**
 * @}
 * @weakgroup retrieval
 * @{
 */
/**
 * @ref cfgetent()
 * Inquire configuration database for content of entry name
 */
void  *cfgetent(char *name, CFFLAGTYP typ);
/**
 * @ref cfgetvers()
 * Get Library Version/Patchlevel
 */
int    cfgetvers(void);
/**
 * @ref cfgetsubvers()
 * Get Library Subversion Details
 */
char  *cfgetsubvers(void);
/**
 * @ref cfgetusg()
 * Get Usage Message for (Terminal) Output
 */
char  *cfgetusg(void);
/**
 * @}
 * @weakgroup errors
 * @{
 */
/**
 * @ref cfgeterr()
 * Error Code and Message Inquiry Function
 */
int    cfgeterr(char *string, size_t len);
/**
 * @ref cfputerr()
 * Init, exit or append to Error List
 */
int    cfputerr(int ecode, char *string, ...);
/**
 * @ref cfclearerr()
 * Free all entries in error list
 */
void   cfclearerr(void);
/**
 * @ref cfreverr()
 * Revert order of entries in error list from last->first to first->last
 */
int    cfreverr(void);
/**
 * @}
 * @weakgroup advanced_features
 * @{
 */
/**
 * @ref cfhomexp()
 * Expand @c ~ or @c ~user in parameter content
 */
char  *cfhomexp(char *name);
#ifndef _CF_NODEBUGGING
/**
 * @ref cfdinichk()
 * Debugging Function (@b experimental)
 */
int    cfdinichk(CONFIG *set);
/**
 * @ref cfdump()
 * Dump CFLIB DB content to @c fout
 */
int    cfdump(FILE *fout);
#endif
/**
 * @}
 * @weakgroup setting_saving
 * @{
 */
/**
 * @ref cfnosave()
 * Alter or query the @ref CF_NOSAVE Flag of Parameter @e name
 */
int    cfnosave(char *name, const char *onoff);
/**
 * @ref cfputstr()
 * Update or Add Parameter @e name with string @e content
 */
int    cfputstr(char *name, char *content);
/**
 * @ref cfputtime()
 * Set all Time and/or Date entries in CFLIB DB to @e now or @e today
 */
int    cfputtime(CFFLAGTYP td);
/** @} */
#ifndef _CF_NOSAVING
/**
 * @weakgroup setting_saving
 * @{
 */
/**
 * @ref cfsave()
 * Write configuration data to a Configuration File or @c stdout
 */
int    cfsave(char *fname, const char *savemode);
/**
 * @}
 * @weakgroup utilities
 * @{
 */
/**
 * @ref BackupFile()
 * Copy or Rename File @c "file" to Backup File @c "file~" or @c "file.bak"
 */
int    BackupFile(const char *file, char *modus);
/** @} */
#endif
/**
 * @weakgroup report_generation Report Generation
 *
 * Process templates doing variable substitution and file inclusion
 *
 * @{ *//**
 *
 * @ref cfform()
 * Process a Template from file or @c stdin and write generated Report to
 * File or @c stdout
 */
int    cfform(char *infile, char *outfile, char *vardelim, int mode);
/**
 * @}
 * @weakgroup utilities
 * @{
 */
/**
 * Strip Carriage Return at end of string (after fgets) by introducing
 * zero byte at CR position.
 */
void   RemoveCR(char *string);
/**
 * Strip whitespaces at end of string by introducing zero byte after last
 * non-whitespace character
 */
void   RemoveTrailSpace(char *string);
/**
 * @ref EatWhiteSpace()
 * Set pointer to next non-whitespace-character in string.
 */
char  *EatWhiteSpace(char *string);
/**
 * @ref IsATerminal()
 * Test whether stream is a terminal
 */
int    IsATerminal( FILE *fp );
#ifndef _CF_NOFINDFILE
/**
 * @ref FindFile()
 * Find a File in Path trying Extensions
 */
char *FindFile( const char *fname, const char *fpath, const char *const *fext );
#endif
/** @} */

/** @} */
/** Marker: cf.h has been included */
#define __CF_H__
#endif
