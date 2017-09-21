// Draw.cpp : 实现文件
#include "stdafx.h"
#include "Experiment.h"
#include "ExperimentDlg.h"
#include "Draw.h"
#include<cmath>
CPoint m_startPoint;//选区左上点
CPoint m_finishPoint;//选区右下点
CPoint m_anchorPoint;//锚点：鼠标按住时的点
CPoint m_mouseStartPoint,m_mouseFinishPoint;//鼠标选区的左上、右下点
bool reference[1024*768];


bool m_isMouseFlag=false;//记录鼠标是否工作,true为鼠标工作，false为键盘工作
bool m_isMousePressed=false;//鼠标状态，true为按住，flase为松开
bool flagLXChange=false;
bool flagRXChange=false;
bool flagUYChange=false;
bool flagDYChange=false;
bool flagAChange=false;
//bool flagRight=false;//要去掉
IMPLEMENT_DYNAMIC(Draw, CDialog)
CPoint minPoint(CPoint start,CPoint finish)
{
	CPoint point;
	point.x=start.x>finish.x? finish.x:start.x;
	point.y=start.y>finish.y? finish.y:start.y;
	return point;
}
CPoint maxPoint(CPoint start,CPoint finish)
{
	CPoint point;
	point.x=start.x<finish.x? finish.x:start.x;
	point.y=start.y<finish.y? finish.y:start.y;
	return point;
}
Draw::Draw(CWnd* pParent /*=NULL*/)//构造函数 只有程序退出后重新运行，单击显示按钮才会运行
	: CDialog(Draw::IDD, pParent)
	, m_leftUpX(0)
	, m_leftUpY(0)
	, m_rightDownX(0)
	, m_rightDownY(0)
{
	
}

Draw::~Draw()
{

}

BOOL Draw::OnInitDialog()		//每次单击开始显示按钮都会运行
{
	CDialog::OnInitDialog();
	//初始化代码
	//键盘工作
	m_isMouseFlag=false;
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	//初始化矩形的两个特征点
//	readPoint();
	m_mouseStartPoint = m_startPoint;
	m_mouseFinishPoint = m_finishPoint;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void Draw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LeftUpX, m_leftUpX);
	DDX_Text(pDX, IDC_EDIT_LeftUpY, m_leftUpY);
	DDX_Text(pDX, IDC_EDIT_RightDownX, m_rightDownX);
	DDX_Text(pDX, IDC_EDIT_RightDownY, m_rightDownY);
}


BEGIN_MESSAGE_MAP(Draw, CDialog)

ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_PAINT()								
ON_WM_MOUSEMOVE()					
ON_WM_SHOWWINDOW()
ON_BN_CLICKED(IDC_BTN_SavePoint, &Draw::OnBnClickedSavePoint)
ON_BN_CLICKED(IDC_BTN_DrawCancel, &Draw::OnBnClickedDrawCancel)
ON_WM_CREATE()
ON_BN_CLICKED(IDC_BTN_ShowAllPoint, &Draw::OnBnClickedShowAllPoint)
ON_BN_CLICKED(IDC_BTN_ShowZeroPoint, &Draw::OnBnClickedShowZeroPoint)
ON_BN_CLICKED(IDC_BTN_ShowArea, &Draw::OnBnClickedShowArea)
ON_BN_CLICKED(IDC_RADIO1, &Draw::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &Draw::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_BTN_DrawDlgSiteUp, &Draw::OnBnClickedDrawDlgSiteUp)
ON_BN_CLICKED(IDC_BTN_DrawDlgSiteLeft, &Draw::OnBnClickedDrawDlgSiteLeft)
ON_BN_CLICKED(IDC_BTN_DrawDlgSiteDown, &Draw::OnBnClickedDrawDlgSiteDown)
ON_BN_CLICKED(IDC_BTN_DrawDlgSiteRight, &Draw::OnBnClickedDrawDlgSiteRight)
END_MESSAGE_MAP()

void Draw::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键按下*********************************************
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_isMouseFlag)					//鼠标不工作
		return;
	if((point.x>1024)||(point.y>768))//起点越界
	{	
		m_isMousePressed = false;
		return;
	}
	m_isMousePressed=true;
	m_anchorPoint=point;
	CDialog::OnLButtonDown(nFlags, point);
}

