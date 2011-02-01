/********************************************************
 lcd helper for using frame buffer
 
	by Tom 2010-05-07


 Tom 2010-05-26
 add overlay2 for RGB
*********************************************************/

/************************
 control lcd power
*************************/
int OpenLCD();
int CloseLCD();

/************************
 base frame buffer
*************************/
extern int fbBase;
extern int lenBase;
extern unsigned char * memBase;
int OpenBase();
int CloseBase();
int Clean(int color);

typedef struct _OverlayInfo {
	int x;
	int y;
	int w;
	int h;
	int bpp;
	int fmt;
	int palette;//1 : palette range -127-129 ; 0:range is 0-255;default is 0
} OverlayInfo;

/************************
 overlay1 frame buffer
*************************/
enum OVERLAY1_BPP {
	OVERLAY1_BPP_24_NOTRANS	= 0, // 24-bit mode without transparency bit.
	OVERLAY1_BPP_RESERVED	= 2, // Reserved
	OVERLAY1_BPP_8_PALETTE	= 3, // 8 bpp [256 entry, 512 or 1024 byte palette buffer]
	OVERLAY1_BPP_16			= 4, // 16 bpp [no palette buffer]
	OVERLAY1_BPP_18_UNPACKED	= 5, // 18 bpp unpacked [no palette buffer]
	OVERLAY1_BPP_18_PACKED	= 6, // 18 bpp packed [no palette buffer]
	OVERLAY1_BPP_19_UNPACKED	= 7, // 19 bpp unpacked [no palette buffer]
	OVERLAY1_BPP_19_PACKED 	= 8, // 19 bpp packed [no palette buffer]	
	OVERLAY1_BPP_24			= 9, // 24 bpp [no palette buffer]
	OVERLAY1_BPP_25			= 10, // 25 bpp [no palette buffer]
	OVERLAY1_BPP_18_666		= 11, // 18-bits/pixel in (6R+2¡¯0s, 6G+2¡¯0s, 6B+2¡¯0s) format.
};

extern int fbOverlay1;
extern int lenOverlay1;
extern unsigned char * memOverlay1;
int OpenOverlay1(OverlayInfo * info);
int CloseOverlay1();

/************************
 overlay2 frame buffer
*************************/
enum OVERLAY2_FORMAT {
	OVERLAY2_FORMAT_RGB = 0,
	OVERLAY2_FORMAT_YCbCr_444_PACKED = 1,
	OVERLAY2_FORMAT_YCbCr_444_PLANAR = 2,
	OVERLAY2_FORMAT_YCbCr_422_PLANAR = 3,
	OVERLAY2_FORMAT_YCbCr_420_PLANAR = 4,
};

enum OVERLAY2_BPP {
	OVERLAY2_BPP_RESERVED	= 2, // Reserved
	OVERLAY2_BPP_8_PALETTE	= 3, // 8 bpp [256 entry, 512 or 1024 byte palette buffer]
	OVERLAY2_BPP_16			= 4, // 16 bpp [no palette buffer]
	OVERLAY2_BPP_18_UNPACKED	= 5, // 18 bpp unpacked [no palette buffer]
	OVERLAY2_BPP_18_PACKED	= 6, // 18 bpp packed [no palette buffer]
	OVERLAY2_BPP_19_UNPACKED	= 7, // 19 bpp unpacked [no palette buffer]
	OVERLAY2_BPP_19_PACKED 	= 8, // 19 bpp packed [no palette buffer]	
	OVERLAY2_BPP_24			= 9, // 24 bpp [no palette buffer]
	OVERLAY2_BPP_25			= 10, // 25 bpp [no palette buffer]
	OVERLAY2_BPP_18_888		= 11, // 18-bits/pixel in (6R+2¡¯0s, 6G+2¡¯0s, 6B+2¡¯0s) format (pxa31x only)
	OVERLAY2_BPP_18_888_PACKED = 12,
	OVERLAY2_BPP_19_888 = 13, // pxa31x only
};

extern int fbOverlay2;
extern int lenOverlay2;
extern unsigned char * memOverlay2;
int OpenOverlay2(OverlayInfo * info);
int CloseOverlay2();
int EnableOverlay2();
int DisableOverlay2();

/************************
 draw
*************************/
int draw_string(const char * srt);
int draw_line(char c);
//int DrawPNG(const char * szName, int x, int y);

/* debug */
int open_debug_file();
int output_debug();
int close_debug_file();
