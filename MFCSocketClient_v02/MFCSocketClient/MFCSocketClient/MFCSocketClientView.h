
// MFCSocketClientView.h : interface of the CMFCSocketClientView class
//

#pragma once


class CMFCSocketClientView : public CView
{
protected: // create from serialization only
	CMFCSocketClientView() noexcept;
	DECLARE_DYNCREATE(CMFCSocketClientView)

// Attributes
public:
	CMFCSocketClientDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCSocketClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClient();
	afx_msg void OnMenuPage1();
};

#ifndef _DEBUG  // debug version in MFCSocketClientView.cpp
inline CMFCSocketClientDoc* CMFCSocketClientView::GetDocument() const
   { return reinterpret_cast<CMFCSocketClientDoc*>(m_pDocument); }
#endif

