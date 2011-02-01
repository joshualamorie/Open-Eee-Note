/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#ifndef SCANDICT_H
#define SCANDICT_H

#include <QString>
#include <QStringList>
#include <glob.h>

class CScanDict
{
    public:
        /**
         *   @fn    runScan()
         *   @brief scan all the dictionaries present in the given directory and store them
         *          defines Globbing structure, gives the appropriate format of dictionaries path (specific location)
         *   @param pDictPath directory path in which all stardict dictionaries are present
         *   @return QStringList of .ifo file path names of all the available dictionary
         */
        QStringList runScan(const char *pDictPath);

        /**
         *   @fn loadBookname(QString ifoFileName, QString &bookname)
         *   @brief load bookname from .ifo file of dictionaries
         *   @param ifoFileName .ifo file path of the dictionary
         *   @param bookname dictionary name of the dictionary
         *   @return true if dictionary is of correct type and its name is successfully parsed
         *           the bookname argument is appropriately modified (arg passed by reference)
         */
        bool loadBookname(QString ifoFileName, QString &bookname);

    private:
        QStringList ifoPathList;
        /**
         *   @fn    scanDir()
         *   @brief recursively scan the given dictionaries path and check for pattern matching of dictionary .ifo files path
         *          This is internally used by runScan for scanning the dictionary .ifo files
         *   @param pPath
         *   @param globs
         *   @return    number of files in the given directory
         */
        int scanDir(const char *pPath, glob_t *globs);
};
#endif
