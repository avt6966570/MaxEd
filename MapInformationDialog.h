#pragma once

// CMapInformationDialog dialog

class CMapInformationDialog : public CDialog
{
	DECLARE_DYNAMIC(CMapInformationDialog)

public:
	CMapInformationDialog(CWnd* pParent = NULL);   // standard constructor
	CMapInformationDialog(UINT uSolids, UINT uFaces,
						  UINT uPointEntities, UINT uSolidEntities,
						  UINT uUniqueTextures, UINT uTextureMemory,
						  const CStringArray* parTexPathes,
						  CWnd* pParent = NULL);
	virtual ~CMapInformationDialog();

// Dialog Data
	enum { IDD = IDD_MAPINFORMATIONDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	UINT m_uSolids;
	UINT m_uFaces;
	UINT m_uPointEntities;
	UINT m_uSolidEntities;
	UINT m_uUniqueTextures;
	UINT m_uTextureMemory;
	const CStringArray* m_parTexPathes;
};