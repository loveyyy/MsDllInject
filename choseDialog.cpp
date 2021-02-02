// choseDialog.cpp: 实现文件
//

#include "pch.h"
#include "DllInject.h"
#include "choseDialog.h"
#include "afxdialogex.h"
#include "DllInject.h"


// choseDialog 对话框

IMPLEMENT_DYNAMIC(choseDialog, CDialog)

choseDialog::choseDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

choseDialog::~choseDialog()
{
	
}

void choseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, etAddress);
}


BEGIN_MESSAGE_MAP(choseDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL2, &choseDialog::OnBnClickedCancel2)
	ON_BN_CLICKED(IDOK, &choseDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &choseDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// choseDialog 消息处理程序


//选择文件
void choseDialog::OnBnClickedCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");

	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
		_T("Describe Files (*.dll)|*.dll|All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	etAddress.SetWindowTextW(strFile);

}

//开始注入
void choseDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	/*GetDlgItem(IDD_DLLINJECT_DIALOG)->GetWindowText(ProcessID);*/

	CDllInjectApp* pApp = (CDllInjectApp*)AfxGetApp();
	
	UpdateData(FALSE);

	EnablePrivilege();

	CString cs;
	etAddress.GetWindowTextW(cs);
	if (inject(pApp->ProcessID, cs)) {
		AfxMessageBox(L"注入成功");
	}
	else {
		DWORD d = GetLastError();
		AfxMessageBox(L"注入失败"+ d);
	}


}

//关闭
void choseDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	choseDialog::EndDialog(0);
}

//提升程序的权限
BOOL choseDialog::EnablePrivilege()
{
	HANDLE hObject;
	LUID Luid;
	TOKEN_PRIVILEGES NewStatus;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES | TOKEN_READ, &hObject))
		return FALSE;
	if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid))
	{
		NewStatus.Privileges[0].Luid = Luid;
		NewStatus.PrivilegeCount = 1;
		NewStatus.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hObject, FALSE, &NewStatus, 0, 0, 0);
		CloseHandle(hObject);
		return TRUE;
	}
	return FALSE;
}




BOOL choseDialog::inject(DWORD ProcessID, PCWSTR pszLibFile)
{

	HANDLE hProcess = NULL, hThread = NULL;

	PWSTR pszLibFileRemote = NULL;
	//打开进程 
		hProcess = OpenProcess(
			PROCESS_ALL_ACCESS, 
			FALSE, ProcessID);
		if (hProcess == NULL) {
			return false;
		}

		int cch = 1 + lstrlenW(pszLibFile);
		int cb = cch * sizeof(WCHAR);

		//分配远程进程的内存
		pszLibFileRemote = (PWSTR)
			VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if (pszLibFileRemote == NULL) {
			return false;
		}

		//写入进程
		if (!WriteProcessMemory(hProcess, pszLibFileRemote,
			(PVOID)pszLibFile, cb, NULL)) {
			return false;
		}
		//活的加载dll的函数地址
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		if (pfnThreadRtn == NULL) {
			return false;
		}
		// Create a remote thread that calls LoadLibraryW(DLLPathname)
		//创建线程 并且调用loadLibrarty 函数 传递分配的地址
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			pfnThreadRtn, pszLibFileRemote, 0, NULL);
		if (hThread == NULL) {
			return false;
		}
		// Wait for the remote thread to terminate
		//等待线程
		WaitForSingleObject(hThread, INFINITE);

		if (pszLibFileRemote != NULL)
			//释放内存
			VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);

		if (hThread != NULL)
			//关闭线程
			CloseHandle(hThread);

		if (hProcess != NULL)
			//关闭进程
			CloseHandle(hProcess);

		return true;
}

