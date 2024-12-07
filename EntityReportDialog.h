#pragma once
#include "afxwin.h"


// CEntityReportDialog dialog

class CEntityReportDialog : public CDialog
{
	DECLARE_DYNAMIC(CEntityReportDialog)

public:
	CEntityReportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEntityReportDialog();

// Dialog Data
	enum { IDD = IDD_ENTITYREPORTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedMarkbutton();
	afx_msg void OnBnClickedGotobutton();
	afx_msg void OnBnClickedDeletebutton();
	afx_msg void OnBnClickedPropertiesbutton();
	afx_msg void OnBnClickedBykeyvaluecheck();
	afx_msg void OnBnClickedByclasscheck();


private:
	// do entity list-box
	void AddEntityToList(const CString& name, ENTITY* pOwner);
	void UpdateEntityList();

	// filter

	// get entity pointer
	void* GetEntityFromList() const
		{ int nSelPos = m_cEntityList.GetCurSel();
		  return nSelPos != LB_ERR ? m_cEntityList.GetItemDataPtr(nSelPos) : 0; }


private:
	// entity name list box (automatically sorted)
	CListBox m_cEntityList;
	// brush filter: everything/solid/point?
	int m_nBrushFilter;
	// include hidden objects?
	BOOL m_bIncludeHidden;
	// filter entity by key/value?
	BOOL m_bByKeyValue;
	// exact value match?
	BOOL m_bExactMatch;
	// key string
	CString m_strKey;
	// value string
	CString m_strValue;
	// filter by class name?
	BOOL m_bByClass;
	// entity class name combo list
	CComboBox m_cClassList;
	// current selected string in combo (entity class name)
	CString m_strClassName;
public:
	afx_msg void OnCbnSelchangeByclasscombo();
};