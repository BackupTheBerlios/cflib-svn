/*******************************************************************************
 *            CFLIB - Flexible Configuration Library
 ***************************************************************************//**
 *
 * @internal @file cflib.h
 *
 * Library Internal Definitions
 *
 * This file is included by cf*.c modules and holds the source code of the
 * Documentation Framework (Javadoc Style, tuned for Doxygen)
 *
 * @version   SVN: \$Id$
 * @author    Stefan Habermehl <stefan.habermehl@mcff.de>
 * @copyright (c) 1994,1995,1996,2002,2006,2007,2008,2009 Stefan Habermehl
 * @license   http://www.gnu.org/licenses GNU General Public License v3 or later
 * @package   CFLIB
 * @subpackage Library_Core
 *
 ***************************************************************************//**
 *
 * @page license CFLIB License
 *
 * This file is part of @b CFLIB - Flexible Configuration Library.
 *
 * @author Stefan Habermehl <stefan.habermehl@mcff.de>
 *
 * @copyright (c) 1994,1995,1996,1997,1998,2006,2007,2008,2009 Stefan Habermehl
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
 * @verbinclude LICENSE.txt
 *
 *//****************************************************************************
 *
 * Version history
 *
 * 2008-08-03 [sh] header with meta info and license
 * 2009-01-22 [sh] Documentation Update
 * 2009-01-28 [sh] Final Documentation Update for PL 20
 *
 ***************************************************************************//**
 * @ifnot CFLIB_Tagfile_Supplement
 *
 * @mainpage CFLIB Documentation
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
 * @see
 *    - @ref purpose
 *    - @ref license
 *    - @ref names
 *    - @ref properties
 *
 * @par Project Homepage:
 *
 *    - http://cflib.berlios.de
 *
 * @section basic_usage Basic Usage
 *
 * 1. Include cf.h
 *
 * 2. Define the @ref config_initializer, an array of CONFIG structures
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
 * - @ref config_files
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
 *    into your setup: See @ref names
 *
 * 6. Run your program, test CFLIB functionality and adjust the
 *    @ref config_initializer according to your needs
 *
 * @see @ref simple_example
 * @see @ref config_levels
 * @see @ref cflib_core
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
 * - @ref platforms
 * - @ref binaries
 * - @ref library_building
 * - @ref compilation_options
 * - @ref minimal_replacement
 *
 * @section help Help and Support
 *
 * - @b Help is this documentation
 * - @b Support is the (open) source code
 * - The project is maintained from time to time as needed ;-)
 * - Comments, Bug Reports or (better) Bug Fixes are welcome!
 * - See @ref license
 * - ... Have Fun!
 *
 * @endif
 *
 ***************************************************************************//**
 *
 * @page purpose Purpose of CFLIB
 *
 * This library started as a Beginner Project in "C Library Building" following
 * practical needs arising from scientific/technical modeling projects.
 * Most of the library code has been created in 1994/95 on an Atari ST4/16 MHz
 * under TOS 2.05/MiNT 1.12 with gcc 2.5.8 and the Mintlibs Patchlevel 46.
 * The C coding is probably not the best possible ;-) but the source as well
 * as the executable and allocated memory structures are very compact, simple
 * to modify and still fine for many applications that need a stable, portable
 * and small configuration interface.
 *
 * @par 'Mission Statement' from 1994/95 README file:
 * CFLIB is meant to be a flexible, sound and easy to use tool for C
 * programmers. It provides a set of functions for a standard method for
 * feeding a C program with all the (external) information it needs to perform
 * as desired with a minimum of expense for both the programmer and the user:
 * arguments, commands, program input, configurable and/or installation
 * dependent features, system settings and a lot more can be passed to the
 * program through different interfaces: command line, environment,
 * configuration files or sections within them, interactive input and last but
 * not least a built-in default. So it should be a fast and easy task for both
 * the programmer and the user to build and change a configuration. The library
 * will always take more space and perform slower than code that is written and
 * optimized for a specific requirement, but this will only be a noticeable
 * disadvantage in some cases. The library also provides some special features
 * such as file search, time and date handling, generating simple, text template
 * driven reports, etc. It is suitable for creating a comfortable and/or
 * individual user interface for existing programs that don't have one. Some
 * basic ideas for the library came from looking at the @e termcap library for
 * easy and portable terminal I/O. @n
 *
 ***************************************************************************//**
 *
 * @page properties General Notes
 *
 * @section names The names "CFLIB", "libcf", "*cf*"
 *
 * All "names" in this library source and documentation are nothing but
 * technical acronyms just as "src", "inc", "C" and are
 * @b not registered names, trademarks or anything like that ;-)
 * You may change the names of the library, the include file and on compilation
 * the function names and defines to whatever you like :-)
 *
 * @section io Input and Output
 *
 * CFLIB is designed to be usable as a @b Filter program that reads from
 * @c stdin and writes to @c stdout wherever it was desirable.
 *
 * None of the library functions will produce any terminal output (to @c stdout
 * or @c stderr) unless:
 *
 * - The @ref CF_QUERY flag is set in the @ref special_options_mask of an entry
 *   and the parameter value is still unassigned after parsing all other
 *   possible sources in cfinit().
 *
 * - You call a function that reads from or writes to files with arguments
 *   that trigger the use of @c stdin, @c stdout, @c stderr etc. according to
 *   the function's documentation.
 *
 * @section general_tasks General (future) tasks
 *
 * @todo Make proper Man Pages with function references etc.
 * @todo Make Doxygen Developer Documentation
 * @todo More Modularization! Still very similar code in various functions
 * @see @ref bug
 *
 * @section misc_general_issues Other general issues
 *
 * - @ref versions
 *
 ***************************************************************************//**
 *
 * @page simple_example Simple Usage Example
 *
 * @include fifi.c
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
 * @see @ref parameter_types
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
 ***************************************************************************//**
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
 ***************************************************************************//**
 *
 * @page advanced_example Advanced Usage Example
 *
 * Setup @ref config_initializer, generate Report and save @ref config_files
 *
 * @include fill.c
 *
 ***************************************************************************//**
 *
 * @page development Compilation and Development
 *
 * @section versions Library Versions
 *
 * There is no strict versioning of the library. A Library @b "Patchlevel" is
 * defined as an Integer Number counting @b "Major Versions". See cfgetvers().
 *
 * The individual source files contain more detailed
 * version information and they all support a file version identifier @c $@e Id$
 * for automatic update by SVN and other Source Management Tools.
 * See cfgetsubvers().
 *
 * @section platforms Platforms
 *
 * The library source code is fairly simple ANSI C code and should compile
 * and link without problems on most platforms.
 * Most of the development has been done with different versions of the
 * @b "GNU C Compiler" @c gcc and related tools. All source modules compile
 * free of errors and warnings with:
 *
 * @c "gcc -pedantic -pedantic-errors -Wall -Werror -ansi ..."
 *
 * So, if you have @c gcc, use it! Any other ANSI C compiler should also work,
 * at least after minimal adaption to the library setup,
 * see @ref compilation_options.
 *
 * until now the library has been built and used under the following setups:
 *
 * - gcc, Linux, Intel PC
 * - gcc, MinGW, MS Windows NT/XP, Intel PC
 * - gcc, MinT/TOS Atari ST
 * - cc,  Unix, IBM AIX
 * - cc,  Unix, SGI
 * - MSC, MS DOS/Windows Intel PC
 * - MSC, OS/2 Intel PC
 *
 * @section library_building Building the Library
 *
 * Compiling the source modules and building the library should be a
 * "straight forward" task:
 * - Make all objects from C sources, including @e cf.h and @e cflib.h
 * - Link the objects with @c ar or another tools to get a library executable
 *
 * @see @b Makefiles under CFLIB project tree and @ref compilation_options
 *
 * @section binaries Binaries and Executables
 *
 * - The library project should be seen as "pure" source/text code on
 *   distribution. Compile a library executable with a C compiler of your choice
 *   with appropriate setup for your platform.
 *
 * - Binary and executable versions of the library found in the project tree
 *   should be seen as examples that worked under one specific setup but have
 *   not been intensively tested and may even not be up to date. If it works
 *   for you, feel free to use them. If you build a library executable which is
 *   not too ;-) dependent on a specific setup, you can include it as example
 *   in the project tree.
 *
 * @section minimal_replacement Minimal CFLIB Replacement
 *
 * In project directory @e src/examples you may find a source file
 * @e cf_minimal.c. You can adapt this example to your need and include it
 * in your C source list on compilation instead of linking the library
 * executable. This makes user programs that use CFLIB functions independent
 * of the library at the price of very reduced functionality, which may be
 * desirable for specific executables or if you have problems with building
 * the library on the target platform.
 *
 * @section compilation_options System and Compiler dependent Defines
  *
 * The following Defines are used in the library source to decide whether to
 * include certain header files and use certain functions or defines:
 *
 * - @b _HAS_PWD :
 *              If defined, include @b <pwd.h> and make use of function
 *              @b getpwnam() in cfhomexp() to find a user's home directory
 * - @b _HAS_ISATTY :
 *              If defined, use function @b isatty() to determine whether a
 *              stream is a terminal (for interactive query/input) in function
 *              IsATerminal()
 * - @b _HAS_LIMITS :
 *              If defined, include @b <limits.h> and use @b PATH_MAX defined
 *              therein in function FindFile()
 *
 * The following Defines can be used to control, which features or functions
 * shall be excluded from the library build:
 *
 * - @b _CF_NOFINDFILE :
 *              Function FindFile(), component findfile.c
 * - @b _CF_NOSAVING :
 *              Functions cfsave() and BackupFile(), component cfwrite.c
 * - @b _CF_NODEBUGGING :
 *              Functions cfdinichk() and cfdump(), component cfdebug.c
 *
 * The following Defines can be used to switch on certain Debug Features:
 *
 * - @b DEBUG_DINICHK :
 *              Debug cfdinichk()
 * - @b DEBUG_ERROR :
 *              Debug cfputerr()
 * - @b DEBUG_TIME :
 *              Debug cfputtime()
 * - @b DEBUG_NOSAVE :
 *              Debug cfsave()
 * - @b DEBUG_BACKUP :
 *              Debug BackupFile()
 * - @b DEBUG_FINDFILE :
 *              Debug FindFile() usage in cfinit()
 * - @b DEBUG_FORM :
 *              Debug cfform()
 *
 * The following Defines can be used to switch on certain other Features:
 *
 * - @b _PREFER_BACKSLASH :
 *              Prefer Backslash as Directory Separator in function FindFile()
 * - @b _PATHSEP_SEMICOLON :
 *              Use Semicolon as Path Separator in function FindFile(), usually
 *              on systems where a ":" can appear in a directory path
 * - @b _PATHSEP_COMMA :
 *              Test Comma as Alternative Path Separator in function FindFile()
 * - @b _CF_RESID_FREE :
 *              Remove residual arguments after having read them all
 *              in cfgetent()?
 *
 * The following Platform dependent Defines are used in the library code:
 *
 * - @b unx
 * - @b linux
 * - @b atarist
 * - @b __MINT__
 *
 ******************************************************************************/

