// MyControl.cpp: 구현 파일
//


#include "pch.h"
#include "FileRW.h"
#include "MyControl.h"


// MyControl

IMPLEMENT_DYNAMIC(MyControl, CWnd)

MyControl::MyControl()
{
	strTitle = _T("");
	strModel = _T("");
	strDataTimeDigitized = _T("");
	strDescript = _T("");
	strPath = _T("");
	strStar = _T("");

	IsClkWrite = FALSE;
	titleMode = 0;
	modelMode = 0;
	desMode = 0;
	IsOffSt = FALSE;
}

MyControl::~MyControl()
{
}


BEGIN_MESSAGE_MAP(MyControl, CWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTONOPEN, OnBnClickedOpen)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// MyControl 메시지 처리기

BOOL MyControl::RegisterWindowClass()
{
	WNDCLASS    wndcls;
	HINSTANCE   hInst = AfxGetInstanceHandle();
	MSG Message;
	// 윈도우 클래스 등록되었는지 확인
	if (!(::GetClassInfo(hInst, GRAPHVIEW_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS /*| CS_HREDRAW | CS_VREDRAW*/;
		wndcls.lpfnWndProc = ::DefWindowProc;//	 WndProc;	// 무엇
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = NULL;
		wndcls.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = GRAPHVIEW_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
		AfxRegisterClass(&wndcls);		//registerclass와의 차이?d
	}
	CWnd* m_Main = AfxGetMainWnd();
	HWND hWnd = CreateWindow(GRAPHVIEW_CLASSNAME, NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInst, NULL);

	return AfxRegisterClass(&wndcls);
}



int MyControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void MyControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.

	GetClientRect(&re);

	// file open/ close
	CString str(_T("file Path"));
	if (!m_Open.GetSafeHwnd())
	{
		m_Open.Create(_T("open"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(re.left+220, re.top, re.left+270, re.top + 20)
			, this, IDC_BUTTONOPEN);
	}
	if (!m_Path.GetSafeHwnd())
	{
		m_Path.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(re.left+70, re.top, re.left +210, re.top + 20)
			, this, IDC_EDIT_PATH);
		m_Path.SetWindowText(_T(""));

	}
	dc.TextOut(re.left, re.top, str);

	for (int j = 0; j < 5; j+=4)
	{
		CString arr[4];
		arr[0] = strTitle;
		arr[1] = strModel;
		arr[2] = strDataTimeDigitized;
		arr[3] = strDescript;

		CString name[4];
		name[0] = _T("title");
		name[1] = _T("model");
		name[2] = _T("date, time");
		name[3] = _T("description");

		int k = 0;
		int i;
		for (i = 0 + j; i < 3 + j; i++)
		{
			if (!m_Exif[i].GetSafeHwnd())
			{
				m_Exif[i].Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL
					, CRect(re.left + 80 + (re.right*j/8), re.top + 50 * (k + 1), re.left + 350+(re.right * j / 8), re.top + 50 * (k + 1) + 20), this, IDC_EDIT_EXIF1 + i);
			}
			m_Exif[i].SetWindowText(arr[k]);
			dc.TextOut(re.left + (re.right / 2) * (j / 4), re.top + 50 * (k + 1), name[k]);
			k++;
		}
		if (!m_Exif[i].GetSafeHwnd())
		{
			m_Exif[i].Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL
				, CRect(re.left + 80 + (re.right * j / 8), re.top + 50 * (k + 1), re.left + 350 + (re.right * j / 8), re.top + 50 * (k + 1) + 150), this, IDC_EDIT_EXIF1 + i);
		}
		m_Exif[i].SetWindowText(arr[k]);
		dc.TextOut(re.left + (re.right / 2) * (j / 4), re.top + 50 * (k + 1), name[k]);
	}
	
	/*
	if (!m_Write.GetSafeHwnd())
	{
		m_Write.Create(_T("write"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(re.right - 60, re.top + 120, re.right, re.top + 140)
			, this, IDC_BUTTONWRITE);
	}
	*/
	m_Exif[6].EnableWindow(FALSE);
	m_Exif[2].EnableWindow(FALSE);
	//m_Comtext.ReplaceSel(arr[3]);
				   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
}

