
#pragma once

#define GRAPHVIEW_CLASSNAME _T("MyOwnControl")
#include <iostream>
#include <list>
using namespace std;
// MyControl

typedef class Off_set
{
public:
	int offNum;
	int Mode;
};
class MyControl : public CWnd
{
	DECLARE_DYNAMIC(MyControl)

public:
	MyControl();
	virtual ~MyControl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CRect re;
	CButton m_Open, m_Write;
	CEdit m_Path;
	CEdit m_Exif[8];
	

	//metadata file info
	CString strTitle, strModel, strDataTimeDigitized, strDescript, strPath, strStar;

	CString post;		// tag
	int titleMode, modelMode, desMode, rateMode;

	//offset
	CString strOff, off_value;
	list<Off_set> m_offset;
	list<Off_set> ::iterator iter;
	BOOL IsOffSt;
	BOOL IsClkWrite;
	FILE* fp, *fp_move;
public:
	static BOOL RegisterWindowClass();
	afx_msg void OnBnClickedOpen();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	BOOL ReadMetadata(CString filePath);
	BOOL CheckPoint(char check, int len);
	void GetOffset(char buff, int len);
};