void Draw::OnLButtonUp(UINT nFlags, CPoint point)//鼠标左键松开
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_isMouseFlag)
		return;
	
	if((point.x>1024)||(point.y>768))//终点越界
	{
		m_isMousePressed=false;
		return;
	}	
	if(m_isMousePressed == false)
	{//起点越界
		return;
	}
	m_isMousePressed=false;
	
	if(flagLXChange||flagRXChange||flagUYChange||flagDYChange||flagAChange)//进行图形修改操作
	{
		flagLXChange=flagRXChange=flagUYChange=flagDYChange=flagAChange=false;
		return;
	}
	//绘出新的图像
	//m_mouseStartPoint=minPoint(m_anchorPoint,point);
	//m_mouseFinishPoint=maxPoint(point,m_anchorPoint);

	//清除过时的图像
	//CBrush brush(RGB(255,255,255));
	//CClientDC dc(this);
	//dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	//绘出新的图像
	
}


void Draw::OnPaint()					
{
	 //TODO: 在此处添加消息处理程序代码
	 //不为绘图消息调用 CDialog::OnPaint()
	//画矩形框架，使用空笔刷画透明矩形
	CPoint point1,point2;
	point1.x=0;
	point1.y=0;
	point2.x=1024;
	point2.y=768;
	CClientDC dc(this);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush=dc.SelectObject(pBrush);
	dc.Rectangle(CRect(point1,point2));
	dc.SelectObject(pOldBrush);
	//画图
	//m_mouseStartPoint = m_startPoint;
	//m_mouseFinishPoint = m_finishPoint;
	CBrush brush(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
}
void printXYLocation(CDC *pDC,CPoint point)
{
	CString strtemp;
	pDC->TextOutA(1030,20,"                                            ");
	strtemp.Format("鼠标坐标[X,Y]:[%d,%d]",point.x,point.y);
	pDC->TextOutA(1030,20,strtemp);
	
}
void printfLocation(CDC *pDC)//显示当前区域的大小
{
	CString strtemp;
	//显示区域大小
	pDC->TextOutA(1030,50,"                                            ");
	strtemp.Format("当前区域大小:%dX%d",m_mouseFinishPoint.x-m_mouseStartPoint.x+1,m_mouseFinishPoint.y-m_mouseStartPoint.y+1);
	pDC->TextOutA(1030,50,strtemp);
	//显示选点坐标
	pDC->TextOutA(1030,80,"                                            ");
	strtemp.Format("左上坐标:[%d,%d]",m_mouseStartPoint.x,m_mouseStartPoint.y);
	pDC->TextOutA(1030,80,strtemp);
	pDC->TextOutA(1030,100,"                                            ");
	strtemp.Format("右下坐标:[%d,%d]",m_mouseFinishPoint.x,m_mouseFinishPoint.y);
	pDC->TextOutA(1030,100,strtemp);
	strtemp.Format("鼠标状态:%d",m_isMousePressed);
	pDC->TextOutA(1030,130,strtemp);

}
void setCursor(Draw *pthis,CPoint point)
{
		if(!m_isMouseFlag)//鼠标不工作
			return;
		//判断鼠标是否在所选区域内部
		if((point.x>m_mouseStartPoint.x+2&&point.x<m_mouseFinishPoint.x-2)&&(point.y>m_mouseStartPoint.y+2&&point.y<m_mouseFinishPoint.y-2))
		{
			//改变成可以拖动的形状
			flagAChange=true;
			flagLXChange=flagRXChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEALL));

		}
		//判断在左边界上
		else if((abs(point.x-m_mouseStartPoint.x)<3) && (point.y>m_mouseStartPoint.y&&point.y<m_mouseFinishPoint.y))
		{
			//改变成水平方向的箭头
			flagLXChange=true;
			flagAChange=flagRXChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEWE));
		}
		//判断在右边界上
		else if((abs(point.x-m_mouseFinishPoint.x)<3)&&(point.y>m_mouseStartPoint.y&&point.y<m_mouseFinishPoint.y))
		{
			//改变成水平方向的箭头
			flagRXChange=true;
			flagLXChange=flagAChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEWE));
		}
		//判断在上边界上
		else if((abs(point.y-m_mouseStartPoint.y)<3)&&(point.x>m_mouseStartPoint.x&&point.x<m_mouseFinishPoint.x))
		{
			//改变成垂直方向的箭头
			flagUYChange=true;
			flagLXChange=flagRXChange=flagAChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZENS));
		}
		//判断在下边界上
		else if((abs(point.y-m_mouseFinishPoint.y)<3)&&(point.x>m_mouseStartPoint.x&&point.x<m_mouseFinishPoint.x))
		{
			//改变成垂直方向的箭头
			flagDYChange=true;
			flagLXChange=flagRXChange=flagUYChange=flagAChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZENS));
		}
		else//不在区域上
		{
			//改变成正常形状
			flagLXChange=false;
			flagRXChange=false;
			flagUYChange=false;
			flagDYChange=false;
			flagAChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
		}
}
void changeLocation(CPoint point)
{
	int distance;
	if(flagLXChange)//左边界在改变
	{	
		distance = point.x - m_anchorPoint.x;
		m_mouseStartPoint.x += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
		
	}
	else if(flagRXChange)//右边界在改变
	{
		distance = point.x - m_anchorPoint.x;
		m_mouseFinishPoint.x += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
		if(m_mouseFinishPoint.x>1023)
		{
			m_mouseFinishPoint.x = 1023;
		}
	}
	else if(flagUYChange)//上边界在改变
	{
		distance = point.y - m_anchorPoint.y;
		m_mouseStartPoint.y += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
	}
	else if(flagDYChange)//下边界在改变
	{
		
		distance = point.y - m_anchorPoint.y;
		m_mouseFinishPoint.y += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
		if(m_mouseFinishPoint.y>767)
		{
			m_mouseFinishPoint.y = 767;
		}
	}
	else if(flagAChange)//鼠标的拖动
	{	
		
		long xdistance = point.x-m_anchorPoint.x;
		long ydistance = point.y-m_anchorPoint.y;

		m_mouseStartPoint.x += xdistance;
		m_mouseStartPoint.y += ydistance;
		m_mouseFinishPoint.x += xdistance;
		m_mouseFinishPoint.y += ydistance;
		m_anchorPoint = point;
		
		if(m_mouseStartPoint.x>1023||m_mouseStartPoint.y>767||m_mouseFinishPoint.x>1023||m_mouseFinishPoint.y>767||m_mouseStartPoint.x<0||m_mouseStartPoint.y<0||m_mouseFinishPoint.x<0||m_mouseFinishPoint.y<0)
		{
			m_mouseStartPoint.x -= xdistance;
			m_mouseStartPoint.y -= ydistance;
			m_mouseFinishPoint.x -= xdistance;
			m_mouseFinishPoint.y -= ydistance;
		}
		
	}	
	else	//画图
	{
		m_mouseStartPoint=minPoint(m_anchorPoint,point);
		m_mouseFinishPoint=maxPoint(m_anchorPoint,point);
	}
}
void Draw::OnMouseMove(UINT nFlags, CPoint point)//鼠标移动时输出坐标
{
	
	if(point.x>1023||point.y>767)//越界
		return;
	CDC *pDC=GetDC();
	CBrush brush(RGB(255,255,255));
	CClientDC dc(this);

	if(!m_isMousePressed)
	{
		//如果鼠标左键没有按下，判断鼠标的形状
		setCursor(this,point);
	}
	else	//如果鼠标左键按下，则改变startPoint和finishPoint的坐标。并刷新显示的图像
	{
		changeLocation(point);//改变startPoint和finishPoint的坐标
		dc.FillRect(CRect(0,0,1023,767),&brush);		//刷新显示的图像
	}
	printXYLocation(pDC,point);	//显示坐标
	printfLocation(pDC);//显示区域大小
	CDialog::OnMouseMove(nFlags, point);
}

