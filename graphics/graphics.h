/* graphics.h -
 *
 *     *********************************************************************
 *     * Copyright (C) 1985, 1990 Regents of the University of California. *
 *     * Permission to use, copy, modify, and distribute this              *
 *     * software and its documentation for any purpose and without        *
 *     * fee is hereby granted, provided that the above copyright          *
 *     * notice appear in all copies.  The University of California        *
 *     * makes no representations about the suitability of this            *
 *     * software for any purpose.  It is provided "as is" without         *
 *     * express or implied warranty.  Export of this software outside     *
 *     * of the United States of America may require an export license.    *
 *     *********************************************************************
 *
 * This file contains a bunch of macros that look like
 * normal procedure calls but really indirect through procedure
 * pointers in order to achieve graphics display independence.
 */

/* rcsid "$Header: /usr/cvsroot/magic-8.0/graphics/graphics.h,v 1.3 2010/06/24 12:37:18 tim Exp $" */

#ifndef _GRAPHICS_H
#define	_GRAPHICS_H

#include "utils/magic.h"
#include "utils/geometry.h"

/* data structures */
typedef struct {
    int idx, mask, color, outline, fill, stipple;
    char shortname;
    char *longname;
} GR_STYLE_LINE;

typedef struct {	/* Colormap table entry */
    char *name;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} colorEntry;

/* Housekeeping and initialization routines */
bool (*GrInitPtr)();
void (*GrClosePtr)();
void (*GrTextSizePtr)();

/*
 * Display painting and text routines
 *
 *    You must call GrLock() before using these, and
 *    call GrUnlock() afterwards.
 */
void (*GrLockPtr)();
void (*GrUnlockPtr)();
bool GrHaveLock();
void GrClipTo();
void GrClipBox();
void GrClipLine();
bool GrPutText();
void GrFillPolygon();
void (*GrDrawGlyphPtr)();
void (*GrBitBltPtr)();
int  (*GrReadPixelPtr)();
void (*GrFlushPtr)();

/* Tablet routines */
void (*GrEnableTabletPtr)();
void (*GrDisableTabletPtr)();
void (*GrSetCursorPtr)();

/* graphics routines that are called in the same way for all displays */
int  GrLoadStyles();
bool GrLoadCursors();
bool GrSetDisplay();
bool GrIsDisplay();
void GrLabelSize();
void GrSetStuff();
void GrDrawFastBox();
void GrGuessDisplayType(
    char **graphics,		/* default device for sending out graphics */
    char **mouse,		/* default device for reading mouse (tablet) */
    char **display,		/* default type of device (OGL, etc...) */
    char **monitor		/* default type of monitor (pale, std) */
);
int  GrGetStyleFromName();
bool GrDrawGlyphNum();

#define GrFastBox(x)	GrDrawFastBox(x, 0)

/* external color map routines */
bool GrReadCMap(), GrSaveCMap();
bool GrGetColor(), GrPutColor();
int  GrNameToColor();
void GrPutManyColors();
void (*GrSetCMapPtr)();
void GrResetCMap();

/* Routines for windows, called only if non-null.  See SUN160 driver for
 * details.
 */
bool (*GrCreateWindowPtr)();	/* Passed a window just after it is
					 * created.
					 */
void (*GrDeleteWindowPtr)();	/* Passed a window just before it
					 * is destroyed.
					 */
void (*GrDamagedPtr)();		/* Called at a conventient time after
					 * receiving a SIG_WINCH signal.
					 */
void (*GrUpdateIconPtr)(); 	/* Adjust text on icon.
					 */
bool (*GrEventPendingPtr)(); 	/* Check for pending graphics events.
					 */
void (*GrConfigureWindowPtr)();	/* Called to reconfigure size/location
					 * of an existing window.
					 */
void (*GrOverWindowPtr)();	/* Raise window to top of stack.
					 */
void (*GrUnderWindowPtr)();	/* Lower window to bottom of stack.
					 */
int (*GrWindowIdPtr)(); 		/* Get the window ID from a window name
					 */
