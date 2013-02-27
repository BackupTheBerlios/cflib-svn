/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @file cflib_doc.h
 *
 * User  Documentation Supplement Dummy C Header File for
 * @b CFLIB Flexible Configuration Library
 *
 * @note      Include this file in the source tree of documentation tools
 *            that support "@ref" references in order to resolve all unhandled
 *            references in Public Header File cf.h
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 2009-2013 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU Lesser General Public License version 3.0 (LGPLv3)
 * @package   CFLIB Flexible Configuration Library
 * @subpackage Library_Documentation : @ref cflib
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
 * 2009-02-02 [sh] File created
 *
 ******************************************************************************/

#ifndef __CFLIB_H__
#ifndef __CFLIB_DOC_H__

/***************************************************************************//**
 * @if CFLIB_Tagfile_Supplement
 * @mainpage CFLIB Flexible Configuration Library
 * @subpage cflib
 * @endif
 *
 * @page cflib Flexible Configuration Library - User Documentation
 *
 * @section intro Introduction and General Issues
 *
 * CFLIB is a small, simple, flexible and portable ANSI C Library to be used as
 * configuration interface for user programs. CFLIB builds and maintains a
 * compact database structure consisting of a list of parameters with their
 * name, content and some additional information about each parameter.
 *
 * CFLIB targets the basic needs of technical, scientific or other programmers
 * who want to spend minimal time on coding input, output, variable parsing,
 * report generation and the like but still have a simple to use,
 * reliable, flexible and portable configuration interface for their programs.
 *
 * @par Main Features:
 *
 * - Commandline, environment and terminal input parsing
 * - Configuration files
 * - File search
 * - Template driven report generation
 * - Automatic time and date update
 *
 * @section project_homepage Project Homepage
 *
 * The text you are reading is part of the User Documentation for
 * the Flexible Configuration Library that supplements the library and its
 * include file cf.h. More (detailed) information, source code and maybe
 * updates are available from the library homepage:
 *
 * - http://cflib.berlios.de
 *
 * Anonymous (Read Only) SVN Checkout is supported!
 *
 * @section basic_usage Basic Usage
 *
 * 1. Include cf.h
 *
 * 2. Define the @subpage config_initializer, an array of CONFIG structures
 *
 * 3. Call cfinit() with Configuration Initializer and Commandline.
 *   The most compact initialization is done by cfstart(), a wrapper function
 *   for cfinit() that includes error reporting, usage message and (optional)
 *   debugging output. On initialization the following data sources are
 *   inspected or parsed in the order presented:
 *
 * - Commandline Arguments (or compatible structure given to cfinit())
 *   according to the description in @ref parameter_option
 * - Environment Variables
 * - @subpage config_files
 * - Built-in User-defined Default from Configuration Initializer:
 *   @ref parameter_default
 * - Get parameter value from Standard Input @c stdin, if required
 *
 * 4. Use the @ref retrieval_functions @b cfget*() to access configuration
 *    parameters
 *
 * 5. Compile your program, linking the appropriate CFLIB library file for your
 *    platform and setup, usually the file name is @e libcf.a which means
 *    the library is referred to as @b "cf". You can change the names to fit
 *    into your setup: See @ref project_homepage for details.
 *
 * 6. Run your program, test CFLIB functionality and adjust the
 *    @ref config_initializer according to your needs
 *
 * @see @subpage config_levels
 * @see @ref cflib_core
 *
 * @section simple_example Simple Usage Example
 *
 * Program: @e fici.c: Find a File in Path
 * Input:   Commandline or Environment
 * Output:  File path, Help message or Error message
 * Compile: gcc fifi.c -o fifi -lcf -L../../lib@e platform
 * Run:     ./fifi -h
 * Test:    ./fifi gcc
 *
 * @code
 * #include <stdio.h>
 * #include "cf.h"
 *
 * int main( int argc, char **argv )
 * {
 * 	int ret; char *tmp;
 *
 *	CONFIG setting[] =
 *	{
 *      // Name, Default Content, Commandline Option, Special Option Flags
 *		{ "PROGNAME", NULL,        '1', CF_FINDFILE|CF_SET_ARG|CF_NO_OPT_ARG, },
 *		{ "FF_PATH",  NULL,        ' ', CF_PATH,                              },
 *		{ "help",     CF_FLAG_OFF, 'h', CF_FLAG|CF_LAST,                      },
 *	};
 *
 *	if ( !( ret = cfstart( setting, argc, argv, "help", CFS_USG ) ) ){
 *
 *		if( ( tmp = cfgetstr("PROGNAME") ) != NULL )
 *
 *			fprintf( stdout, "%s\n", tmp );
 *	}
 *
 *	return ret;
 * }
 * @endcode
 *
 * @section retrieval_functions Retrieval Functions
 *
 * 1. Get CFLIB Version and Copyright Information:
 *    cfgetvers(), cfgetsubvers(), cfgetcpr()
 *
 * 2. Get Usage Message for Output: cfgetusg()
 *
 * 3. Get Configuration Parameter Value:
 *
 * - Get parameter value: cfget(),
 *   interpretation of content and return type depend on the type setting in
 *   the parameter's @ref special_options_mask
 * - Get string value: cfgetstr()
 * - Get integer value: cfgetnum()
 * - Get real/float value: cfgetreal()
 * - Inquire flag/switch status: cfgetflag()
 * - Get value of (next) residual command line argument: cfgetres()
 *
 * @note All of these functions except cfgetres() require the parameter name as
 *       argument
 *
 * @section set_save Setting Parameters and Saving the Configuration
 *
 * - cfput()     : @copybrief cfput()
 * - cfputstr()  : @copybrief cfputstr()
 * - cfputtime() : @copybrief cfputtime()
 * - cfnosave()  : @copybrief cfnosave()
 * - cfsave()    : @copybrief cfsave()
 *
 * - See @ref config_files for details
 *
 * @section general_utilities General Utilities
 *
 * These functions and Macros are used in the library but do not depend on the
 * configuration database or any cf*() functions. They are (small) general
 * tools that you can use in your program if you like.
 *
 * - String Manipulation: EatWhiteSpace(), RemoveCR(), RemoveTrailSpace()
 * - File Utilities: FindFile(), BackupFile()
 * - Other Tools: IsATerminal(), DelFlag(), SetFlag()
 *
 * @section advanced Advanced Usage
 *
 * @subsection error_handling Error Handling
 *
 * CFLIB maintains a simple global Error Stack that is used by library functions
 * like cfinit() when multiple errors can occur. Error Items consist of a
 * numeric Error Code and (optionally) an Error Message string.
 * Repeated calls to cfgeterr() will successively return error entries while
 * deleting them from the stack until the list is empty.
 * User programs may also use the CFLIB error stack by calling cfputerr()
 * without the need to initialize a configuration.
 *
 * For more details see: @ref errors - @copybrief errors
 *
 * @subsection reports Report Generation
 *
 * - Generate Output from Template and current parameter values: cfform()
 *
 * @subsection debugging Configuration Debugging
 *
 * These functions are thought to be used by the programmer working with CFLIB
 * during development and testing of a program.
 *
 * - Dump Configuration DB in human readable form: cfdump()
 * - Test and Dump Configuration Initializer: cfdinichk()
 *
 * @subsection metainfos Get Information About a Configuration Parameter:
 *
 * - Get source/origin of the parameter's value: cfgetsrc()
 * - Inquire Bit from parameter's Special Options Mask by Offset: cfflaginq()
 *
 * @subsection advanced_other Other Functions and Features
 *
 * - Exit Configuration: cfexit()
 * - Expand User Home Directory in a File Path Parameter: cfhomexp()
 * - General (internal) retrieval function: cfgetent()
 *
 * @section compilation_issues Compilation and Development Issues
 *
 * For information about platforms, binaries, library building and compilation
 * options visit the project homepage http://cflib.berlios.de!
 *
 * There you will also find a small example C source module @e cf_minimal.c
 * that serves as a @b "minimal replacement" of (all) CFLIB library functions.
 * You can adapt and include the source in your project together with cf.h
 * to completely remove library dependency in case you don't need most of the
 * functionality or if a CFLIB library executable is not available or buildable
 * for your system or setup.
 *
 * @section help Help and Support
 *
 * - @b Help is this documentation
 * - @b Support is the (open) source code
 * - The project is maintained from time to time as needed ;-)
 * - Comments, Bug Reports or (better) Bug Fixes are welcome!
 * - See Flexible Configuration Library at http://cflib.berlios.de
 * - ... Have Fun!
 *
 ***************************************************************************//**
 *
 * @page config_initializer Configuration Initializer
 *
 * The @b "Configuration Initializer" is the major interface between the user
 * program and CFLIB. It is an Array of @ref CONFIG items, just as the CFLIB
 * Configuration Database itself.
 *
 * Every parameter is characterized by:
 *
 * - @ref parameter_name (@ref CONFIG::name)
 *
 * - @ref parameter_default (@ref CONFIG::inhalt)
 *
 * - @ref parameter_option (@ref CONFIG::option)
 *
 * - @ref parameter_flagmask (@ref CONFIG::flag)
 *
 * An example initializer may look like this:
 * @code
 * const CONFIG initializer[] = {
 *       // name        default        option  flag
 *     { "SPI_PROFILE", "~/.spi.cnf",  'c',    CF_SETFILE|CF_SET_ENV,     },
 *     { "SPI_SYSCONF", "/etc/spi.cnf",' ',    CF_SYS_SETFILE|CF_NOSAVE,  },
 *     { "SPI_DEFS",    "loop-0.A",    'f',    CF_SECTION,                },
 *     { "SPI_ID",      "",            ' ',    CF_SET_PRIV,               },
 *     { "verbosity",   "1",           'v',    CF_INT|CF_CONCAT|CF_NOSAVE,},
 *     { "label_items", CF_FLAG_ON,    'l',    CF_FLAG,                   },
 *     { "label_date",  NULL,          ' ',    CF_DATE|CF_SET_PUT,        },
 *     { "label_logo",  NULL,          'L',    0,                         },
 *     { "output_file", NULL,          'o',    CF_CONCAT|CF_QUERY,        },
 *     { "TERM",        "0.2",         'T',    CF_STR|CF_IGN_ENV,         },
 *     { "SCALE",       "1",           'S',    CF_REAL|CF_IGN_ENV,        },
 *     { "LOOPMOD",     "random",      '1',    CF_NO_OPT_ARG,             },
 *     { "CYCLES",      "2009",        '2',    CF_INT|CF_NO_OPT_ARG,      },
 *     { "CF_DUMPVERB", CFD_COLHEAD,   ' ',    CF_INT|CF_NOSAVE|CF_LAST,  },
 * };
 * @endcode
 *
 * The Configuration Initializer controls the behavior and actions of all
 * @b cf*() functions, starting with the control of @ref initialization_process.
 *
 * @section parameter_name Parameter Name
 *
 * Every entry in the initializer must have a name, that is a non-empty string,
 * which is searched for in the environment and in the configuration files and
 * which is used as an argument to the inquiry functions.
 * Parameters that have a name beginning with @b "CF_" may have special meaning,
 * see @ref cflib_parameters.
 *
 * @section parameter_default Parameter Default Value
 *
 * As the default content of an entry you can specify a @c NULL pointer or a
 * string, which may be empty. If @ref CF_FLAG is set for that entry you should
 * use the @ref CF_FLAG_ON or @ref CF_FLAG_OFF macros.
 *
 * @section parameter_option Commandline Option for Parameter
 *
 * You can control whether and how an entry's content can be set from the
 * Commandline by setting one of the following
 * @b "Option Specifier" Characters
 * (and Special Option Flags where indicated):
 *
 * @arg '<em> </em>' (blank)
 *         - No Commandline Option for this Parameter
 *
 * @arg '<em>c</em>' (@e c = any "normal" character)
 *         - Looks for '-c <@e string>' on the Commandline
 *         - Looks for '-c<@e string>' on the Commandline, if Flag
 *           @ref CF_CONCAT is set in the Special Options Mask
 *         - Looks for '-c<@e char>' on the Commandline, if Flag
 *           @ref CF_FLAG is set in the Special Options Mask
 *
 * @arg '<em>#</em>' (@e # = a positive number = 1, 2, 3, ...)
 *         - Looks for the 1st, 2nd, 3rd, ... '<@e string>' that is not part of
 *           an option, if Flag @ref CF_NO_OPT_ARG is set in
 *           the Special Options Mask
 *
 * @section parameter_flagmask Special Option Flags Mask for Parameter
 *
 * Most of the magic happens here! ;-)
 * @copybrief special_options_mask See @ref special_options_mask for details.
 *
 * @see @subpage parameter_types
 * @see @ref config_levels
 *
 * @attention The last entry in the initializer list must have the
 *            @ref CF_LAST Flag set!
 *
 ***************************************************************************//**
 *
 * @page config_levels Configuration Parsing Levels and Source/Origin Options
 *
 * CFLIB is designed to give you a maximum of possibilities for feeding
 * parameters and parameter values into the configuration.
 *
 * @section origin_window Origin Window
 *
 * The library remembers the source/origin of all parameter values by setting
 * one of the @b "Source Flags" in the @b "Origin Window" of the
 * @ref special_options_mask ranging from Bit Offset 8 to 15
 * (see @ref parsing_levels).
 * On initialization the Origin Window Flags @b CF_SET_* of an entry in the
 * @ref config_initializer are used to control  @ref parsing_level_priorities.
 *
 * @section parsing_levels  Parsing Levels
 *
 * @b "Parsing Levels" are equal to the Relative Bit Offset of the corresponding
 * @b CF_SET_* Bit within the "Origin Window":
 *
 * Parsing Level : Description (Source Flag)
 *
 * - 0 = @ref CFP_PUT  : @copybrief CFP_PUT (@ref CF_SET_PUT)
 * - 1 = @ref CFP_ARG  : @copybrief CFP_ARG (@ref CF_SET_ARG)
 * - 2 = @ref CFP_ENV  : @copybrief CFP_ENV (@ref CF_SET_ENV)
 * - 3 = @ref CFP_PRIV : @copybrief CFP_PRIV (@ref CF_SET_PRIV)
 * - 4 = @ref CFP_SYS  : @copybrief CFP_SYS (@ref CF_SET_SYS)
 * - 5 : @ref CFP_DEF  : @copybrief CFP_DEF (@ref CF_SET_DEF)
 * - 6 : @ref CFP_QRY  : @copybrief CFP_QRY (@ref CF_SET_QRY)
 * - 7 : @ref CFP_RESERVED : Reserved for CFLIB Subprojects
 *
 * @section initialization_process Parsing Levels in the Initialization Process
 *
 * On initialization the Parsing Levels 1 through 6 are processed by cfinit()
 * in that order and priority. If a Parameter is found that has been defined in
 * the Configuration Initializer, its content is included as String Content of
 * that parameter in the configuration database.
 *
 * Whether and how the @b Commandline Arguments are used to get a parameter
 * value depends on the CONFIG::option setting for that parameter and the
 * Special Option Flags:
 *
 * - @ref CF_CONCAT : @copybrief CF_CONCAT
 * - @ref CF_NO_OPT_ARG : @copybrief CF_NO_OPT_ARG
 *
 * See details under @ref parameter_option.
 *
 * The @b Environment can be excluded as a possible source of a parameter value
 * by setting the Special Option Flag
 *
 * - @ref CF_IGN_ENV  : Do not look for Environment Variable
 *
 * @b Configuration Files are only used when corresponding entries are defined
 * in the Configuration Initializer, see @ref config_file_parsing for details.
 *
 * @b Interactive queries are not performed by default. If cfinit() shall ask
 * the user for interactive input of items that are still unresolved after
 * parsing all lower levels, you must set Special Option Flag:
 *
 * - @ref CF_QUERY : @copybrief CF_QUERY
 *
 * If an entry may not be empty (@c NULL or string "") after initialization, you
 * can have cfinit() produce an appropriate error by setting the
 * Special Option Flag:
 *
 * - @ref CF_MUST :  @copybrief CF_MUST
 *
 * @subsection parsing_level_priorities Parsing Level Priorities
 *
 * More precise control of parsing levels priorities and origin control for a
 * parameter is available through setting one of the
 * Flags in the "Origin Window" of the parameter's @ref special_options_mask in
 * the @ref config_initializer :
 *
 * - @ref CF_SET_PUT  : Must be set by a function call / automatic processing:
 *                      Set this Flag on Time or Date @ref parameter_types for
 *                      automatic initialization with @e now or @e today
 *
 * - @ref CF_SET_ARG  : Force initialization from commandline,
 *                      ignore anything else
 *
 * - @ref CF_SET_ENV  : Let environment variable override earlier setting
 *
 * - @ref CF_SET_PRIV : Let variable from private configuration file
 *                      override earlier setting
 *
 * - @ref CF_SET_SYS  : Let variable from system configuration file
 *                      override earlier setting
 *
 * - @ref CF_SET_DEF  : Let default from Configuration Initializer
 *                      override earlier setting
 *
 * - @ref CF_SET_QRY  : Force setting from interactive Query (@c stdin)
 *
 * @section config_file_parsing Parsing of Configuration Files
 *
 * To indicate that an entry in the initializer list given to cfinit()
 * represents the filename of one of the @ref config_files, set one of the
 * following Flags for that parameter:
 *
 * - @ref CF_SETFILE     : @copybrief CF_SETFILE
 * - @ref CF_SYS_SETFILE : @copybrief CF_SYS_SETFILE
 *
 * Within the system configuration file you can choose a section, from which
 * information is read as from a single file. To specify an entry in the list
 * referring to the name of that section give this entry the flag
 *
 * - @ref CF_SECTION : @copybrief CF_SECTION
 *
 * As the filenames for the two configuration files are themselves entries
 * in the database, these levels are revisited, if neccessary, in reverse order
 * after parsing the built-in default.
 *
 * @section residuals Residual Items
 *
 * Any commandline argument that did not match conditions for a parameter will
 * be included in the configuration database as parameter without name marked
 * with Flag @ref CF_RESID in the @ref special_options_mask. These additional
 * entries from the command line can be accessed by successive calls to
 * cfgetres().
 *
 * Residual items from one of the configuration files will also be accumulated
 * in the database and can be accessed through inquiry by name with one of the
 * @ref retrieval_functions. This is especially useful together with the
 * report generation function cfform().
 *
 *
 * @page config_files Configuration Files
 *
 * A major task of the library is handling import and export of configuration
 * parameters from/to files. CFLIB knows two types of configuration files:
 *
 * - @b "Private Configuration File" - User and/or program specific file in
 *   simple format to be read on initialization and optionally be updated
 *   by cfsave()
 *
 * - @b "System Configuration File" - System and/or project specific file in
 *   extended format (supporting sections, see below) will only be used as a
 *   data source by cfinit() and will not be touched by cfsave() unless you
 *   explicitly specify the filename.
 *
 * @section config_format Configuration File Format
 *
 * In a configuration file lines beginning with @c '#' are treated as comments
 * and are ignored. Blank lines are ignored, too. A valid line in the file is of
 * the form:
 *
 * <@e name> = <@e entry>
 *
 * Blank chars around the @c '=' are ignored. The name must match one of the
 * entry's names in the initializer. In fact, any line not containing a @c '='
 * will be ignored, but it's better to indicate comments with @c '#'!
 *
 * The optional sections in the system configuration file begin with a line
 * like:
 *
 * [<@e sectionname>]
 *
 * and end with another line like this or with the file's last line. Anything
 * after the closing bracket is ignored.
 *
 * A simple example of a valid configuration file could look like this:
 * @code
 * # This is my private configuration file for Project 1356 Branch C in spe
 *
 * Search_Path  = /my/subproject/directory:/general/settings/directory
 * Section      = project_1356
 * Outfile      = my_subproject.cnf
 * ask_if_empty =
 * My_Flag =      ON
 * VERBOSITY = 1
 * @endcode
 *
 * The corresponding system configuration file could look like this:
 * @code
 * # This is a system wide configuration file
 * [some_other_program]
 * blah = blubber
 * ...
 * [project_1356]
 * # Settings for Project 1356 Branch B
 * VERBOSITY = 3
 * Outfile = project_1356b.cnf
 * X_EUR_USD=1.4562
 * [some_other_project]
 * ...
 * @endcode
 *
 * @section config_in Reading Configuration Files
 *
 * A search for configuration files and import of data from these sources is
 * only performed by the function cfinit() on initialization and only when
 * appropriate Special Option Flags are set as described under
 * @ref config_file_parsing.
 *
 * Configuration files are read once when the configuration database is
 * initialized by a call to cfinit() or cfstart() using the internal function
 * cfreadfile().
 *
 * @section config_out Writing a (private) Configuration File
 *
 * Parameter export does not depend on any specific setting and can be performed
 * whenever and as often as you like. For writing a configuration file call
 * cfsave() with either the name of the file or @c NULL, in which case the
 * current value of the "Private Configuration File" parameter will be used, if
 * it exists. If an entry has the @ref CF_NOSAVE flag set, it is excluded from
 * saving. The "System Configuration File" may not be referred to directly.
 *
 * @attention Writing @b section marks is @b not supported!
 *
 ***************************************************************************//**
 *
 * @page parameter_types Types of Parameters
 *
 * The internal format of a parameter's content @ref CONFIG::inhalt is always a
 * String (char Pointer). The use and interpretation of parameters is
 * controlled by a number of Flags in the @ref special_options_mask :
 *
 * @section parameter_datatypes Parameter Data Types
 *
 * - @ref CF_STR : @copybrief CF_STR (@b Default).
 * - @ref CF_INT : @copybrief CF_INT
 * - @ref CF_REAL : @copybrief CF_REAL
 * - @ref CF_FLAG : @ref parameter_type_flag
 *
 * @section processing_instructions Special Processing Instructions
 *
 * Specific processing of a String Parameter's Value/Content in various
 * CFLIB functions can be triggered by these Flags:
 *
 * You can mark a parameter as Time @b or Date. These parameters will be
 * initialized with @e now or @e today by cfinit() when
 * @ref parsing_level_priorities are controlled by a @ref CF_SET_PUT Flag in the
 * @ref special_options_mask or on call of cfputtime():
 *
 * - @ref CF_TIME : @copybrief CF_TIME
 * - @ref CF_DATE : @copybrief CF_DATE
 *
 * - @ref CF_FINDFILE : @copybrief CF_FINDFILE
 *   FindFile() will be used on initialization to search the file in the @b path
 *   found in Environment Variable @b PATH. You can specify an alternative
 *   search path for this functionality in another database parameter with
 *   Flag @ref CF_PATH set in the corresponding @ref config_initializer entry.
 *   Depending on the Operating System a list of possible of @b extensions of
 *   executables will be tried, if no extension is given with the filename:
 *   - Linux, Unix: "sh", "pl"
 *   - DOS,Windows, OS2: "exe", "com", "bat", "cmd"
 *   - TOS, MinT: "ttp", "tos", "prg", "app", "gtp"
 *
 * - @ref CF_EXPHOME : @copybrief CF_EXPHOME See cfhomexp().
 *   @ref config_files will have home directory expanded by default
 *
 * @section handling_instructions Special Handling Instructions
 *
 * A parameter can be marked as a @b "volatile" entry that is not saved by
 * cfsave():
 *
 * - @ref CF_NOSAVE : @copybrief CF_NOSAVE
 *
 * @section cflib_parameters CFLIB Parameters
 *
 * Some String Parameters in the configuration database are used by the library
 * itself and are marked by one of the following Flags:
 *
 * - @ref CF_PRGNAME : @copybrief CF_PRGNAME
 *   This parameter @b always exists as the @b first entry in a
 *   configuration database successfully initialized by cfinit(). The default
 *   parameter name used if there was no corresponding entry in the Initializer
 *   is @b "CF_PRGNAME".
 * - @ref CF_SETFILE : @copybrief CF_SETFILE See @ref config_files
 * - @ref CF_SYS_SETFILE : @copybrief CF_SYS_SETFILE See @ref config_files
 * - @ref CF_SECTION : @copybrief CF_SECTION See @ref config_files
 * - @ref CF_PATH : @copybrief CF_PATH See @ref CF_FINDFILE and FindFile()
 * - @ref CF_USAGE : @copybrief CF_USAGE See cfgetusg()
 *
 * Optional Parameters for fine tuning CFLIB behavior without Special Option
 * Flag, characterized by Parameter Name:
 *
 * - @b "CF_DUMPVERB" - Non-Default Verbosity Mode for cfdump()
 *
 * @section parameter_type_flag FLAG Parameter Type
 *
 * The entry is treated like a boolean variable and the string content set
 * from any of the @ref config_levels except the default setting is interpreted
 * as follows:
 *
 * @arg @ref CF_FLAG_OFF : String Content is:
 *              - beginning with a @c '-'
 *              - @c "OFF" or @c "off"
 *              - @c "FALSE" or @c "false"
 * @arg @ref CF_FLAG_ON : any other case
 *
 * @par Example:
 * - Initializer entry: { "extended_message", CF_FLAG_OFF, 'x', CF_FLAG, }
 * - Commandline: myprog -x-
 * - Configuration file: extended_message = OFF
 *
 ******************************************************************************/
#define __CFLIB_DOC_H__
#endif