#ifndef __CFLIB_H__

#ifndef __CF_H__
#include "../include/cf.h"
#endif

/**
 * Library Identification
 */
#define Subversion "CFLIB PL " Patchlevel " $LastChangedRevision$";

/* "Flag windows" using Macros from cf.h */

/**
 * Source (Origin) Flags Window
 */
#define CF_SET          (CF_SET_PUT|CF_SET_ARG|CF_SET_ENV|CF_SET_PRIV|CF_SET_SYS|CF_SET_DEF|CF_SET_QRY)
/**
 * Entry's content came from command line or function call
 */
#define CF_SET_PUT_ARG  (CF_SET_ARG|CF_SET_PUT)
/**
 * Entry's content came from config file
 */
#define CF_SET_FIL      (CF_SET_PRIV|CF_SET_SYS)
/**
 * These sources override config file entries
 */
#define CF_OVRD_SETFILE (CF_SET_PUT|CF_SET_ARG|CF_SET_ENV)
/**
 * Argument is concatenated to option
 */
#define CF_NO_ARG_OPT   (CF_FLAG|CF_CONCAT)
/**
 * All Type Flags
 */
#define CF_TYPE         (CF_STR|CF_INT|CF_FLAG|CF_REAL)
/**
 * All Get Flags
 */
