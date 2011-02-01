/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#include <stdio.h>
#include <glib.h>
#include "scandict.h"



int CScanDict::scanDir(const char *pPath, glob_t *globs)
{
    char str[256];
    int i, r, nFrom, nTo;
    int nNum;

    sprintf(str, "%s*", pPath);

	/*!< number of patterns matched till now */
	nFrom = globs->gl_pathc;
    /*!
    * Globbing by enabling GLOB_MARK, GLOB_NOESCAPE, GLOB_NOSORT, GLOB_APPEND Flags
    * GLOB_MARK: If the pattern matches the name of a directory, append `/' to the directory's name when returning it
    * GLOB_NOESCAPE: `\' is an ordinary character
    * GLOB_NOSORT: Don't sort the file names. To save time.
    * GLOB_APPEND: Append the words from this expansion to the vector of words produced by previous calls to glob
    * glob function returns 0 if it succeeds
    */
    if (nFrom == 0)
        r = glob(str, GLOB_MARK | GLOB_NOESCAPE | GLOB_NOSORT, NULL, globs);
    else
        r = glob(str, GLOB_MARK | GLOB_NOESCAPE | GLOB_NOSORT | GLOB_APPEND, NULL, globs);
    nNum = 0;

    /*! If Glob was successful then do recursive scan */
    if (r == 0)
    {
		// total no. of matches/ folders till previous globbing */
		nTo = globs->gl_pathc;
        /*!
        * Scan and glob for all the matches/files present in the current folder/ match
        *number of all such matches r = nTo - nFrom
        */
        for (i = nFrom; i < nTo; i++)
        {
            if (globs->gl_pathv[i][strlen(globs->gl_pathv[i]) - 1] == '/')  /*!< if directory */
                nNum += scanDir(globs->gl_pathv[i], globs);  /*!< recursively scan the directory */
			else
			{// if not directory, but file
                char *pFileName = globs->gl_pathv[i];
                char pFileType[4];
                strncpy(pFileType, pFileName+strlen(pFileName)-3, 3);
                if(strncmp(pFileType, "ifo", 3) == 0)
				   // ifoPathList.push_back(QString::fromUtf8(globs->gl_pathv[i])); /*!< store the .ifo file path of the dictionary */
					ifoPathList.push_front(QString::fromUtf8(globs->gl_pathv[i]));
				++nNum;
            }
        }
    }
    return nNum;
}


QStringList CScanDict::runScan(const char *pDictPath)
{
    ifoPathList.clear();
    char str[256];
    glob_t globs;
    // Check if the dictionary path has '/' at end. If not append it before scanning the dictionaries and globbing
    if (pDictPath[strlen(pDictPath) - 1] == '/')
        strcpy(str, pDictPath);
    else
        sprintf(str, "%s/", pDictPath);
    globs.gl_pathc = 0;
    scanDir(str, &globs);   /*!< Call the function to scan the path given by str and matching a given pattern using globbing */
    globfree(&globs);
    return ifoPathList;
}


bool CScanDict::loadBookname(QString ifoFileName, QString &bookname)
{
    std::string bName;
    std::string ifofilename= ifoFileName.toStdString();
    gchar *buffer;
    if (!g_file_get_contents(ifofilename.c_str(), &buffer, NULL, NULL))
            return false;

    #define DICT_MAGIC_DATA "StarDict's dict ifo file\nversion="
    const gchar *magic_data=DICT_MAGIC_DATA;
    if (!g_str_has_prefix(buffer, magic_data))
    {
        g_free(buffer);
        return false;
    }
    bool is_dict_300 = false;
    gchar *p1;
    p1 = buffer + sizeof(DICT_MAGIC_DATA) -1;
    #define DICT_VERSION_242 "2.4.2\n"
    #define DICT_VERSION_300 "3.0.0\n"
    if (g_str_has_prefix(p1, DICT_VERSION_242))
    {
        p1 += sizeof(DICT_VERSION_242) -2;
    }
    else if (g_str_has_prefix(p1, DICT_VERSION_300))
    {
        p1 += sizeof(DICT_VERSION_300) -2;
        is_dict_300 = true;
    }
    else
    {
        g_print("Load %s failed: Unknown version.\n", ifofilename.c_str());
        g_free(buffer);
        return false;
    }
    gchar *p2,*p3;
    if (is_dict_300)
    {
        p2 = strstr(p1,"\nidxoffsetbits=");
        if (p2)
        {
            p2 = p2 + sizeof("\nidxoffsetbits=") -1;
            if (g_str_has_prefix(p2, "64\n"))
            {
                // TODO
                g_print("Load %s failed: not supported presently.\n", ifofilename.c_str());
                g_free(buffer);
                return false;
            }
        }
    }
    p2 = strstr(p1,"\nbookname=");
    if (!p2)
    {
        g_free(buffer);
        return false;
    }
    p2 = p2 + sizeof("\nbookname=") -1;
    p3 = strchr(p2, '\n');
    bName.assign(p2, p3-p2);
    bookname=QString::fromUtf8(bName.c_str());
    g_free(buffer);
    return true;
}

