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

// Print_ProjectView.h: CPrintProjectView 클래스의 인터페이스
//

#pragma once
enum DRAW_MODE { PENCIL_MODE,LINE_MODE, ELLIPSE_MODE, RECTANGLE_MODE , ERASER_MODE
	, TRIANGLE_MODE, RIGHTTRIANGLE_MODE, ROUNDRECT_MODE,PIERECT_MODE,HALFCIRCLE_HORIZONTAL_MODE, HALFCIRCLE_VERTICAL_MODE
	, PIERECT270_MODE};
enum PEN_MODE { SOLID_MODE, DASH_MODE, DOT_MODE, DASHDOT_MODE, DASHDOTDOT_MODE };

class CPrintProjectView : public CView
{
protected: // serialization에서만 만들어집니다.
	CPrintProjectView();
	DECLARE_DYNCREATE(CPrintProjectView)

// 특성입니다.
public:
	CPrintProjectDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CPrintProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonSizectrl();
	afx_msg void OnButtonRotate();
	afx_msg void OnButtonPencil();
	afx_msg void OnButtonColorfill();
	afx_msg void OnButtonEraser();
	afx_msg void OnComboFigure();
	afx_msg void OnComboLinestyle();
	afx_msg void OnButtonColor();
	afx_msg void OnButtonUpsize();
	afx_msg void OnButtonDownsize();
	
	afx_msg void OnButtonDash();
	afx_msg void OnButtonDashdot();
	afx_msg void OnButtonDashdotdot();
	afx_msg void OnButtonDot();
	afx_msg void OnButtonFigure();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonLinestyle();
	afx_msg void OnButtonRect();
	afx_msg void OnButtonSolid();
	afx_msg void OnButtonTri();
	afx_msg void OnButtonCircle();
	CPoint m_ptStart;
	CPoint m_ptPrev;
	bool m_bFirst;
	bool m_bLButtonDown;
	bool m_bContextMenu;
	bool m_bHatch;
	CPoint m_ptData[100];
	int m_nCount;
	COLORREF m_PenColor;
	COLORREF m_BrushColor;
	int m_nPenMode;
	int m_nDrawMode;
	int m_nHatchStyle;
	int m_nPenSize;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEraser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDash(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDashdot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDashdotdot(CCmdUI* pCmdUI);

	afx_msg void OnButtonFigurecolor();
	afx_msg void OnButtonAllerase();
	afx_msg void OnButtonRighttri();
	afx_msg void OnButtonUndo();

	afx_msg void OnButtonLinecontrol();
	afx_msg void OnButtonRoundrect();
	afx_msg void OnButtonPierect();
	afx_msg void OnButtonHalfcircle();
	afx_msg void OnButtonHalfcircle2();
	afx_msg void OnButton270pierect();
	afx_msg void OnUpdateButton270pierect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDash(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDashdot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDashdotdot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEraser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonHalfcircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonHalfcircle2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonPencil(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonPierect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRighttri(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRoundrect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonTri(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLinestyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonFigure(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLinecontrol(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // Print_ProjectView.cpp의 디버그 버전
inline CPrintProjectDoc* CPrintProjectView::GetDocument() const
   { return reinterpret_cast<CPrintProjectDoc*>(m_pDocument); }
#endif

