#define	LA_SER_ORIG	0x01	/* original (needed) name */
#define	LA_SER_LIBPATH	0x02	/* LD_LIBRARY_PATH entry prepended */
#define	LA_SER_RUNPATH	0x04	/* runpath entry prepended */
#define	LA_SER_CONFIG	0x08	/* configuration entry prepended */
#define	LA_SER_DEFAULT	0x40	/* default path prepended */
#define	LA_SER_SECURE	0x80	/* default (secure) path prepended */

#define REPLIT_LD_LIBRARY_PATH "REPLIT_LD_LIBRARY_PATH"

#define MAX_LD_LIBRARY_PATH_LENGTH  65535
#define MAX_PATH_LENGTH             4096

// Parser modes
#define PARSE_VARNAME               0
#define PARSE_VALUE                 1
#define PARSE_LD_LIBRARY_PATH       2

#define CHANNEL_UNKNOWN             0
#define CHANNEL_23_11               1
#define CHANNEL_23_05               2
#define CHANNEL_22_11               3
#define CHANNEL_22_05               4