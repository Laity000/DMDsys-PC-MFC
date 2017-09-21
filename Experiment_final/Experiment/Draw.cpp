// Draw.cpp : ʵ���ļ�
#include "stdafx.h"
#include "Experiment.h"
#include "ExperimentDlg.h"
#include "Draw.h"
#include<cmath>
CPoint m_startPoint;//ѡ�����ϵ�
CPoint m_finishPoint;//ѡ�����µ�
CPoint m_anchorPoint;//ê�㣺��갴סʱ�ĵ�
CPoint m_mouseStartPoint,m_mouseFinishPoint;//���ѡ�������ϡ����µ�
bool reference[1024*768];


bool m_isMouseFlag=false;//��¼����Ƿ���,trueΪ��깤����falseΪ���̹���
bool m_isMousePressed=false;//���״̬��trueΪ��ס��flaseΪ�ɿ�
bool flagLXChange=false;
bool flagRXChange=false;
bool flagUYChange=false;
bool flagDYChange=false;
bool flagAChange=false;
//bool flagRight=false;//Ҫȥ��
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
Draw::Draw(CWnd* pParent /*=NULL*/)//���캯�� ֻ�г����˳����������У�������ʾ��ť�Ż�����
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

BOOL Draw::OnInitDialog()		//ÿ�ε�����ʼ��ʾ��ť��������
{
	CDialog::OnInitDialog();
	//��ʼ������
	//���̹���
	m_isMouseFlag=false;
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	//��ʼ�����ε�����������
//	readPoint();
	m_mouseStartPoint = m_startPoint;
	m_mouseFinishPoint = m_finishPoint;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

void Draw::OnLButtonDown(UINT nFlags, CPoint point)//����������*********************************************
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_isMouseFlag)					//��겻����
		return;
	if((point.x>1024)||(point.y>768))//���Խ��
	{	
		m_isMousePressed = false;
		return;
	}
	m_isMousePressed=true;
	m_anchorPoint=point;
	CDialog::OnLButtonDown(nFlags, point);
}

void Draw::OnLButtonUp(UINT nFlags, CPoint point)//�������ɿ�
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_isMouseFlag)
		return;
	
	if((point.x>1024)||(point.y>768))//�յ�Խ��
	{
		m_isMousePressed=false;
		return;
	}	
	if(m_isMousePressed == false)
	{//���Խ��
		return;
	}
	m_isMousePressed=false;
	
	if(flagLXChange||flagRXChange||flagUYChange||flagDYChange||flagAChange)//����ͼ���޸Ĳ���
	{
		flagLXChange=flagRXChange=flagUYChange=flagDYChange=flagAChange=false;
		return;
	}
	//����µ�ͼ��
	//m_mouseStartPoint=minPoint(m_anchorPoint,point);
	//m_mouseFinishPoint=maxPoint(point,m_anchorPoint);

	//�����ʱ��ͼ��
	//CBrush brush(RGB(255,255,255));
	//CClientDC dc(this);
	//dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	//����µ�ͼ��
	
}


