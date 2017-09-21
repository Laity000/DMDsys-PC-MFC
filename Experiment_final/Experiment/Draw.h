#pragma once
#include "atltypes.h"


// Draw 对话框

class Draw : public CDialog
{
	DECLARE_DYNAMIC(Draw)

public:
	Draw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Draw();
	//void Draw::readPoint();
	//void Draw::savePoint();
	

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
		virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSavePoint();
	afx_msg void OnBnClickedDrawCancel();
	afx_msg void OnBnClickedShowAllPoint();
	afx_msg void OnBnClickedShowZeroPoint();
	afx_msg void OnBnClickedShowArea();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedDrawDlgSiteUp();
	afx_msg void OnBnClickedDrawDlgSiteLeft();
	afx_msg void OnBnClickedDrawDlgSiteDown();
	afx_msg void OnBnClickedDrawDlgSiteRight();
	afx_msg void OnEnChangeEditLeftup();
	long m_leftUpX;
	long m_leftUpY;
	long m_rightDownX;
	long m_rightDownY;
};