#define CF_ALLGET       (CF_TYPE|CF_RESID)

/* Size and position of CF_SET_* Origin Window */

#define CF_SET_1STBIT   8           /**< Offset for first CF_SET-bit */
#define CF_SET_LASTBIT  15          /**< Offset for last CF_SET-bit */

/* Global options and communication in library */

/**
 * All Entry Configuration Flags
 */
#define CFG_ALL         (CF_LAST|CF_MUST|CF_FINDFILE|CF_QUERY|CF_PATH|CF_SECTION|CF_FLAG|CF_TIME|CF_DATE|CF_SETFILE|CF_SYS_SETFILE)

#define CFG_SAGAIN      0x10000000  /**< @private Try again to find System config file */
#define CFG_PAGAIN      0x20000000  /**< @private Try again to find Private config file */
#define CFG_AGAIN       (CFG_SAGAIN|CFG_PAGAIN) /**< @private Try Again Flags */

#define CFG_NOSFILE     0x40000000  /**< @private System Config File Init Error */
#define CFG_NOPFILE     0x80000000  /**< @private Private Config File Init Error */
#define CFG_NOFILE      (CFG_NOSFILE|CFG_NOPFILE) /**< @private Config File Init Error */

#define CFG_SFILES      (CF_SETFILE|CF_SYS_SETFILE) /**< @private Config File Flags */

