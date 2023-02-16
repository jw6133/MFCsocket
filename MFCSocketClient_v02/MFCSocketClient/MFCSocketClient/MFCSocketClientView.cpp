
// MFCSocketClientView.cpp : implementation of the CMFCSocketClientView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCSocketClient.h"
#endif

#include "MFCSocketClientDoc.h"
#include "MFCSocketClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CClientDlg.h"
#include "Attend.h"


// CMFCSocketClientView

IMPLEMENT_DYNCREATE(CMFCSocketClientView, CView)

BEGIN_MESSAGE_MAP(CMFCSocketClientView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_MENU_CLIENT, &CMFCSocketClientView::OnMenuClient)
	ON_COMMAND(ID_MENU_PAGE1, &CMFCSocketClientView::OnMenuPage1)
END_MESSAGE_MAP()

// CMFCSocketClientView construction/destruction

CMFCSocketClientView::CMFCSocketClientView() noexcept
{
	// TODO: add construction code here

}

CMFCSocketClientView::~CMFCSocketClientView()
{
}

BOOL CMFCSocketClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCSocketClientView drawing

void CMFCSocketClientView::OnDraw(CDC* /*pDC*/)
{
	CMFCSocketClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMFCSocketClientView printing

BOOL CMFCSocketClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCSocketClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCSocketClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCSocketClientView diagnostics

#ifdef _DEBUG
void CMFCSocketClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSocketClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSocketClientDoc* CMFCSocketClientView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSocketClientDoc)));
	return (CMFCSocketClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCSocketClientView message handlers


void CMFCSocketClientView::OnMenuClient()
{
	CClientDlg dlg;
	if (dlg.DoModal()== IDOK) {
		
	}
}


void CMFCSocketClientView::OnMenuPage1()
{
	Attend dlg;
	if (dlg.DoModal() == IDOK) {

	}
}