void Draw::OnShowWindow(BOOL bShow, UINT nStatus)
{

	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: 在此处添加消息处理程序代码
}

//************************************     
// 函数名称: OnBnClickedSavePoint     
// 函数说明：保存选区位置     
// 返 回 值: void     
//************************************  
void Draw::OnBnClickedSavePoint()//按下保存键
{
	// TODO: 在此添加控件通知处理程序代码
	//1,获取坐标值并保存startpoint和finishpoint
	//2,生成reference数组
	//savePoint();
	//生成reference数组
	m_startPoint = m_mouseStartPoint;
	m_finishPoint = m_mouseFinishPoint;
	int i,j;
	for(j=0;j<768;j++)
	{
		for(i=0;i<1024;i++)
		{
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//在矩形框内
			{
				reference[j*1024+i]=true;
			}
			else
			{
				reference[j*1024+i]=false;	
			}
		}
	}
	OnOK();
}

void Draw::OnBnClickedDrawCancel()//按下取消键
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void Draw::OnBnClickedShowAllPoint()//按下全部显示按钮
{
	// TODO: 在此添加控件通知处理程序代码
	//将起始点坐标设置为（0，0）终点设置为：（1023，767）
	m_mouseStartPoint.x=m_mouseStartPoint.y=0;
	m_mouseFinishPoint.x=1023;
	m_mouseFinishPoint.y=767;

	CClientDC dc(this);
	CBrush brush(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}

void Draw::OnBnClickedShowZeroPoint()// 全部取消
{
	// TODO: 在此添加控件通知处理程序代码
	m_mouseStartPoint.x=m_mouseStartPoint.y=m_mouseFinishPoint.x=m_mouseFinishPoint.y=0;

	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1023,767),&brush);

	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}