void MyControl::OnBnClickedOpen()
{
	UpdateData(TRUE);

	CString str;
	GetDlgItemText(IDC_EDIT_PATH, strPath);

	if (!ReadMetadata(strPath))
		return;
	
	Invalidate(TRUE);

}

/*
void MyControl::OnBnClickedWrite()
{
	UpdateData(TRUE);
	CString str;
	GetDlgItemText(IDC_EDIT_PATH, strPath);

	WriteJPEGFileMetadata(strPath);
	//CopyMetaData(f_name, strPath);

	CoUninitialize();

}
*/

BOOL MyControl::ReadMetadata(CString filePath)
{
	// TODO: 여기에 구현 코드 추가.
	char fileP[100];
	memset(fileP, 0, 100);
	memcpy(fileP, (char*)(LPCWSTR)filePath, filePath.GetLength()*2);
	
	int j = 0;
	for (int k = 0; k < filePath.GetLength() * 2; k++)
	{
		if (fileP[k] == '\0')
			continue;
		fileP[j] = fileP[k];
		j++;
	}
	fileP[j] = '\0';

	//file read
	int err=fopen_s(&fp, fileP, "rb");
	if (err != 0)
	{
		CString erro(_T("fail to open"));
		AfxMessageBox(erro);
		return FALSE;
	}
	
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//aloocate buffer
	char* buff = (char*)malloc(len+1);
	if (!buff)
	{
		CString err(_T("memory error"));
		AfxMessageBox(err);
		fclose(fp);
	}
	fread(buff, len, sizeof(char), fp);

	int k = 0;
	CString temp(_T(""));
	while (k < len)
	{
		
		CString ch = _T("");
		ch.Format(_T("%02X"), (char)(buff[k]));

		CheckPoint(buff[k], k);
		GetOffset(buff[k], k);
		

		temp += ch;

		k++;
	}
	AfxMessageBox(temp);
	fclose(fp);


	if (IsClkWrite)
	{
		fopen_s(&fp_move, "temp.jpg", "wb");

		fwrite(buff, sizeof(buff), len, fp_move);
		fclose(fp_move);
	}
	
	Invalidate(TRUE);
}