/**
 * Private Configuration File Flags
 */
#define CFG_FPRIV       (CFG_PAGAIN|CFG_NOPFILE|CF_SET_PRIV|CF_SETFILE)
/**
 * System Configuration File Flags
 */
#define CFG_FSYS        (CFG_SAGAIN|CFG_NOSFILE|CF_SET_SYS|CF_SYS_SETFILE)


#ifndef __CF_ERROR__
/**
 * @ingroup cflib_core
 *
 * Library Internal: CFLIB Configuration Database with at most @ref MAXCONF
 * parameters
 */
CONFIG *_conf[MAXCONF + 1];
#endif

/**
 * @ingroup errors CFLIB Error Handling
 *
 * Library Internal: Error List Item
 */
typedef struct _cfe{
	struct _cfe *next;          /**< Next Error Pointer */
	int errcode;                /**< Numeric Error Code */
	char errstr[CF_MAXERRSTR];	/**< Error Message String
	                                 of maximum length @ref CF_MAXERRSTR */
}
CONFERR;

/**
 * @internal
 * @addtogroup advanced_features CFLIB Advanced Features
 * @{
 */
/* Prototypes for Library Internal Functions */

/** @private */
CFFLAGTYP cfreadfile( CFFLAGTYP setfile, int *res_arg );
/** @private */
int       cfreadterm( void );

/** Debug Define, unless already set elsewhere */
#ifndef DEBUG
#define DEBUG 0
#endif

/** Marker: cflib.h has been included */
#define __CFLIB_H__

/** @} */

#endif

/******************************************************************************/
