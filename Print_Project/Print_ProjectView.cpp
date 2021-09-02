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
#include "MainFrm.h"
#include "LineControl.h"

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
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_BUTTON_ALLERASE, &CPrintProjectView::OnButtonAllerase)
	ON_COMMAND(ID_BUTTON_RIGHTTRI, &CPrintProjectView::OnButtonRighttri)
	
	
	ON_WM_HSCROLL()
	ON_COMMAND(ID_BUTTON_LINECONTROL, &CPrintProjectView::OnButtonLinecontrol)
	ON_COMMAND(ID_BUTTON_LINECONTROL, &CPrintProjectView::OnButtonLinecontrol)
	ON_COMMAND(ID_BUTTON_ROUNDRECT, &CPrintProjectView::OnButtonRoundrect)
	ON_COMMAND(ID_BUTTON_PIERECT, &CPrintProjectView::OnButtonPierect)
	ON_COMMAND(ID_BUTTON_HALFCIRCLE, &CPrintProjectView::OnButtonHalfcircle)
	ON_COMMAND(ID_BUTTON_HALFCIRCLE2, &CPrintProjectView::OnButtonHalfcircle2)
	ON_COMMAND(ID_BUTTON_270PIERECT, &CPrintProjectView::OnButton270pierect)
END_MESSAGE_MAP()

// CPrintProjectView 생성/소멸

CPrintProjectView::CPrintProjectView() 
	: m_nDrawMode(0)
	//, m_nHatchStyle(0)
	, m_ptStart(0)
	, m_ptPrev(0)
	//, m_bFirst(false)
	, m_bLButtonDown(false)
	//, m_bContextMenu(false)
	, m_bHatch(false)
	, m_nCount(0)
	, m_nPenMode(0)
	, m_nPenSize(1)
{
	m_bContextMenu = true;
	m_bFirst = true;

	for (int i = 0; i < 100; i++)
	{
		m_ptData[i] = 0;
	}
	//같은 거 memset(m_ptData, 0, sizeof(CPoint) * 100);

	m_nHatchStyle = HS_CROSS;
	m_PenColor = RGB(0, 0, 0); //black
	m_BrushColor = RGB(255, 255, 255); //white

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

void CPrintProjectView::OnDraw(CDC* pDC)
{
	CPrintProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CPen pen, * oldpen;
	switch (m_nPenMode)
	{
	case 0:
		pen.CreatePen(PS_SOLID, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 1:
		pen.CreatePen(PS_DASH, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 2:
		pen.CreatePen(PS_DOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 3:
		pen.CreatePen(PS_DASHDOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 4:
		pen.CreatePen(PS_DASHDOTDOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	
	}

	oldpen = pDC->SelectObject(&pen);		//pen 객체 등록
	pDC->SetROP2(R2_COPYPEN);				//COPTPEN으로 설정

	CBrush brush, * oldbrush;
	if (m_bHatch)		//brush 객체 생성
		brush.CreateHatchBrush(m_nHatchStyle, m_BrushColor);
	else				//brush 객체 등록
		brush.CreateSolidBrush(m_BrushColor);
	oldbrush = pDC->SelectObject(&brush);
	CRect PieRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
	switch (m_nDrawMode)
	{
	case PENCIL_MODE:

			break;
	case LINE_MODE:
		pDC->MoveTo(m_ptStart);
		pDC->LineTo(m_ptPrev);
		break;
	case ELLIPSE_MODE:
		pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
		break;
	case RECTANGLE_MODE:
		pDC->Rectangle(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
		break;
		
	case ROUNDRECT_MODE:
		pDC->RoundRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y,50,50);
		break;

	case PIERECT_MODE:
		pDC->Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
		//pDC->Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
		break;
	case HALFCIRCLE_HORIZONTAL_MODE:
		pDC->Pie(PieRect, CPoint(m_ptPrev.x, PieRect.CenterPoint().y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
		//pDC->Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
		break;
	case HALFCIRCLE_VERTICAL_MODE:
		pDC->Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(PieRect.CenterPoint().x, m_ptPrev.y));
		break;
	case PIERECT270_MODE:
		pDC->Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
		break;
	case TRIANGLE_MODE:
		//pDC->MoveTo(m_ptStart.x,m_ptStart.y);
		//pDC->LineTo(m_ptPrev.x,m_ptPrev.y);
		
		//pDC->Polygon(m_ptData, m_nCount);
		break;

	case RIGHTTRIANGLE_MODE:
		//pDC->MoveTo(m_ptStart.x, m_ptStart.y);
		//pDC->LineTo(m_ptPrev.x, m_ptPrev.y);
		//pDC->Polygon(m_ptData, m_nCount);
		break;
	}
	pDC->SelectObject(oldpen);		//이전 pen으로 설정
	pDC->SelectObject(oldbrush);	//이전 brush로 설정
	pen.DeleteObject();				//pen 객체 삭제
	brush.DeleteObject();			//brush 객체 삭제

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





void CPrintProjectView::OnButtonColorfill()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlgColor;
	if (dlgColor.DoModal() == IDOK)
	{
		m_BrushColor = dlgColor.GetColor();
	}
	Invalidate(false);
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
	//CColorDialog dlg(m_BrushColor,CC_FULLOPEN);
	//CColorDialog dlg(m_PenColor, CC_FULLOPEN);
	//dlg.DoModal();
	//COLORREF m_BrushColor = dlg.GetColor();
	//COLORREF m_PenColor = dlg.GetColor();

	CColorDialog dlgColor;
	if (dlgColor.DoModal() == IDOK)
	{
		m_PenColor = dlgColor.GetColor();
	}
	Invalidate(false);
	
}


void CPrintProjectView::OnButtonUpsize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonDownsize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonSolid()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nPenMode = 0;
}


void CPrintProjectView::OnButtonDash()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nPenMode = 1;
}

void CPrintProjectView::OnButtonDot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nPenMode = 2;
}

void CPrintProjectView::OnButtonDashdot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nPenMode = 3;
}


void CPrintProjectView::OnButtonDashdotdot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nPenMode = 4;
}





void CPrintProjectView::OnButtonFigure()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CPrintProjectView::OnButtonLinestyle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPrintProjectView::OnButtonPencil()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 0;

	//스테이터스 바 표시
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("연필 그리기"));
}