void Draw::OnBnClickedShowArea()//显示选择的区域
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_isMouseFlag)//如果键盘正在工作
	{
		UpdateData(TRUE);
		if(m_leftUpX<0 || m_leftUpX>1023){
			//MessageBox("左上坐标X轴超出范围！");
			return;
		}
		if(m_leftUpY<0 || m_leftUpY>767){
			//MessageBox("左上坐标Y轴超出范围！");
			return;
		}
		if(m_rightDownX<0 || m_rightDownX>1023){
			//MessageBox("右下坐标X轴超出范围！");
			return;
		}
		if(m_rightDownY<0 || m_rightDownY>767){
			//MessageBox("右下坐标Y轴超出范围！");
			return;
		}
		if(m_leftUpX>=m_rightDownX || m_leftUpY>=m_rightDownY){
			return;
		}
		
		m_mouseStartPoint.x = m_leftUpX;
		m_mouseStartPoint.y = m_leftUpY;
		m_mouseFinishPoint.x = m_rightDownX;
		m_mouseFinishPoint.y = m_rightDownY;

		//将图形清除
		CBrush brush(RGB(255,255,255));
		CClientDC dc(this);
		dc.FillRect(CRect(0,0,1023,768),&brush);
		//将两个点的坐标读进来以更新图形
		CBrush brush1(RGB(255,0,0));
		dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush1);
		CDC *pDC=GetDC();
		printfLocation(pDC);
	}
}

void Draw::OnBnClickedRadio1()//键盘工作
{
	// TODO: 在此添加控件通知处理程序代码
	m_isMouseFlag=false;
	GetDlgItem(IDC_EDIT_LeftUpX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LeftUpY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RightDownX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RightDownY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ShowArea)->EnableWindow(TRUE);
}

void Draw::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isMouseFlag=true;
	GetDlgItem(IDC_EDIT_LeftUpX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LeftUpY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RightDownX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RightDownY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ShowArea)->EnableWindow(FALSE);
}

void Draw::OnBnClickedDrawDlgSiteUp()//上
{
	// TODO: 在此添加控件通知处理程序代码
	m_mouseStartPoint.y-=1;
	m_mouseFinishPoint.y-=1;
	if(m_mouseStartPoint.y<0||m_mouseFinishPoint.y<0)
	{
		m_mouseStartPoint.y+=1;
		m_mouseFinishPoint.y+=1;		
	}
	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1024,768),&brush);
	CBrush brush1(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}

void Draw::OnBnClickedDrawDlgSiteLeft()//左
{
	// TODO: 在此添加控件通知处理程序代码
	m_mouseStartPoint.x-=1;
	m_mouseFinishPoint.x-=1;
	if(m_mouseStartPoint.x<0||m_mouseFinishPoint.x<0)
	{
		m_mouseStartPoint.x+=1;
		m_mouseFinishPoint.x+=1;	
	}
	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1024,768),&brush);
	CBrush brush1(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}

void Draw::OnBnClickedDrawDlgSiteDown()//下
{
	// TODO: 在此添加控件通知处理程序代码
	m_mouseStartPoint.y+=1;
	m_mouseFinishPoint.y+=1;
	if(m_mouseStartPoint.y>767||m_mouseFinishPoint.y>767)
	{
		m_mouseStartPoint.y-=1;
		m_mouseFinishPoint.y-=1;
	}
	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1024,768),&brush);
	CBrush brush1(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}

void Draw::OnBnClickedDrawDlgSiteRight()//右
{
	// TODO: 在此添加控件通知处理程序代码
	m_mouseStartPoint.x+=1;
	m_mouseFinishPoint.x+=1;
	if(m_mouseStartPoint.x>1023||m_mouseFinishPoint.x>1023)
	{
		m_mouseStartPoint.x-=1;
		m_mouseFinishPoint.x-=1;
	}
	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1024,768),&brush);
	CBrush brush1(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//显示区域大小
}