void Draw::OnPaint()					
{
	 //TODO: �ڴ˴������Ϣ����������
	 //��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//�����ο�ܣ�ʹ�ÿձ�ˢ��͸������
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
	//��ͼ
	//m_mouseStartPoint = m_startPoint;
	//m_mouseFinishPoint = m_finishPoint;
	CBrush brush(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
}
void printXYLocation(CDC *pDC,CPoint point)
{
	CString strtemp;
	pDC->TextOutA(1030,20,"                                            ");
	strtemp.Format("�������[X,Y]:[%d,%d]",point.x,point.y);
	pDC->TextOutA(1030,20,strtemp);
	
}
void printfLocation(CDC *pDC)//��ʾ��ǰ����Ĵ�С
{
	CString strtemp;
	//��ʾ�����С
	pDC->TextOutA(1030,50,"                                            ");
	strtemp.Format("��ǰ�����С:%dX%d",m_mouseFinishPoint.x-m_mouseStartPoint.x+1,m_mouseFinishPoint.y-m_mouseStartPoint.y+1);
	pDC->TextOutA(1030,50,strtemp);
	//��ʾѡ������
	pDC->TextOutA(1030,80,"                                            ");
	strtemp.Format("��������:[%d,%d]",m_mouseStartPoint.x,m_mouseStartPoint.y);
	pDC->TextOutA(1030,80,strtemp);
	pDC->TextOutA(1030,100,"                                            ");
	strtemp.Format("��������:[%d,%d]",m_mouseFinishPoint.x,m_mouseFinishPoint.y);
	pDC->TextOutA(1030,100,strtemp);
	strtemp.Format("���״̬:%d",m_isMousePressed);
	pDC->TextOutA(1030,130,strtemp);

}
void setCursor(Draw *pthis,CPoint point)
{
		if(!m_isMouseFlag)//��겻����
			return;
		//�ж�����Ƿ�����ѡ�����ڲ�
		if((point.x>m_mouseStartPoint.x+2&&point.x<m_mouseFinishPoint.x-2)&&(point.y>m_mouseStartPoint.y+2&&point.y<m_mouseFinishPoint.y-2))
		{
			//�ı�ɿ����϶�����״
			flagAChange=true;
			flagLXChange=flagRXChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEALL));

		}
		//�ж�����߽���
		else if((abs(point.x-m_mouseStartPoint.x)<3) && (point.y>m_mouseStartPoint.y&&point.y<m_mouseFinishPoint.y))
		{
			//�ı��ˮƽ����ļ�ͷ
			flagLXChange=true;
			flagAChange=flagRXChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEWE));
		}
		//�ж����ұ߽���
		else if((abs(point.x-m_mouseFinishPoint.x)<3)&&(point.y>m_mouseStartPoint.y&&point.y<m_mouseFinishPoint.y))
		{
			//�ı��ˮƽ����ļ�ͷ
			flagRXChange=true;
			flagLXChange=flagAChange=flagUYChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZEWE));
		}
		//�ж����ϱ߽���
		else if((abs(point.y-m_mouseStartPoint.y)<3)&&(point.x>m_mouseStartPoint.x&&point.x<m_mouseFinishPoint.x))
		{
			//�ı�ɴ�ֱ����ļ�ͷ
			flagUYChange=true;
			flagLXChange=flagRXChange=flagAChange=flagDYChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZENS));
		}
		//�ж����±߽���
		else if((abs(point.y-m_mouseFinishPoint.y)<3)&&(point.x>m_mouseStartPoint.x&&point.x<m_mouseFinishPoint.x))
		{
			//�ı�ɴ�ֱ����ļ�ͷ
			flagDYChange=true;
			flagLXChange=flagRXChange=flagUYChange=flagAChange=false;
			SetClassLong(pthis->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_SIZENS));
		}
		else//����������
		{
			//�ı��������״
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
	if(flagLXChange)//��߽��ڸı�
	{	
		distance = point.x - m_anchorPoint.x;
		m_mouseStartPoint.x += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
		
	}
	else if(flagRXChange)//�ұ߽��ڸı�
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
	else if(flagUYChange)//�ϱ߽��ڸı�
	{
		distance = point.y - m_anchorPoint.y;
		m_mouseStartPoint.y += distance;
		m_mouseStartPoint=minPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_mouseFinishPoint=maxPoint(m_mouseStartPoint,m_mouseFinishPoint);
		m_anchorPoint = point;
	}
	else if(flagDYChange)//�±߽��ڸı�
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
	else if(flagAChange)//�����϶�
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
	else	//��ͼ
	{
		m_mouseStartPoint=minPoint(m_anchorPoint,point);
		m_mouseFinishPoint=maxPoint(m_anchorPoint,point);
	}
}
void Draw::OnMouseMove(UINT nFlags, CPoint point)//����ƶ�ʱ�������
{
	
	if(point.x>1023||point.y>767)//Խ��
		return;
	CDC *pDC=GetDC();
	CBrush brush(RGB(255,255,255));
	CClientDC dc(this);

	if(!m_isMousePressed)
	{
		//���������û�а��£��ж�������״
		setCursor(this,point);
	}
	else	//������������£���ı�startPoint��finishPoint�����ꡣ��ˢ����ʾ��ͼ��
	{
		changeLocation(point);//�ı�startPoint��finishPoint������
		dc.FillRect(CRect(0,0,1023,767),&brush);		//ˢ����ʾ��ͼ��
	}
	printXYLocation(pDC,point);	//��ʾ����
	printfLocation(pDC);//��ʾ�����С
	CDialog::OnMouseMove(nFlags, point);
}