void CPrintProjectView::OnButtonLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 1;

	//스테이터스 바 표시
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("직선 그리기"));
}


void CPrintProjectView::OnButtonCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 2;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("원 그리기"));
}
void CPrintProjectView::OnButtonRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 3;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("사각형 그리기"));
}


void CPrintProjectView::OnButtonRoundrect()
{
	m_nDrawMode = 7;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("둥근사각형 그리기"));
}


void CPrintProjectView::OnButtonEraser()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	

}

void CPrintProjectView::OnButtonTri()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 5;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("삼각형 그리기"));

}

void CPrintProjectView::OnButtonRighttri()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 6;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("직각삼각형 그리기"));

}

void CPrintProjectView::OnButtonPierect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 8;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("부채꼴 그리기"));
}


void CPrintProjectView::OnButtonHalfcircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 9;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("반원(가로) 그리기"));

}

void CPrintProjectView::OnButtonHalfcircle2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 10;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("반원(세로) 그리기"));

}


void CPrintProjectView::OnButton270pierect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = 11;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("270도 부채꼴 그리기"));
}


void CPrintProjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString strPoint;
	strPoint.Format(_T("마우스 위치 x:%d, y:%d"), point.x, point.y);


	//사용 위해 protected->public

	//======================================================
	CClientDC dc(this);							//클라이언트 객체 얻음
	CPen pen, * oldpen;
	switch (m_nPenMode)
	{
	case 0:
		pen.CreatePen(PS_SOLID, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 1:
		pen.CreatePen(PS_DASH, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 2:
		pen.CreatePen(PS_DOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 3:
		pen.CreatePen(PS_DASHDOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;
	case 4:
		pen.CreatePen(PS_DASHDOTDOT, m_nPenSize, m_PenColor);	//pen 객체 생성
		break;

	}	//Pen 객체 생성
	oldpen = dc.SelectObject(&pen);				//Pen 객체 등록

	dc.SetROP2(R2_NOTXORPEN);					//R2_NOTXORPEN으로 설정

	CBrush brush, * oldbrush;
	if (m_bHatch)
		brush.CreateHatchBrush(m_nHatchStyle, m_PenColor); //Hatch Brush 객체 생성
	else
		brush.CreateSolidBrush(m_PenColor);				 //Solid Brush 객체 생성

	oldbrush = dc.SelectObject(&brush);

	switch (m_nDrawMode)
	{
	case PENCIL_MODE:						//직선 그리기
		if (m_bLButtonDown)
		{
			
			
			dc.MoveTo(m_ptPrev);
			dc.LineTo(point);			//현재 직선 그림
			m_ptPrev = point;			//이전 점에 현재 점을 저장
		
		}
		break;

	case LINE_MODE:						//직선 그리기
		if (m_bLButtonDown)
		{
			dc.MoveTo(m_ptStart);
			dc.LineTo(m_ptPrev);		//이전 직선 지움
			dc.MoveTo(m_ptStart);
			dc.LineTo(point);			//현재 직선 그림
			m_ptPrev = point;			//이전 점에 현재 점을 저장
		}
		break;

	case ELLIPSE_MODE:					//원 그리기
		if (m_bLButtonDown)
		{
			dc.Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
			dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
			m_ptPrev = point;			//이전 점에 현재 점을 저장
		}
		break;

	case RECTANGLE_MODE:
		if (m_bLButtonDown)
		{
			
			dc.Rectangle(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
			dc.Rectangle(m_ptStart.x, m_ptStart.y, point.x, point.y);
			m_ptPrev = point;

		}
		break;
	case ROUNDRECT_MODE:
		if (m_bLButtonDown)
		{

			dc.RoundRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y,50,50);
			dc.RoundRect(m_ptStart.x, m_ptStart.y, point.x, point.y,50,50);
			m_ptPrev = point;

		}
		break;
	case PIERECT_MODE:
	if (m_bLButtonDown)
	{
		CRect PieRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
		dc.Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
		dc.Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
		//dc.Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
		//dc.Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
		//dc.Pie(PieRect, CPoint(PieRect.CenterPoint().x, PieRect.top), CPoint(point.x, point.y));
		m_ptPrev = point;

	}
	case PIERECT270_MODE:
		if (m_bLButtonDown)
		{
			CRect PieRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
			
			dc.Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
			dc.Pie(PieRect, CPoint(m_ptStart.x, PieRect.CenterPoint().y), CPoint(PieRect.CenterPoint().x, m_ptStart.y));
			
			m_ptPrev = point;

		}
	break;
	case HALFCIRCLE_HORIZONTAL_MODE:
		if (m_bLButtonDown)
		{
			CRect PieRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
			dc.Pie(PieRect, CPoint(m_ptPrev.x, PieRect.CenterPoint().y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
			dc.Pie(PieRect, CPoint(m_ptPrev.x, PieRect.CenterPoint().y), CPoint(m_ptStart.x, PieRect.CenterPoint().y));
	
			m_ptPrev = point;
		}
		break;
	case HALFCIRCLE_VERTICAL_MODE:
		if (m_bLButtonDown)
		{
			CRect PieRect(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
			dc.Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(PieRect.CenterPoint().x, m_ptPrev.y));
			dc.Pie(PieRect, CPoint(PieRect.CenterPoint().x, m_ptStart.y), CPoint(PieRect.CenterPoint().x, m_ptPrev.y));

			m_ptPrev = point;
		}
		break;
	case TRIANGLE_MODE:
		if (m_bLButtonDown)
		{
			//dc.MoveTo(m_ptStart.x, m_ptStart.y);
			//dc.LineTo(m_ptPrev.x, m_ptPrev.y);
			//dc.MoveTo(m_ptStart.x,m_ptStart.y);
			//dc.LineTo(point.x,point.y);			//현재 직선 그림
			POINT arPt1[4] = { {m_ptStart.x,m_ptStart.y},{(m_ptPrev.x - m_ptStart.x) / 2, m_ptPrev.y},{ m_ptPrev.x, m_ptPrev.y} };
			POINT arPt2[4] = { {m_ptStart.x,m_ptStart.y},{(point.x - m_ptStart.x) / 2,point.y},{point.x, point.y} };
			

			dc.Polygon(arPt1, 3);
			dc.Polygon(arPt2, 3);
			m_ptPrev = point;
		}
		break;

	case RIGHTTRIANGLE_MODE:
		if (m_bLButtonDown)
		{
			//dc.MoveTo(m_ptStart.x, m_ptStart.y);
			//dc.LineTo(m_ptPrev.x, m_ptPrev.y);
			//dc.MoveTo(m_ptStart.x,m_ptStart.y);
			//dc.LineTo(point.x,point.y);			//현재 직선 그림
			POINT arPt1[4] = { {m_ptStart.x,m_ptStart.y},{m_ptStart.x, m_ptPrev.y},{ m_ptPrev.x, m_ptPrev.y} };
			POINT arPt2[4] = { {m_ptStart.x,m_ptStart.y},{m_ptStart.x, point.y},{point.x, point.y} };


			dc.Polygon(arPt1, 3);
			dc.Polygon(arPt2, 3);
			m_ptPrev = point;
		}
		break;

	}

	pen.DeleteObject();					//pen 객체 삭제
	brush.DeleteObject();				//brush 객체 삭제

	CView::OnMouseMove(nFlags, point);
}


void CPrintProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (m_bFirst)
	{
		m_ptStart = m_ptPrev = 0;
		m_bLButtonDown = false;
		m_nCount = 0;
		for (int i = 0; i < 100; i++)
			m_ptData[i] = 0;
		Invalidate(false);		//화면갱신
	}

	switch (m_nDrawMode)
	{
	case PENCIL_MODE:
	case LINE_MODE:						//직선 그리기
	case ELLIPSE_MODE:					//원 그리기
	case RECTANGLE_MODE:
	case TRIANGLE_MODE:
	case RIGHTTRIANGLE_MODE:
	case ROUNDRECT_MODE:
	case PIERECT_MODE:
	case HALFCIRCLE_HORIZONTAL_MODE:
	case HALFCIRCLE_VERTICAL_MODE:
	case PIERECT270_MODE:
		m_bLButtonDown = true;			//왼쪽 버튼 눌림
		m_ptStart = m_ptPrev = point;	//시작 점과 이전 점에 현재 점을 저장
		m_bFirst = false;				//처음 그리는 것 -> false
		break;

	

	
	}

	//러버밴드 때문에 추가
	RECT rectClient;					//구조체 변수 선언
	SetCapture();						//마우스 캡처
	GetClientRect(&rectClient);			//클라이언트 영역 받음
	ClientToScreen(&rectClient);		//스크린 좌표계 변환
	::ClipCursor(&rectClient);			//마우스 이동 범위를 클라이언트 영역으로 제한

	CView::OnLButtonDown(nFlags, point);
}


void CPrintProjectView::OnLButtonUp(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLButtonDown)
	{
		if (m_nDrawMode == PENCIL_MODE || m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE || m_nDrawMode == RECTANGLE_MODE 
			||m_nDrawMode == TRIANGLE_MODE || m_nDrawMode == RIGHTTRIANGLE_MODE || m_nDrawMode == ROUNDRECT_MODE 
			|| m_nDrawMode == PIERECT_MODE || m_nDrawMode == HALFCIRCLE_HORIZONTAL_MODE || m_nDrawMode == HALFCIRCLE_VERTICAL_MODE
			|| m_nDrawMode == PIERECT270_MODE)
		{
			m_bLButtonDown = false;
			m_bFirst = true;
			ReleaseCapture();		//마우스 캡처 해제
			::ClipCursor(NULL);		//마우스 클립 해제
			Invalidate(false);		//화면 갱신
		}
	
		
	}
	
	CView::OnLButtonUp(nFlags, point);
}

void CPrintProjectView::OnUpdateLine(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->SetCheck(m_nDrawMode == LINE_MODE ? TRUE : FALSE);
	pCmdUI->SetCheck(m_nDrawMode == LINE_MODE ? 1 : 0);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("직선 그리기"));
}


void CPrintProjectView::OnUpdateEllipse(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? TRUE : FALSE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("원 그리기"));
}


void CPrintProjectView::OnUpdateRectangle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nDrawMode == RECTANGLE_MODE ? TRUE : FALSE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetWindowText(_T("사각형 그리기"));
}

void CPrintProjectView::OnUpdateEraser(CCmdUI* pCmdUI)
{
	
}


void CPrintProjectView::OnUpdateSolid(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nPenMode == SOLID_MODE ? 1 : 0);
}


void CPrintProjectView::OnUpdateDash(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nPenMode == DASH_MODE ? 1 : 0);
}


void CPrintProjectView::OnUpdateDot(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nPenMode == DOT_MODE ? 1 : 0);
}


void CPrintProjectView::OnUpdateDashdot(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nPenMode == DASHDOT_MODE ? 1 : 0);
}


void CPrintProjectView::OnUpdateDashdotdot(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nPenMode == DASHDOTDOT_MODE ? 1 : 0);
}





void CPrintProjectView::OnButtonAllerase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Invalidate(TRUE);
}







void CPrintProjectView::OnButtonLinecontrol()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	LineControl dlg;
	
	if (dlg.DoModal() == IDOK)
	{
		m_nPenSize = dlg.m_StrSize;

	}
	Invalidate(false);
}









