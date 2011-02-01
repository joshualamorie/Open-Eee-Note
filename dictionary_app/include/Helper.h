/**************************************************
copyright:asus
author:paul_zhang@asus.com.cn
**************************************************/
#ifndef HELPER_H
#define HELPER_H

#include <QStringList>
#include <QPainter>

#ifdef ___cplusplus
extern "C" {
#endif

/*
Argument.
--textList: text list to draw, if its count isn't 0, the textList's join won't be called
--sep: separate character
--maxLine: the max line count
--font: text's font
--fgColor: text's color
--rect: draws the given text within the provided rect
--lineSpace: the line space
--textOption: can be used to set alignment, textDirection etc.
--highlight: highlight string
--bgColor: highlight's color
--boundingRect: the boundingRect (if not null) is set to the what 
	the bounding rectangle should be in order to enclose the whole text.
--return value: the actual line count

Usage.
DrawText(painter, tagList, QString(", "), 10, infoTextFont, QColor(66, 66, 66), 
QRect(70, 257 + 20, 600, 400), QTextOption(),
"test", QColor(255, 0, 0));

Note.
if you want to draw multiline the textOption's wordModel must be WrapAtWordBoundaryOrAnywhere or
	WrapAnywhere. but if you want to draw one line you could set it to anything
*/
int DrawText(QPainter* painter, QStringList textList, QString sep, int maxLine, 
	const QFont& font, const QColor& fgColor, const QRect& rect, int lineSpace = 0, QTextOption textOption = QTextOption(), 
	const QString& highlight = QString(), const QColor& bgColor = QColor(), QRectF * boundingRect = NULL );

char* encodeString(const char* str);

char* decodeString(const char* str);
//mp3type:  0:VBRmp3  1:CBRmp3  2:NOmp3  3:VBRImp3
struct mpeglayerIII
{
	unsigned int mp3type;
	unsigned int duration;      //unit ms

	//for VBR
	bool hasToc;
	unsigned int  tocTableSize;         //tocLength;      //unit byte
	unsigned char *xingTable;
	unsigned int filesize;        //unit byte

	//for VBRI
	unsigned int tocTableScale;  
	unsigned int tocEntrybytes;  //bytes per Entry
	unsigned int tocEntryFrames; //frames per Entry
	unsigned int *vbriTable;
};

bool IsMp3File(const QString &fileName);
int _GetFileType(const QString &fileName);//-1 unknown type, 0 not mp3, 1 is mp3
#ifdef ___cplusplus
}
#endif

#endif // HELPER_H
