#pragma once
#include <QtGui>

namespace commonDlg
{
	/*
	CustomTextEdit is used to limit the max line count.
	Constraint: the format of QTextEdit only supports setFont 
	and LineSpace dynamic property
	*/
	class CustomTextEdit : public QTextEdit
	{
		Q_OBJECT
			int mMaxLine;
	public:
		CustomTextEdit(QWidget* parent = NULL);
		bool eventFilter(QObject *o, QEvent *e);
		QList<int> GetLineCountList();
		int getMaxLine() const {return mMaxLine;}
		void setMaxLine(int maxLine) {mMaxLine = maxLine;}
	protected:
		void mousePressEvent(QMouseEvent *e);

	};
}
