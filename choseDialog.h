#pragma once


// choseDialog 对话框

class choseDialog : public CDialog
{
	DECLARE_DYNAMIC(choseDialog)

public:
	choseDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~choseDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit etAddress;
	afx_msg void OnBnClickedCancel2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	BOOL EnablePrivilege();
	BOOL inject(DWORD ProcessID, PCWSTR pszLibFile);
};