BOOL MyControl::CheckPoint(char check, int len)
{
	// TODO: 여기에 구현 코드 추가.
	CString tep = _T("");
	tep.Format(_T("%02X"), check);

	post += tep;
	
	//title block

	if (post==_T("FFFFFFFFFFFFFFD8"))	// soi
	{
		CString f(_T("It's jpg file"));
		AfxMessageBox(f);
		return FALSE;
	}
	else if (post == _T("010E") || (titleMode>=1 && titleMode<= 10))	// title
	{

		if (titleMode >= 7 && titleMode <= 8)
		{
			CString t = _T("");
			if (titleMode == 7)
			{
				strTitle.Format(_T("%c"), check);
			}
			else
			{
				t.Format(_T("%c"), check);
				strTitle += t;
			}
			
		}
		else if (titleMode >= 9 && titleMode <= 10)
		{
			if (titleMode == 9)
			{
				off_value = tep;
			}
			else
			{
				off_value += tep;

				char* pszTemp = new char[off_value.GetLength() + 1];
				strcpy_s(pszTemp, off_value.GetLength() + 1, CT2A(off_value));
				int offset = strtol(pszTemp, NULL, 16);
				off_value = (_T(""));

				Off_set tOff;
				tOff.offNum = offset + 30;
				tOff.Mode = TITLE_MODE;
				m_offset.push_back(tOff);
				titleMode = 0;
				delete[]pszTemp;
				return TRUE;

			}
		}
		titleMode++;
	}
	else if (post == _T("0110") || (modelMode >= 1 && modelMode <= 10))		// model
	{
		if (modelMode >= 7 && modelMode <= 8)
		{
			CString t=_T("");
			if (modelMode == 7)
			{
				strTitle.Format(_T("%c"), check);
			}
			else
			{
				t.Format(_T("%c"), check);
				strModel += t;
			
			}

		}
		else if (modelMode >= 9 && modelMode <= 10)
		{
			if (modelMode == 9)
			{
				off_value = tep;
			}
			else
			{
				off_value += tep;

				char* pszTemp = new char[off_value.GetLength() + 1];
				strcpy_s(pszTemp, off_value.GetLength() + 1, CT2A(off_value));
				int offset = strtol(pszTemp, NULL, 16);
				off_value = (_T(""));

				Off_set mOff;
				mOff.offNum = offset + 30;
				mOff.Mode = MODEL_MODE;
				m_offset.push_back(mOff);
				modelMode = 0;
				delete[]pszTemp;
				return TRUE;

			}
		}
		modelMode++;
	}
	else if (post == _T("FFFFFF9CFFFFFF9C") || (desMode >= 1 && desMode <= 10))		// description
	{
		if (desMode >= 7 && desMode <= 8)
		{
			CString t = _T("");
			if (desMode == 7)
			{
				strTitle.Format(_T("%c"), check);
			}
			else
			{
				t.Format(_T("%c"), check);
				strDescript += t;
				
			}

		}
		else if (desMode >= 9 && desMode <= 10)
		{
			if (desMode == 9)
			{
				off_value = tep;
			}
			else
			{
				off_value += tep;

				char* pszTemp = new char[off_value.GetLength() + 1];
				strcpy_s(pszTemp, off_value.GetLength() + 1, CT2A(off_value));
				int offset = strtol(pszTemp, NULL, 16);
				off_value = (_T(""));

				Off_set mOff;
				mOff.offNum = offset + 30;
				mOff.Mode = DESCRIPT_MODE;
				m_offset.push_back(mOff);
				desMode = 0;
				delete[]pszTemp;
				return TRUE;

			}
		}
		desMode++;
	}
	if (post == _T("FFFFFF47FFFFFF49") || (rateMode >= 1 && rateMode <= 10))		// rate
	{
		if (rateMode >= 7 && rateMode <= 8)
		{
			CString t = _T("");
			if (rateMode == 8)
			{
				t.Format(_T("%c"), check);

				char* Tm = new char[t.GetLength() + 1];
				strcpy_s(Tm, t.GetLength() + 1, CT2A(t));
				int star = strtol(Tm, NULL, 16);

				if (star == 99)
					star = 100;
				star /= 25;
				star += 1;
				strStar.Format(_T("%d"), star);
			}

		}
		rateMode++;
	}
	
	post = _T("");
	post = tep;
	return FALSE;
}


void MyControl::GetOffset(char buff, int len)
{
	// TODO: 여기에 구현 코드 추가.
	int size = m_offset.size();

	if (!IsOffSt)
	{
		iter = m_offset.begin();

		for (int i = 0; i < size; i++)
		{
			if (iter->offNum == len)
			{
				IsOffSt = TRUE;
				strOff = _T("");	// initialize offset string
				break;
			}


			if (i == (size - 1))
				break;
			iter++;
		}
	}
	

	if (IsOffSt)
	{
		CString te = _T("");

		switch (iter->Mode)
		{
		case TITLE_MODE:
			te.Format(_T("%c"), buff);
			strOff += te;
			if (buff == '\0')
				strTitle = strOff;
			break;
		case MODEL_MODE:
			te.Format(_T("%c"), buff);
			strOff += te;
			if (buff == '\0')
				strModel = strOff;
			break;
		case DESCRIPT_MODE:
			if ((len - iter->offNum) % 2 == 0)
			{
				te.Format(_T("%c"), buff);
				strOff += te;
				if (buff == '\0')
				{
					strDescript = strOff;
					IsOffSt = FALSE;

				}
			}
			break;
		default:
			break;
		}
		if (buff == '\0' && iter->Mode != DESCRIPT_MODE)
		{
			IsOffSt = FALSE;

		}
	}
}