char *(*GrWindowNamePtr)(); 	/* Get the window name from a MagWin ptr.
					 */
bool (*GrGetCursorPosPtr)(); 	/* Get the current cursor position
					 */
bool (*GrGetCursorRootPosPtr)();	/* Get the current cursor position
					 * in root (screen) coordinates
					 */

/* Backing store mechanism (added 4/2/05) */
bool (*GrGetBackingStorePtr)();
bool (*GrScrollBackingStorePtr)();
void (*GrPutBackingStorePtr)();
void (*GrCreateBackingStorePtr)();
void (*GrFreeBackingStorePtr)();

/* Routines called by the signals module just before Magic is stopped
 * (such as via ^Z) and just after it is resumed.
 */
void (*GrStopPtr)();
void (*GrResumePtr)();
#define GrStop	(*GrStopPtr)
#define GrResume (*GrResumePtr)

/* Number of colors defined in the colormap */
int GrNumColors;

/* Different graphics (OpenGL vs. X11, for instance) have different
 * definitions of line and rectangle/polygon coordinates.  Usually
 * this boils down to a single pixel difference on rectangle edges.
 * GrPixelCorrect should be set to 0 for OpenGL and other real-valued
 * coordinate systems, 1 for X11 and other pixel-based systems.
 */
int GrPixelCorrect;

/* The size of the screen in screen coordinates */
Rect GrScreenRect;

/* The size of crosses (drawn for zero-size boxes), in pixels. */
Rect GrCrossRect;

/* Multiples of the grid spacing which we draw lighter, for a fading effect */
unsigned char GrGridMultiple;

/* Style tables */
GR_STYLE_LINE *GrStyleTable;
int **GrStippleTable;
int GrStyleNames[];

#ifndef lint

/* Constants for easy access */

#define GrLock (*GrLockPtr)
#define GrUnlock (*GrUnlockPtr)
#define GrInit (*GrInitPtr)
#define GrClose (*GrClosePtr)
#define GrSetCMap (*GrSetCMapPtr)
#define GrTextSize (*GrTextSizePtr)
#define GrDrawGlyph (*GrDrawGlyphPtr)
#define GrBitBlt (*GrBitBltPtr)
#define	GrReadPixel (*GrReadPixelPtr)
#define GrFlush (*GrFlushPtr)

#define GrEnableTablet (*GrEnableTabletPtr)
#define GrDisableTablet (*GrDisableTabletPtr)
#define GrGetCursorPos (*GrGetCursorPosPtr)
#define GrGetCursorRootPos (*GrGetCursorRootPosPtr)
#define GrGetButtons (*GrGetButtonsPtr)
#define GrSetCursor (*GrSetCursorPtr)

#endif

enum { DISPLAY_IDLE, DISPLAY_IN_PROGRESS, DISPLAY_BREAK_PENDING, DISPLAY_SUSPEND };
unsigned char GrDisplayStatus;

/* constants */

/* Rectangle filling styles.
 * (note: this must match the array in grDStyle.c)
 */

#define GR_STSOLID      0
#define GR_STCROSS      1
#define GR_STOUTLINE    2
#define GR_STSTIPPLE    3
#define GR_STGRID       4

/* 4 text sizes that are specified by the graphics driver, plus a 5th one.
 * This last one is for systems that allow the user to specify a default font
 * (such as a sun) -- on other systems it's size is decided by the driver.
 *
 * Note:
 *    These must be in sequential, ascending order.
 */
#define GR_TEXT_SMALL	0
#define GR_TEXT_MEDIUM	1
#define GR_TEXT_LARGE	2
#define GR_TEXT_XLARGE	3
#define GR_TEXT_DEFAULT	4

/* Default cursor position -- used on startup and if we can't read the cursor */
#define GR_CURSOR_X	100
#define GR_CURSOR_Y	100

/* Special full-screen access for window manager only */
#define GR_LOCK_SCREEN	(MagWindow *)(-1)

#endif /* _GRAPHICS_H */
