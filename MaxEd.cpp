// MaxEd.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MaxEd.h"
#include "MainFrm.h"

#include "MaxEdDoc.h"
#include "MaxEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//***********************************************
char * __cdecl ftoa(double value, int count, char* buffer)
{
	int decimal, sign;
	char* buf = ::fcvt(value, count, &decimal, &sign), *ptr = buffer;

	if(sign)
		*ptr++ = '-';
	while(decimal--)
		*ptr++ = *buf++;
	*ptr++ = '.';
	while(*ptr++ = *buf++)
		;
	return buffer;
}
//***********************************************


// CMaxEdApp

BEGIN_MESSAGE_MAP(CMaxEdApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMaxEdApp construction

CMaxEdApp::CMaxEdApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMaxEdApp object

CMaxEdApp theApp;

// CMaxEdApp initialization

BOOL CMaxEdApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Load standard INI file options (including MRU)
	LoadStdProfileSettings(4);

	// Init environiment directory patches
	::GetCurrentDirectory(_MAX_EDITORENV_PATH, EDITORENV_ROOT_PATH);
	::_tcscat(::_tcscpy(EDITORENV_TEXTURES_PATH, EDITORENV_ROOT_PATH), _T("\\textures"));
	::_tcscat(::_tcscpy(EDITORENV_MODELS_PATH,	EDITORENV_ROOT_PATH), _T("\\models"));
	::_tcscat(::_tcscpy(EDITORENV_SOUND_PATH,	EDITORENV_ROOT_PATH), _T("\\sound"));
	::_tcscat(::_tcscpy(EDITORENV_MUSIC_PATH,	EDITORENV_ROOT_PATH), _T("\\music"));
	::_tcscat(::_tcscpy(EDITORENV_SPRITES_PATH, EDITORENV_ROOT_PATH), _T("\\sprites"));
	::_tcscat(::_tcscpy(EDITORENV_ENTITY_PATH,	EDITORENV_ROOT_PATH), _T("\\entity"));
	::_tcscat(::_tcscpy(EDITORENV_PREFERENCES_PATH, EDITORENV_ROOT_PATH), _T("\\preferences"));
	::_tcscat(::_tcscpy(EDITORENV_SHADER_PATH,	EDITORENV_ROOT_PATH), _T("\\shaders"));
	::_tcscat(::_tcscpy(EDITORENV_SAVE_PATH,	EDITORENV_ROOT_PATH), _T("\\save"));
	::_tcscat(::_tcscpy(EDITORENV_UTILS_PATH,	EDITORENV_ROOT_PATH), _T("\\utils"));
	::_tcscat(::_tcscpy(EDITORENV_TEMP_PATH,	EDITORENV_ROOT_PATH), _T("\\temp"));
	::_tcscat(::_tcscpy(EDITORENV_DEFAULT_PATH, EDITORENV_ROOT_PATH), _T("\\default"));

	::_tcscat(::_tcscpy(EDITORENV_SETTINGS_PATH, EDITORENV_ROOT_PATH), _T("\\settings.ini"));
	::_tcscat(::_tcscpy(EDITORENV_ENTITYDEF_PATH, EDITORENV_ROOT_PATH), _T("\\entities.def"));

	// Load additional INI file options
	::LoadPreferencesFromIni( EDITORENV_SETTINGS_PATH );

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMaxEdDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMaxEdView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// App command to run the dialog
void CMaxEdApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void LoadPreferencesFromIni(const TCHAR* inifile)
{
	TCHAR szBuffer[256];

	// init general section
	::GetPrivateProfileString("GENERAL", "EnableUndo", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bEnableUndo = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "UndoLevels", "10", szBuffer, 256, inifile);
	PREFERENCES::Current.nUndoStackDepth = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "UseGlobalClearColor", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bUseGlobalClearColor = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "GlobalClearColor", "8421504", szBuffer, 256, inifile);
	PREFERENCES::Current.clrGlobalClearColor = ::atoi(szBuffer);
//	::GetPrivateProfileString("GENERAL", "ColorScheme", "0", szBuffer, 256, inifile);
//	PREFERENCES::Current.nUndoStackDepth = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "SnapToGrid", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bSnapToGrid = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "ShowWindowLabel", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bShowWindowLabel = ::atoi(szBuffer);
	PREFERENCES::Current.cLabelColor = PREFERENCES::Default.cLabelColor;
	::GetPrivateProfileString("GENERAL", "ShowCoordSpaceIcon", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bShowCoordSpaceIcon = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "ShowActiveWindowRect", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bShowActiveWindowRect = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "ForceCenteredWhenScaleMode", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.bForceCenteredWhenScaleMode = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "TextureLock", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bTextureLock = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "ShowWorldAxis", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bShowWorldAxis = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "EnableCubicClipping", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.bEnableCubicClipping = ::atoi(szBuffer);
	::GetPrivateProfileString("GENERAL", "EnableOpenGLLighting", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.bEnableOpenGLLighting = ::atoi(szBuffer);

	// init window layout
	::GetPrivateProfileString("VIEWPORT", "CurrentLayout", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.eWindowLayout = (WINDOWLAYOUT)::atoi(szBuffer);
	PREFERENCES::Current.nNumEditWindow = WINDOWNUM[PREFERENCES::Current.eWindowLayout];

	// init major grid
	::GetPrivateProfileString("MAJORGRID", "Show", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.bShow = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "Dotted", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.bDotted = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "Stippled", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.bStipple = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "Width", "1.0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.fWidth = ::atof(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "Step", "16", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.nStep = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "Color", "6842472", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.cColor = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "StippleFactor", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.nStippleFactor = ::atoi(szBuffer);
	::GetPrivateProfileString("MAJORGRID", "StippleMask", "65535", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Major.uStippleMask = ::atoi(szBuffer);

	// init minor grid
	::GetPrivateProfileString("MINORGRID", "Show", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.bShow = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "Dotted", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.bDotted = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "Stippled", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.bStipple = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "Width", "1.0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.fWidth = ::atof(szBuffer);
	::GetPrivateProfileString("MINORGRID", "Step", "128", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.nStep = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "Color", "5263440", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.cColor = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "StippleFactor", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.nStippleFactor = ::atoi(szBuffer);
	::GetPrivateProfileString("MINORGRID", "StippleMask", "65535", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Minor.uStippleMask = ::atoi(szBuffer);

	// init axis grid
	::GetPrivateProfileString("AXISGRID", "Show", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.bShow = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "Dotted", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.bDotted = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "Stippled", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.bStipple = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "Width", "1.0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.fWidth = ::atof(szBuffer);
	::GetPrivateProfileString("AXISGRID", "Step", "16", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.nDotStep = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "Color", "0", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.cColor = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "StippleFactor", "1", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.nStippleFactor = ::atoi(szBuffer);
	::GetPrivateProfileString("AXISGRID", "StippleMask", "65535", szBuffer, 256, inifile);
	PREFERENCES::Current.sGrid.Axis.uStippleMask = ::atoi(szBuffer);
}

void SavePreferencesToIni(const TCHAR* inifile)
{
	TCHAR szBuffer[256];

	// init general section
	::itoa(PREFERENCES::Current.bEnableUndo, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "EnableUndo", szBuffer, inifile);
	::itoa(PREFERENCES::Current.nUndoStackDepth, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "UndoLevels", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bUseGlobalClearColor, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "UseGlobalClearColor", szBuffer, inifile);
	::itoa((COLORREF)PREFERENCES::Current.clrGlobalClearColor, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "GlobalClearColor", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bSnapToGrid, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "SnapToGrid", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bShowWindowLabel, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "ShowWindowLabel", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bShowCoordSpaceIcon, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "ShowCoordSpaceIcon", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bShowActiveWindowRect, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "ShowActiveWindowRect", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bForceCenteredWhenScaleMode, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "ForceCenteredWhenScaleMode", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bTextureLock, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "TextureLock", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bShowWorldAxis, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "ShowWorldAxis", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bEnableCubicClipping, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "EnableCubicClipping", szBuffer, inifile);
	::itoa(PREFERENCES::Current.bEnableOpenGLLighting, szBuffer, 10);
	::WritePrivateProfileString("GENERAL", "EnableOpenGLLighting", szBuffer, inifile);

	// init window layout
	::itoa(PREFERENCES::Current.eWindowLayout, szBuffer, 10);
	VERIFY(::WritePrivateProfileString("VIEWPORT", "CurrentLayout", szBuffer, inifile));

	// init major grid
	::itoa(PREFERENCES::Current.sGrid.Major.bShow, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "Show", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.bDotted, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "Dotted", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.bStipple, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "Stippled", szBuffer, inifile);
	::ftoa(PREFERENCES::Current.sGrid.Major.fWidth, 1, szBuffer);
	::WritePrivateProfileString("MAJORGRID", "Width", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.nStep, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "Step", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.cColor, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "Color", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.nStippleFactor, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "StippleFactor", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Major.uStippleMask, szBuffer, 10);
	::WritePrivateProfileString("MAJORGRID", "StippleMask", szBuffer, inifile);

	// init minor grid
	::itoa(PREFERENCES::Current.sGrid.Minor.bShow, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "Show", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.bDotted, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "Dotted", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.bStipple, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "Stippled", szBuffer, inifile);
	::ftoa(PREFERENCES::Current.sGrid.Minor.fWidth, 1, szBuffer);
	::WritePrivateProfileString("MINORGRID", "Width", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.nStep, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "Step", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.cColor, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "Color", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.nStippleFactor, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "StippleFactor", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Minor.uStippleMask, szBuffer, 10);
	::WritePrivateProfileString("MINORGRID", "StippleMask", szBuffer, inifile);

	// init axis grid
	::itoa(PREFERENCES::Current.sGrid.Axis.bShow, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "Show", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.bDotted, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "Dotted", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.bStipple, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "Stippled", szBuffer, inifile);
	::ftoa(PREFERENCES::Current.sGrid.Axis.fWidth, 1, szBuffer);
	::WritePrivateProfileString("AXISGRID", "Width", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.nDotStep, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "Step", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.cColor, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "Color", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.nStippleFactor, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "StippleFactor", szBuffer, inifile);
	::itoa(PREFERENCES::Current.sGrid.Axis.uStippleMask, szBuffer, 10);
	::WritePrivateProfileString("AXISGRID", "StippleMask", szBuffer, inifile);
}


// CMaxEdApp message handlers
