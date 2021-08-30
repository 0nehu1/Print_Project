// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해
// 추가적으로 제공되는 내용입니다.
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// Print_ProjectView.cpp: CPrintProjectView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Print_Project.h"
#endif

#include "Print_ProjectDoc.h"
#include "Print_ProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPrintProjectView

IMPLEMENT_DYNCREATE(CPrintProjectView, CView)

BEGIN_MESSAGE_MAP(CPrintProjectView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPrintProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_SIZECTRL, &CPrintProjectView::OnButtonSizectrl)
	ON_COMMAND(ID_BUTTON_ROTATE, &CPrintProjectView::OnButtonRotate)
	ON_COMMAND(ID_BUTTON_PENCIL, &CPrintProjectView::OnButtonPencil)
	ON_COMMAND(ID_BUTTON_COLORFILL, &CPrintProjectView::OnButtonColorfill)
	ON_COMMAND(ID_BUTTON_ERASER, &CPrintProjectView::OnButtonEraser)
	
	ON_COMMAND(ID_BUTTON_COLOR, &CPrintProjectView::OnButtonColor)
	ON_COMMAND(ID_BUTTON_UPSIZE, &CPrintProjectView::OnButtonUpsize)
	ON_COMMAND(ID_BUTTON_DOWNSIZE, &CPrintProjectView::OnButtonDownsize)
	ON_COMMAND(ID_BUTTON_DASH, &CPrintProjectView::OnButtonDash)
	ON_COMMAND(ID_BUTTON_DASHDOT, &CPrintProjectView::OnButtonDashdot)
	ON_COMMAND(ID_BUTTON_DASHDOTDOT, &CPrintProjectView::OnButtonDashdotdot)
	ON_COMMAND(ID_BUTTON_DOT, &CPrintProjectView::OnButtonDot)
	ON_COMMAND(ID_BUTTON_FIGURE, &CPrintProjectView::OnButtonFigure)
	ON_COMMAND(ID_BUTTON_LINE, &CPrintProjectView::OnButtonLine)
	ON_COMMAND(ID_BUTTON_LINESTYLE, &CPrintProjectView::OnButtonLinestyle)
	ON_COMMAND(ID_BUTTON_RECT, &CPrintProjectView::OnButtonRect)
	ON_COMMAND(ID_BUTTON_SOLID, &CPrintProjectView::OnButtonSolid)
	ON_COMMAND(ID_BUTTON_TRI, &CPrintProjectView::OnButtonTri)
	ON_COMMAND(ID_BUTTON_CIRCLE, &CPrintProjectView::OnButtonCircle)
END_MESSAGE_MAP()

// CPrintProjectView 생성/소멸

CPrintProjectView::CPrintProjectView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CPrintProjectView::~CPrintProjectView()
{
}

BOOL CPrintProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CPrintProjectView 그리기

void CPrintProjectView::OnDraw(CDC* /*pDC*/)
{
	CPrintProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CPrintProjectView 인쇄


void CPrintProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPrintProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CPrintProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CPrintProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CPrintProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPrintProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPrintProjectView 진단

#ifdef _DEBUG
void CPrintProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPrintProjectDoc* CPrintProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrintProjectDoc)));
	return (CPrintProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CPrintProjectView 메시지 처리기


void CPrintProjectView::OnButtonSizectrl()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonPencil()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonColorfill()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonEraser()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnComboFigure()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnComboLinestyle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


}


void CPrintProjectView::OnButtonColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlg(m_BrushColor,CC_FULLOPEN);
	dlg.DoModal();
	COLORREF m_BrushColor = dlg.GetColor();
}


void CPrintProjectView::OnButtonUpsize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDownsize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDash()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDashdot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDashdotdot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonFigure()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonLinestyle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonSolid()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonTri()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
