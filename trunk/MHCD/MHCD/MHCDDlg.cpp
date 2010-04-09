// MHCDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MHCD.h"
#include "MHCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MYWM_NOTIFYICON WM_USER+100
#define   WM_MYMESSAGE   WM_USER+101 
BOOL MyTaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip) ;
BOOL MyTaskBarDeleteIcon(HWND hwnd, UINT uID) ;
int EnableDebugPriv(const char * name);
DWORD FindTarget( LPCTSTR lpszProcess );
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMHCDDlg �Ի���




CMHCDDlg::CMHCDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMHCDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMHCDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, CListMsg);
}

BEGIN_MESSAGE_MAP(CMHCDDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LISTCLEAR, &CMHCDDlg::OnBnClickedButtonListclear)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMHCDDlg::OnBnClickedButtonStart)
//	ON_WM_TIMER()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_SHOWHELP, &CMHCDDlg::OnBnClickedButtonShowhelp)
	ON_BN_CLICKED(IDC_BUTTON_SHOWABOUT, &CMHCDDlg::OnBnClickedButtonShowabout)
	ON_MESSAGE(MYWM_NOTIFYICON, &CMHCDDlg::OnMyIconNotify)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMHCDDlg ��Ϣ�������

BOOL CMHCDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CreateMutex(NULL,false,"MHCD by suwuhao");     
	EnableDebugPriv(SE_DEBUG_NAME);
	MyTaskBarAddIcon(this->GetSafeHwnd(),IDR_MAINFRAME,m_hIcon,"MapHackClickDetector by suwuhao");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMHCDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ( nID==SC_MINIMIZE  )
	{
		CDialog::OnSysCommand(nID, lParam);
		ShowWindow( SW_HIDE );
	}
	else if(nID == SC_CLOSE)
	{
		if(FindTarget("war3.exe"))
			AfxMessageBox("�ȹر�WAR3");
		else CDialog::OnSysCommand(nID, lParam);
	}
	else CDialog::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMHCDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMHCDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMHCDDlg::OnBnClickedButtonListclear()
{
	CListMsg.ResetContent();
}


int EnableDebugPriv(const char * name)//����Ȩ����
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    //�򿪽������ƻ�
    if(!OpenProcessToken(GetCurrentProcess(),
    TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,
        &hToken))
    {
        MessageBox(NULL,(LPCSTR)"OpenProcessToken Error!",(LPCSTR)"Error!",MB_OK);
            return 1;
    }
    //��ý��̱���ΨһID
    if(!LookupPrivilegeValue(NULL,name,&luid))
    {
        MessageBox(NULL,(LPCSTR)"LookupPrivivlegeValue Error!",(LPCSTR)"Error",MB_OK);
    }
    tp.PrivilegeCount=1;
    tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
    tp.Privileges[0].Luid=luid;
    //����Ȩ��
    if(!AdjustTokenPrivileges(hToken,0,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
    {
        MessageBox(NULL,(LPCSTR)"AdjustTokenPrivileges Error!",(LPCSTR)"Error",MB_OK);
        return 1;
    }
    return 0;
}

DWORD FindTarget( LPCTSTR lpszProcess )
{
    DWORD dwRet = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    Process32First( hSnapshot, &pe32 );
    do
    {
        if ( lstrcmpi( pe32.szExeFile, lpszProcess ) == 0 )
        {
            dwRet = pe32.th32ProcessID;
            break;
        }
    } while ( Process32Next( hSnapshot, &pe32 ) );
    CloseHandle( hSnapshot );
    return dwRet;
}

BOOL RemoteLoadLibrary( DWORD dwProcessID, LPCSTR lpszDll )
{
    // ��Ŀ�����
    HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessID );
    
	if(!hProcess)
	{
		AfxMessageBox( (LPCTSTR)"OpenProcessʧ�ܡ�");
		return FALSE;
	}
	// ��Ŀ����̵�ַ�ռ�д��DLL����
    DWORD dwSize, dwWritten;
    dwSize = lstrlenA( lpszDll ) + 1;
    LPVOID lpBuf = VirtualAllocEx( hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE );
    if ( NULL == lpBuf )
    {
		AfxMessageBox( (LPCTSTR)"VirtualAllocExʧ��1��");
        CloseHandle( hProcess );
        return FALSE;
    }
    if ( WriteProcessMemory( hProcess, lpBuf, (LPVOID)lpszDll, dwSize, &dwWritten ) )
    {
        // Ҫд���ֽ�����ʵ��д���ֽ�������ȣ�����ʧ��
        if ( dwWritten != dwSize )
        {
			AfxMessageBox( (LPCTSTR)"VirtualAllocExʧ��2��");
            VirtualFreeEx( hProcess, lpBuf, dwSize, MEM_DECOMMIT );
            CloseHandle( hProcess );
            return FALSE;
        }
    }
    else
    {
		AfxMessageBox( (LPCTSTR)"WriteProcessMemoryʧ�ܡ�");
        CloseHandle( hProcess );
        return FALSE;
    }
    // ʹĿ����̵���LoadLibrary������DLL
    DWORD dwID;
    LPVOID pFunc = LoadLibraryA;
    HANDLE hThread = CreateRemoteThread( hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, lpBuf, 0, &dwID );
    // �ȴ�LoadLibrary�������
    //WaitForSingleObject( hThread, INFINITE );
    // �ͷ�Ŀ�����������Ŀռ�
    //VirtualFreeEx( hProcess, lpBuf, dwSize, MEM_DECOMMIT );
    CloseHandle( hThread );
    CloseHandle( hProcess );
    return TRUE;
}

