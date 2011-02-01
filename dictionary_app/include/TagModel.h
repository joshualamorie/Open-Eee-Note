/**************************************************
copyright:asus
author:paul_zhang@asus.com.cn
**************************************************/
#pragma once
#include "ManageData.h"
#include <QPair>

namespace commonDlg
{
	struct SearchOption
	{
		SortByType SortType;
		OrderByType OrderType;
		UserTagsType TagType;
		QString      SearchString;
		SearchOption()
			: SortType(Name), OrderType(ASC), TagType(AllTag){}
	};

	//TODO: add namespace
	class TagModel : public QObject
	{
		//Q_OBJECT
		struct PageData
		{
			int BeginIndex;
			int EndIndex;
			PageData(int beginIndex, int endIndex)
				: BeginIndex(beginIndex), EndIndex(endIndex){}
			inline bool isValid(){return BeginIndex >= 0;}
		};

		class PageLayout
		{
			QList<int> mTagCountList;
			int mCurrentIndex;
			QPair<int, int> mCurrentPos;
			int mRowSize;
			int mColumnSize;
			int mPageCount;
		public:
			PageLayout(QList<int> tagCountList, int rowSize, int columnSize);
			PageData CreatePage();
			int getPageCount() const {return mPageCount;}
		};

		QList<PageData > mPageDataList;
		SearchOption mSearchOption;
		int mRowCount;
		int mColumnCount;
	public:
		TagModel(SearchOption searchOption, int rowCount, int columCount, QObject* parent );
		~TagModel();
		SearchOption getSearchOption() const {return mSearchOption;}
		void setSearchOpation(const SearchOption& searchOption);
		inline int getPageCount() const {return mPageDataList.count();}
		virtual QList<SearchTag> pageData(int colum) const;
	protected:
		void Init();
	};

}