void Draw::OnShowWindow(BOOL bShow, UINT nStatus)
{

	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: �ڴ˴������Ϣ����������
}

//************************************     
// ��������: OnBnClickedSavePoint     
// ����˵��������ѡ��λ��     
// �� �� ֵ: void     
//************************************  
void Draw::OnBnClickedSavePoint()//���±����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//1,��ȡ����ֵ������startpoint��finishpoint
	//2,����reference����
	//savePoint();
	//����reference����
	m_startPoint = m_mouseStartPoint;
	m_finishPoint = m_mouseFinishPoint;
	int i,j;
	for(j=0;j<768;j++)
	{
		for(i=0;i<1024;i++)
		{
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//�ھ��ο���
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

void Draw::OnBnClickedDrawCancel()//����ȡ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void Draw::OnBnClickedShowAllPoint()//����ȫ����ʾ��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ʼ����������Ϊ��0��0���յ�����Ϊ����1023��767��
	m_mouseStartPoint.x=m_mouseStartPoint.y=0;
	m_mouseFinishPoint.x=1023;
	m_mouseFinishPoint.y=767;

	CClientDC dc(this);
	CBrush brush(RGB(255,0,0));
	dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush);
	CDC *pDC =GetDC();
	printfLocation(pDC);//��ʾ�����С
}

void Draw::OnBnClickedShowZeroPoint()// ȫ��ȡ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_mouseStartPoint.x=m_mouseStartPoint.y=m_mouseFinishPoint.x=m_mouseFinishPoint.y=0;

	CClientDC dc(this);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(CRect(0,0,1023,767),&brush);

	CDC *pDC =GetDC();
	printfLocation(pDC);//��ʾ�����С
}


void Draw::OnBnClickedShowArea()//��ʾѡ�������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_isMouseFlag)//����������ڹ���
	{
		UpdateData(TRUE);
		if(m_leftUpX<0 || m_leftUpX>1023){
			//MessageBox("��������X�ᳬ����Χ��");
			return;
		}
		if(m_leftUpY<0 || m_leftUpY>767){
			//MessageBox("��������Y�ᳬ����Χ��");
			return;
		}
		if(m_rightDownX<0 || m_rightDownX>1023){
			//MessageBox("��������X�ᳬ����Χ��");
			return;
		}
		if(m_rightDownY<0 || m_rightDownY>767){
			//MessageBox("��������Y�ᳬ����Χ��");
			return;
		}
		if(m_leftUpX>=m_rightDownX || m_leftUpY>=m_rightDownY){
			return;
		}
		
		m_mouseStartPoint.x = m_leftUpX;
		m_mouseStartPoint.y = m_leftUpY;
		m_mouseFinishPoint.x = m_rightDownX;
		m_mouseFinishPoint.y = m_rightDownY;

		//��ͼ�����
		CBrush brush(RGB(255,255,255));
		CClientDC dc(this);
		dc.FillRect(CRect(0,0,1023,768),&brush);
		//�������������������Ը���ͼ��
		CBrush brush1(RGB(255,0,0));
		dc.FillRect(CRect(m_mouseStartPoint,m_mouseFinishPoint),&brush1);
		CDC *pDC=GetDC();
		printfLocation(pDC);
	}
}

void Draw::OnBnClickedRadio1()//���̹���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isMouseFlag=false;
	GetDlgItem(IDC_EDIT_LeftUpX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LeftUpY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RightDownX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RightDownY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ShowArea)->EnableWindow(TRUE);
}

void Draw::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isMouseFlag=true;
	GetDlgItem(IDC_EDIT_LeftUpX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LeftUpY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RightDownX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RightDownY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ShowArea)->EnableWindow(FALSE);
}

void Draw::OnBnClickedDrawDlgSiteUp()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	printfLocation(pDC);//��ʾ�����С
}

void Draw::OnBnClickedDrawDlgSiteLeft()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	printfLocation(pDC);//��ʾ�����С
}

void Draw::OnBnClickedDrawDlgSiteDown()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	printfLocation(pDC);//��ʾ�����С
}

void Draw::OnBnClickedDrawDlgSiteRight()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	printfLocation(pDC);//��ʾ�����С
}