static DWORD dwProcessID;
HANDLE f_;
char * pInfo;
char lastInfo = 100;

void CMHCDDlg::OnBnClickedButtonStart()
{
	char szTarget[MAX_PATH]="war3.exe";

	dwProcessID = FindTarget( (LPCTSTR)szTarget );
	if ( 0 == dwProcessID )
	{
		AfxMessageBox( (LPCTSTR)"�Ҳ���Ŀ����̡�");
		//return;
	}
	else if ( !RemoteLoadLibrary( dwProcessID, (LPCSTR)"MHCD.dll" ) )
	{
		AfxMessageBox( (LPCTSTR)"Զ��DLL����ʧ�ܡ�");
		//return;
	}
	else
	{
		CListMsg.ResetContent();
	}

}

BOOL CMHCDDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if(pCopyDataStruct -> dwData == 0x33)
	{
		CListMsg.AddString((char *)(pCopyDataStruct -> lpData));
		CListMsg.SetTopIndex(CListMsg.GetCount() - 1);
	}

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CMHCDDlg::OnBnClickedButtonShowhelp()
{
	CListMsg.AddString("��exe��dll����ħ��Ŀ¼����ÿ��������Ϸ����ʼ���Ϳ����ˡ�");
	CListMsg.AddString("Win7/Vista��ͬѧ������Ǵ�ƽ̨����ħ�޵Ļ����Թ���ԱȨ������ħ�ޡ�");
	CListMsg.AddString("�����������κθ��˺����岻��ʹ�ô˳���ӯ�������߲��е��κ���ʹ�ñ����������һ�к��");
	CListMsg.SetTopIndex(CListMsg.GetCount() - 1);
}

void CMHCDDlg::OnBnClickedButtonShowabout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


BOOL MyTaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip) 
{ 
    BOOL res; 
	NOTIFYICONDATA tnid; 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
	tnid.hWnd = hwnd; 
    tnid.uID = uID;     
	tnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE; 
    tnid.uCallbackMessage = MYWM_NOTIFYICON; 
	tnid.hIcon = hicon; 
    if (lpszTip) 
lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip)); 
    else         
		tnid.szTip[0] = '\0';  
    res = Shell_NotifyIcon(NIM_ADD, &tnid); 
	if (hicon) 
        DestroyIcon(hicon); 
	return res; 
} 

BOOL MyTaskBarDeleteIcon(HWND hwnd, UINT uID) 
{	
	BOOL res; 
    NOTIFYICONDATA tnid; 
	tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = hwnd;
	tnid.uID = uID; 
    res = Shell_NotifyIcon(NIM_DELETE, &tnid); 
	return res; 
}  

afx_msg LRESULT CMHCDDlg::OnMyIconNotify(WPARAM wParam, LPARAM lParam)
{
	UINT nID;
	UINT uMouseMsg; 
	nID = (UINT)wParam;
    uMouseMsg = (UINT) lParam; 
	if (uMouseMsg == WM_LBUTTONDOWN) //���ͼ��
	{ 
	ShowWindow( SW_SHOWNORMAL );
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);	
	}
	return 0;
}
void CMHCDDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	MyTaskBarDeleteIcon(GetSafeHwnd(),IDR_MAINFRAME);
}
