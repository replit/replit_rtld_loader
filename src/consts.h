#define	LA_SER_ORIG	0x01	/* original (needed) name */
#define	LA_SER_LIBPATH	0x02	/* LD_LIBRARY_PATH entry prepended */
#define	LA_SER_RUNPATH	0x04	/* runpath entry prepended */
#define	LA_SER_CONFIG	0x08	/* configuration entry prepended */
#define	LA_SER_DEFAULT	0x40	/* default path prepended */
#define	LA_SER_SECURE	0x80	/* default (secure) path prepended */

#define MAX_LD_LIBRARY_PATH_LENGTH  65535
#define MAX_PATH_LENGTH             4096