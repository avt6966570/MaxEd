================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : MaxEd Project Overview
===============================================================================

The application wizard has created this MaxEd application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your MaxEd application.

MaxEd.vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

MaxEd.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CMaxEdApp application class.

MaxEd.cpp
    This is the main application source file that contains the application
    class CMaxEdApp.

MaxEd.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\MaxEd.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file MaxEd.rc.

res\MaxEd.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
MaxEd.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.
/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.
MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.
res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in MaxEd.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

MaxEdDoc.h, MaxEdDoc.cpp - the document
    These files contain your CMaxEdDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CMaxEdDoc::Serialize).
    The Document will have the following strings:
        File extension:      mem
        File type ID:        MaxEd.Document
        Main frame caption:  MaxEd
        Doc type name:       MaxEd
        Filter name:         MaxEd Maps (*.mem)
        File new short name: MaxEd
        File type long name: MaxEd.Document
MaxEdView.h, MaxEdView.cpp - the view of the document
    These files contain your CMaxEdView class.
    CMaxEdView objects are used to view CMaxEdDoc objects.
/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named MaxEd.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources MFC70XXX.DLL from the Microsoft
Visual C++ CD-ROM under the Win\System directory to your computer's system or 
system32 directory, and rename it to be MFCLOC.DLL.  ("XXX" stands for the 
language abbreviation.  For example, MFC70DEU.DLL contains resources 
translated to German.)  If you don't do this, some of the UI elements of 
your application will remain in the language of the operating system.

/////////////////////////////////////////////////////////////////////////////



Rendering Level:
Smooth + Highlights, Smooth, Facets + Highlights, Facets, Lit Wireframes, Wireframe, Bounding Box [Edged Faces]

Transparency:
None, Stipple, Blend

Apply To:
Active Viewport Only, All Viewports, All but Active

Rendering Options:
Disable View, Disable Textures, Texture Correction,
Z-buffer Wirefrane Objects, Force 2-Sided, Default Lightning [1 Light, 2 Light],
Shade Selected Faces, Display Selected with Edged Faces,
Viewport Clipping, Fast view Nth Faces: 5

Prespective User View:
Field of View: 45.0




//////////////////////////////////////////////////////////////////////////////

void CXYWnd::HandleDrop()
{
  if (g_PrefsDlg.m_bRightClick == false)
    return;
  if (!m_mnuDrop.GetSafeHmenu()) // first time, load it up
  {
    m_mnuDrop.CreatePopupMenu();
    int nID = ID_ENTITY_START;

    CMenu* pChild2 = new CMenu;
    pChild2->CreateMenu();
    pChild2->AppendMenu(MF_STRING, ID_SELECTION_SELECTCOMPLETETALL, "Select Complete Tall");
    pChild2->AppendMenu(MF_STRING, ID_SELECTION_SELECTTOUCHING, "Select Touching");
    pChild2->AppendMenu(MF_STRING, ID_SELECTION_SELECTPARTIALTALL, "Select Partial Tall");
    pChild2->AppendMenu(MF_STRING, ID_SELECTION_SELECTINSIDE, "Select Inside");
    m_mnuDrop.AppendMenu(MF_POPUP, reinterpret_cast<unsigned int>(pChild2->GetSafeHmenu()), "Select");
    m_mnuDrop.AppendMenu(MF_SEPARATOR, nID++, "");

    CMenu* pChild3 = new CMenu;
    pChild3->CreateMenu();
    pChild3->AppendMenu(MF_STRING, ID_DROP_GROUP_ADDTO, "Add to...");
    pChild3->AppendMenu(MF_STRING, ID_DROP_GROUP_REMOVE, "Remove");
    pChild3->AppendMenu(MF_STRING, ID_DROP_GROUP_NAME, "Name...");
    pChild3->AppendMenu(MF_SEPARATOR, nID++, "");
    pChild3->AppendMenu(MF_STRING, ID_DROP_GROUP_NEWGROUP, "New Group...");
    m_mnuDrop.AppendMenu(MF_POPUP, reinterpret_cast<unsigned int>(pChild3->GetSafeHmenu()), "Group");
    m_mnuDrop.AppendMenu(MF_SEPARATOR, nID++, "");

    m_mnuDrop.AppendMenu(MF_STRING, ID_SELECTION_UNGROUPENTITY, "Ungroup Entity");
    m_mnuDrop.AppendMenu(MF_STRING, ID_SELECTION_MAKE_DETAIL, "Make Detail");
    m_mnuDrop.AppendMenu(MF_STRING, ID_SELECTION_MAKE_STRUCTURAL, "Make Structural");
    m_mnuDrop.AppendMenu(MF_SEPARATOR, nID++, "");

    CMenu* pChild = new CMenu;
    pChild->CreateMenu();
    pChild->AppendMenu(MF_STRING, nID++, "Smart_Train");
    //pChild->AppendMenu(MF_STRING, nID++, "Smart_Rotating");
    m_mnuDrop.AppendMenu(MF_POPUP, reinterpret_cast<unsigned int>(pChild->GetSafeHmenu()), "Smart Entities");
    m_mnuDrop.AppendMenu(MF_SEPARATOR, nID++, "");

    pChild = NULL;
	  eclass_t	*e;
    CString strActive;
    CString strLast;
    CString strName;
	  for (e=eclass ; e ; e=e->next)
    {
      strLast = strName;
      strName = e->name;
      int n_ = strName.Find("_");
      if (n_ > 0)
      {
        CString strLeft = strName.Left(n_);
        CString strRight = strName.Right(strName.GetLength() - n_ - 1);
        if (strLeft == strActive) // this is a child
        {
          ASSERT(pChild);
          pChild->AppendMenu(MF_STRING, nID++, strName);
        }
        else
        {
          if (pChild)
          {
            m_mnuDrop.AppendMenu(MF_POPUP, reinterpret_cast<unsigned int>(pChild->GetSafeHmenu()), strActive);
            g_ptrMenus.Add(pChild);
            //pChild->DestroyMenu();
            //delete pChild;
            pChild = NULL;
          }
          strActive = strLeft;
          pChild = new CMenu;
          pChild->CreateMenu();
          pChild->AppendMenu(MF_STRING, nID++, strName);
        }
      }
      else
      {
        if (pChild)
        {
          m_mnuDrop.AppendMenu(MF_POPUP, reinterpret_cast<unsigned int>(pChild->GetSafeHmenu()), strActive);
          g_ptrMenus.Add(pChild);
          //pChild->DestroyMenu();
          //delete pChild;
          pChild = NULL;
        }
        strActive = "";
        m_mnuDrop.AppendMenu(MF_STRING, nID++, strName);
      }
    }
  }

  CPoint ptMouse;
  GetCursorPos(&ptMouse);
  m_mnuDrop.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, this);
}