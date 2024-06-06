// ChildFrm.h : interface of the CChildFrame class
//

#include "ChildFrm.h"

#pragma once


class CCombinedChildFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CCombinedChildFrame)
public:
	CCombinedChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CCombinedChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

};
