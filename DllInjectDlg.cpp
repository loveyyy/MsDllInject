
// DllInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DllInject.h"
#include "DllInjectDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <TlHelp32.h> 
#include "choseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDllInjectDlg 对话框
CDllInjectDlg::CDllInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLLINJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDllInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, List);
}

BEGIN_MESSAGE_MAP(CDllInjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDllInjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDllInjectDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDllInjectDlg 消息处理程序

BOOL CDllInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//获取系统进程

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf("CreateToolHelp32Snamshot调用失败!\n");
		return -1;
	}

	//遍历进程快照，显示每个进程的信息
	PROCESSENTRY32  pe32;
	pe32.dwSize = sizeof(pe32);
	BOOL bNext = Process32First(hProcessSnap, &pe32);
	List.DeleteAllItems();
	List.InsertColumn(0, L"名称");
	List.SetColumnWidth(0, 200);
	List.InsertColumn(1, L"进程ID");
	List.SetColumnWidth(1, 200);
	List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	while (bNext)
	{
		int n = List.InsertItem(0, pe32.szExeFile);
		TCHAR str[20];
		wsprintf(str, L"%d", pe32.th32ProcessID);
		List.SetItemText(n,1, str);
		bNext = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDllInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDllInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CDllInjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnInitDialog();
}


void CDllInjectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION  pos  = List.GetFirstSelectedItemPosition();
	int nItem;
	if (pos == NULL) {
		AfxMessageBox(L"尚未选中");
	}
	else {
		while (pos)
		{
			nItem = List.GetNextSelectedItem(pos);
			UpdateData(true);
			CDllInjectApp* pApp = (CDllInjectApp*)AfxGetApp();
			CString cp  = List.GetItemText(nItem, 1);
			pApp->ProcessID = _tcstoul(cp, NULL, 10);
			choseDialog dialog;
			dialog.DoModal();
		}
	}
	
}

