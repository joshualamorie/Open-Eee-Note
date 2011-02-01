/*
  Copyright (C) 2010 ASUSTeK Computer Inc. All rights reserved.
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <list>
#include <map>
#include <string>


/**
 * This class can load/save common configuration files.
 * Each line of the file must be in the format:
 * 
 * \<key\>=\<value\>
 * 
 * If keys in the file are duplicated, the later one will overwrite the older one.
 */
class CConfig
{
public:
	CConfig();

	~CConfig();

	/**
	 * Load a configuration file.
         * If call this function more than once, new items will be appended.
	 * @param pPath Path of the configuration file.
	 * @return True if load succesfully.
	 */
	bool Load(const char *pPath);

	/**
	 * Clear all items.
	 */
	void Reset(void);

	/**
	 * Save the configuration file to the path of last Load or SaveAs function.
	 * Do not call this function before calling Load or SaveAs functions.
	 */
	void Save(void);

	/**
	 * Save the configuration file to a new path.
	 * @param pPath Path of the configuration file.
	 */
	void SaveAs(const char *pPath);

	/**
	 * Get the binary value of the specified key.
	 * @param pName Name of the key.
	 * @param pValue The buffer to store the value.
	 * @param nSize The capacity of pValue. If the size of binary value is bigger than nSize, only nSize bytes will be stored.
	 * @return Number of bytes stored in pValue.
	 */
	int GetBinary(const char *pName, void *pValue, int nSize);

	/**
	 * Get the integer value of the specified key.
	 * @param pName Name of the key.
	 * @param nDefault The default value if specified key doesn't exist.
	 * @return The integer value of the specified key or default value if key doesn't exist.
	 */
	int GetInt(const char *pName, int nDefault = 0);

	/**
	 * Get the array of integer values of the specified key.
	 * @param pName Name of the key.
	 * @param pArray An array to store integer values.
	 * @param nSize Capacity of pArray.
	 * @return Number of integer values stored in pArray.
	 */
	int GetIntArray(const char *pName, int *pArray, int nSize);

	/**
	 * Get the list of integer values of the specified key.
	 * @param pName Name of the key.
	 * @param pList A list to store integer values.
	 * @return Number of integer values stored in pList.
	 */
	int GetIntList(const char *pName, std::list<int> *pList);

	/**
	 * Get the long integer value of the specified key.
	 * @param pName Name of the key.
	 * @param nDefault The default value if specified key doesn't exist.
	 * @return The long integer value of the specified key or default value if key doesn't exist.
	 */
	long GetLong(const char *pName, long nDefault = 0);

	/**
	 * Get the string value of the specified key.
	 * @param pName Name of the key.
	 * @param pDefault The default value if specified key doesn't exist.
	 * @return The string value of the specified key or NULL if key doesn't exist.
	 */
	const char *GetStr(const char *pName, const char *pDefault = NULL);

	/**
	 * Set binary value to the specified key.
	 * @param pName Name of the key.
	 * @param pValue Binary value to be set.
	 * @param nSize Number of bytes in pValue.
	 */
	void SetBinary(const char *pName, const void *pValue, int nSize);

	/**
	 * Set integer value to the specified key.
	 * @param pName Name of the key.
	 * @param nValue Integer value to be set.
	 */
	void SetInt(const char *pName, int nValue);

	/**
	 * Set list of integer values to the specified key.
	 * @param pName Name of the key.
	 * @param pList List of integer values to be set.
	 */
	void SetIntList(const char *pName, std::list<int> *pList);

	/**
	 * Set long integer value to the specified key.
	 * @param pName Name of the key.
	 * @param nValue Long integer value to be set.
	 */
	void SetLong(const char *pName, long nValue);

	/**
	 * Set string value to the specified key.
	 * @param pName Name of the key.
	 * @param pValue String value to be set, this value can be NULL.
	 */
	void SetStr(const char *pName, const char *pValue);

private:
	char								*m_pPath;
	std::map<std::string, std::string>	m_Map;
	bool								m_bUpdated;

	const char	*GetValue(const char *pName);
	void		SetValue(const char *pName, const char *pValue);
};

#endif
