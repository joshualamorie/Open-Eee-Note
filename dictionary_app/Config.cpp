/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#include "Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

static int HtoD(char c)
{
	if (c <= '9')
		return c - '0';
	else if (c <= 'F')
		return c - 'A' + 10;
	else
		return c - 'a' + 10;

}

static int ParseInt(const char *str)
{
	int i, n;

	if (*str != '\0')
		if (str[0] == '0' && str[1] == 'x')  //hexadecimal
		{
			n = 0;
			for (i = 2; str[i] != '\0'; i++)
				n = (n << 4) | HtoD(str[i]);
			return n;
		}
	return atoi(str);
}

CConfig::CConfig()
{
	m_pPath = NULL;
	m_bUpdated = false;
}

CConfig::~CConfig()
{
	delete[] m_pPath;
}

bool CConfig::Load(const char *pPath)
{
        char str[1024], *p;
	FILE *f;

	delete[] m_pPath;
	m_pPath =NULL;
	m_pPath = new char[strlen(pPath) + 1];
	strcpy(m_pPath, pPath);
	m_bUpdated = false;

	f = fopen(m_pPath, "rt");
	if (f == NULL)
		return false;

	while (fgets(str, sizeof(str), f))  //read a line
	{
		if (*str == '#')  //comment
			continue;

		p = str + strlen(str) - 1;
                while (*p == '\r' || *p == '\n')  //remove new line character
			*p-- = '\0';
		p = strchr(str, '=');
		if (p == NULL)
			continue;
		*p++ = '\0';
		m_Map.erase(str);
                m_Map.insert(make_pair(str, p));
	}
	fclose(f);
	return true;
}

void CConfig::Reset(void)
{
	m_Map.clear();
	delete[] m_pPath;
	m_pPath = NULL;
	m_bUpdated = false;
}

void CConfig::Save(void)
{
	FILE *f;
	map<string, string>::iterator it;

	if (!m_bUpdated)
		return;

	f = fopen(m_pPath, "wt");
	if (f != NULL)
	{
		for (it = m_Map.begin(); it != m_Map.end(); ++it)
			fprintf(f, "%s=%s\n", it->first.c_str(), it->second.c_str());
		fclose(f);
	}
	m_bUpdated = false;
}

void CConfig::SaveAs(const char *pPath)
{
	delete[] m_pPath;
	m_pPath = new char[strlen(pPath) + 1];
	strcpy(m_pPath, pPath);
	m_bUpdated = true;
	Save();
}

int CConfig::GetBinary(const char *pName, void *pValue, int nSize)
{
	const char *str;
	int i, nLen;

	str = GetValue(pName);
	if (str == NULL)
		nSize = 0;
	else
	{
		nLen = strlen(str);
		if (nSize > nLen / 2)
			nSize = nLen / 2;
		for (i = 0; i < nSize; i++)
			((unsigned char *)pValue)[i] = HtoD(str[i * 2]) << 4 | HtoD(str[i * 2 + 1]);
	}
	return nSize;
}

int CConfig::GetInt(const char *pName, int nDefault)
{
	const char *pValue;

	pValue = GetValue(pName);
	return pValue == NULL? nDefault : ParseInt(pValue);
}

int CConfig::GetIntArray(const char *pName, int *pArray, int nSize)
{
	const char *pValue;
	char *str, *token;
	int i;

	pValue = GetValue(pName);
	if (pValue == NULL)
		return 0;

	i = 0;
	str = new char[strlen(pValue) + 1];
	strcpy(str, pValue);
	token = strtok(str, " ");
	while (token && i < nSize)
	{
		pArray[i++] = ParseInt(token);
		token = strtok(NULL, " ");
	}
	delete[] str;

	return i;
}

int CConfig::GetIntList(const char *pName, list<int> *pList)
{
	const char *pValue;
	char *str, *token;
	int nSize;

	pValue = GetValue(pName);
	if (pValue == NULL)
		return 0;

	nSize = pList->size();
	str = new char[strlen(pValue) + 1];
	strcpy(str, pValue);
	token = strtok(str, " ");
	while (token)
	{
		pList->push_back(ParseInt(token));
		token = strtok(NULL, " ");
	}
	delete[] str;

	return pList->size() - nSize;
}

long CConfig::GetLong(const char *pName, long nDefault)
{
	const char *pValue;

	pValue = GetValue(pName);
	return pValue == NULL? nDefault : atol(pValue);
}

const char *CConfig::GetStr(const char *pName, const char *pDefault)
{
	const char *pValue;

	pValue = GetValue(pName);
	if (pValue == NULL)
		return pDefault;
	return *pValue == '\0'? pDefault : pValue;
}

void CConfig::SetBinary(const char *pName, const void *pValue, int nSize)
{
	char *str, *p;
	int i;

	if (pValue == NULL)
		SetValue(pName, NULL);
	else
	{
		str = new char[nSize * 2 + 1];
		p = str;
		for (i = 0; i < nSize; i++)
		{
			sprintf(p, "%02X", ((unsigned char *)pValue)[i]);
			p += 2;
		}
		*p = '\0';
		SetValue(pName, str);
		delete[] str;
	}
}

void CConfig::SetInt(const char *pName, int nValue)
{
	char str[15];

	sprintf(str, "%d", nValue);
	SetValue(pName, str);
}

void CConfig::SetIntList(const char *pName, list<int> *pList)
{
	char str[1024], *token;
	list<int>::iterator it;

	str[0] = str[1] = '\0';
	token = str;
	for (it = pList->begin(); it != pList->end(); ++it)
	{
		sprintf(token, " %d", *it);
		token = token + strlen(token);
	}
	SetValue(pName, str + 1);
}

void CConfig::SetLong(const char *pName, long nValue)
{
	char str[15];

	sprintf(str, "%ld", nValue);
	SetValue(pName, str);
}

void CConfig::SetStr(const char *pName, const char *pValue)
{
	SetValue(pName, pValue);
}

const char *CConfig::GetValue(const char *pName)
{
	map<string, string>::iterator it;

	it = m_Map.find(pName);
	if (it == m_Map.end())
		return NULL;
	return it->second.c_str();
}

void CConfig::SetValue(const char *pName, const char *pValue)
{
	m_Map.erase(pName);
	if (pValue != NULL)
		m_Map.insert(make_pair(pName, pValue));
	m_bUpdated = true;
}
