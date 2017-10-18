
// ExperimentDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Experiment.h"
#include "ExperimentDlg.h"
//#include"serialport.h"
#include"Draw.h"
#include <wtypes.h>
#include <dbt.h> 
//#include"CyAPI.h"
#include"math.h"
#define TIME 200
#define PATH_MAX 512
#define NAME_MAX 512
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern bool reference[1024*768];
extern CPoint m_startPoint;
extern CPoint m_finishPoint;
//CCyUSBDevice *USBDevice = NULL;
HANDLE USBHandle;
CSerialPort Port;//�˿ڶ���Port
void ChangeMap(unsigned char *ibuf, unsigned char *obuf);
bool StopPrint=false;
//bool hMutex_flag = TRUE;
//int FrameFrequency=0;//֡Ƶ
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
Draw draw;

//new variables
//CString str;
//CString str1;
//unsigned char cmd_buf[4];  //96k bit

float GetFrameFrequency[31]={
0.5,1,5,10,20,40,60,80,100,120,160,200,300,400,500,600,700,800,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3100
};

int dataLength()
{
	return m_finishPoint.x-m_startPoint.x+1;
}
int dataWidth()
{
	return m_finishPoint.y-m_startPoint.y+1;
}
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()




//void CExperimentDlg::MessageBox(char *str,...)
//{
//	va_list var;
//	va_start(var,str);
//	char buffer[NAME_MAX];
//	memset(buffer,0,NAME_MAX);
//	vsnprintf(buffer,NAME_MAX,str,var);
//	va_end(var);
//	CWnd::MessageBox(buffer);
//}

CExperimentDlg::CExperimentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExperimentDlg::IDD, pParent)
	, m_PluseWidth(0)
	, m_FrameCount(1)
	, m_StartIndex(0)
	, m_EndIndex(-1)
	, IntStartIndex(0)
	, IntEndIndex(-1)
	, m_FreqSet(0)
	, m_MemMag(0)
	, m_MemReadAddr(0)
	, m_MemWriterAddr(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//flagDraw=false;
	//FrameFrequency=0;//֡Ƶ
	StopPrint=false;
	//	strPath = "F:\\save\\";

	m_Mode = NORMAL;
	isOpenDevice = false;

	m_strSearchPath = _T("");
	m_strSavePath = _T("");
	m_MultfileName = _T("");
}

void CExperimentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PluseWidth, m_PluseWidth);
	//DDV_MinMaxUInt(pDX, PluseWidth, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_FrameCount, m_FrameCount);
	//DDV_MinMaxInt(pDX, FRAMECOUNT, 1, 1023);
	DDX_Text(pDX, IDC_EDIT_StartIndex, m_StartIndex);
	DDV_MinMaxInt(pDX, m_StartIndex, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_EndIndex, m_EndIndex);
	DDV_MinMaxInt(pDX, m_EndIndex, -1, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_FreqSet, m_FreqSet);
	DDX_Text(pDX, IDC_EDIT_MemMag, m_MemMag);
	DDX_Text(pDX, IDC_EDIT_MemReadAddr, m_MemReadAddr);
	DDX_Text(pDX, IDC_EDIT_MemWriterAddr, m_MemWriterAddr);
	DDX_Text(pDX, IDC_EDIT_MultfileName, m_MultfileName);
}

BEGIN_MESSAGE_MAP(CExperimentDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_FileDownload, &CExperimentDlg::OnBnClickedFileDownload)
ON_BN_CLICKED(IDC_BTN_ResetDMD, &CExperimentDlg::OnBnClickedResetDMD)
ON_BN_CLICKED(IDC_BTN_ShowDMD, &CExperimentDlg::OnBnClickedShowDMD)
ON_BN_CLICKED(IDC_BTN_StopDMD, &CExperimentDlg::OnBnClickedStopDMD)
ON_BN_CLICKED(IDC_BTN_SiteSelect, &CExperimentDlg::OnBnClickedSiteSelect)
ON_BN_CLICKED(IDC_BTN_RandomNumDownload, &CExperimentDlg::OnBnClickedRandomNumDownload)
ON_BN_CLICKED(IDC_BTN_Cancel, &CExperimentDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BTN_OK, &CExperimentDlg::OnBnClickedOK)
ON_BN_CLICKED(IDC_BTN_SiteUp, &CExperimentDlg::OnBnClickedSiteUp)
ON_BN_CLICKED(IDC_BTN_SiteDown, &CExperimentDlg::OnBnClickedSiteDown)
ON_BN_CLICKED(IDC_BTN_SiteLeft, &CExperimentDlg::OnBnClickedSiteLeft)
ON_BN_CLICKED(IDC_BTN_SiteRight, &CExperimentDlg::OnBnClickedSiteRight)
ON_BN_CLICKED(IDC_BTN_PluseWidth, &CExperimentDlg::OnBnClickedPluseWidth)

ON_BN_CLICKED(IDC_BTN_ChangeMode, &CExperimentDlg::OnBnClickedChangeMode)
ON_BN_CLICKED(IDC_BTN_InterSync, &CExperimentDlg::OnBnClickedInterSync)
ON_BN_CLICKED(IDC_BTN_ExterSync, &CExperimentDlg::OnBnClickedExterSync)
ON_BN_CLICKED(IDC_BTN_FrameCount, &CExperimentDlg::OnBnClickedFrameCount)

ON_BN_CLICKED(IDC_BTN_ResetAddr, &CExperimentDlg::OnBnClickedResetAddr)
ON_BN_CLICKED(IDC_BTN_SavePath, &CExperimentDlg::OnBnClickedSavePath)
ON_BN_CLICKED(IDC_BTN_SearchPath, &CExperimentDlg::OnBnClickedSearchPath)
ON_BN_CLICKED(IDC_BTN_MultfileDownload, &CExperimentDlg::OnBnClickedMultfileDownload)
ON_BN_CLICKED(IDC_BTN_UsbInit, &CExperimentDlg::OnBnClickedUsbInit)
ON_BN_CLICKED(IDC_RBTN_CycleShow, &CExperimentDlg::OnBnClickedCycleShow)
ON_BN_CLICKED(IDC_BTN_FreqSet, &CExperimentDlg::OnBnClickedFreqSet)
ON_BN_CLICKED(IDC_BTN_ForShow, &CExperimentDlg::OnBnClickedForShow)
ON_BN_CLICKED(IDC_BTN_RevShow, &CExperimentDlg::OnBnClickedRevShow)
ON_BN_CLICKED(IDC_BTN_CmdTrigger, &CExperimentDlg::OnBnClickedCmdTrigger)
ON_BN_CLICKED(IDC_RstDmd, &CExperimentDlg::OnBnClickedRstdmd)
ON_BN_CLICKED(IDC_BTN_AllOn, &CExperimentDlg::OnBnClickedAllOn)
ON_BN_CLICKED(IDC_BTN_AllOff, &CExperimentDlg::OnBnClickedAllOff)
ON_BN_CLICKED(IDC_BTN_Revoke, &CExperimentDlg::OnBnClickedRevoke)
ON_BN_CLICKED(IDC_BTN_ClearMem, &CExperimentDlg::OnBnClickedClearMem)
ON_BN_CLICKED(IDC_BTN_MemMag, &CExperimentDlg::OnBnClickedMemmag)
ON_BN_CLICKED(IDC_BTN_ReadAddr, &CExperimentDlg::OnBnClickedReadAddr)
ON_BN_CLICKED(IDC_BTN_WriterAddr, &CExperimentDlg::OnBnClickedWriterAddr)
ON_WM_TIMER()

END_MESSAGE_MAP()


// CExperimentDlg ��Ϣ�������
//************************************     
// ��������: readPoint     
// ����˵�����������ļ��ж�ȡ��һ�γ��򱣴�����꼰�ļ�·����ע�⣺�����ļ�·��Ĭ�����ڵ�ǰ����·����     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::readPoint()		//��ʼ���ļ���Ϣ��
{
	FILE *fp;								
	m_startPoint.x=0;
	m_startPoint.y=0;
	m_finishPoint.x=0;
	m_finishPoint.y=0;

	
	
	if((fp=fopen((LPSTR)(LPCTSTR)m_strLocationPathAndName,"r"))!=NULL)//�ļ�����
	{

		char data[30];
		char savePath[PATH_MAX];
		char searchPath[PATH_MAX];
		fgets(data,30,fp);
		fgets(savePath,PATH_MAX,fp);
		//fgets(searchPath,PATH_MAX,fp);
		sscanf(data,"%d %d %d %d",&m_startPoint.x,&m_startPoint.y,&m_finishPoint.x,&m_finishPoint.y);
		int len = strlen(savePath);
		savePath[len-1]='\0';
		m_strSavePath.Format("%s",savePath);
		//strSearchPath.Format("%s",searchPath);
		fclose(fp);
	}

	//MessageBox(m_strSavePath);
	int i,j;
	for(j=0;j<768;j++)
		for(i=0;i<1024;i++)
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//�ھ��ο���
				reference[j*1024+i]=true;
			else
				reference[j*1024+i]=false;	
}
//************************************     
// ��������: savePoint     
// ����˵�����������꼰�ļ�·���������ļ��������´�����ʱ��ȡ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::savePoint()
{
	FILE *fp;
	fp=fopen((LPSTR)(LPCTSTR)m_strLocationPathAndName,"w+");

	CString content;
	content.Format("%d %d %d %d", m_startPoint.x,m_startPoint.y,m_finishPoint.x,m_finishPoint.y);
	content +="\n"+ m_strSavePath + "\n" +m_strSearchPath;
	//content += "\n"+ m_strSavePath; 
	fputs(content,fp);
	fclose(fp);

	int i,j;
	for(j=0;j<768;j++)
		for(i=0;i<1024;i++)
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//�ھ��ο���
				reference[j*1024+i]=true;
			else
				reference[j*1024+i]=false;	
}

//************************************     
// ��������: OnInitDialog     
// ����˵�����Ի����ʼ��     
// �� �� ֵ: BOOL     
//************************************  
BOOL CExperimentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	char buffer[PATH_MAX];
	GetCurrentDirectory(PATH_MAX,buffer);
	//�������ݶԱ��ļ���Ĭ�ϱ���·��
	m_strSavePath.Format("%s\\",buffer);
	//�����ļ�·�����ļ���
	m_strLocationPathAndName.Format("%s\\location.txt",buffer);//ע��//��ת���ַ�
	readPoint();
	//��ʼ�����ε�����������
	SetDlgItemText(IDC_BTN_ChangeMode,"����ģʽ������л�");

	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowTextA("δ�����κ�����");
	
	SetEditText(IDC_EDIT_SavePath,m_strSavePath);

	//SetEditText(IDC_EDIT_LeftUP,m_strSearchPath);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



void CExperimentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExperimentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	////////////////////////////readPoint();
	CDC *pDC=GetDC();
	char buffer1[10];
	char buffer2[10];
	char buffer3[10];
	char buffer4[10];
	memset(buffer1,0,10);
	memset(buffer2,0,10);
	memset(buffer3,0,10);
	memset(buffer4,0,10);
	_itoa(m_startPoint.x,buffer1,10);
	_itoa(m_startPoint.y,buffer2,10);
	_itoa(m_finishPoint.x,buffer3,10);
	_itoa(m_finishPoint.y,buffer4,10);
	int ascend=40;
	pDC->TextOutA(220,334+ascend,_T("                                "));
	pDC->TextOutA(280,334+ascend,_T("                               "));
	pDC->TextOutA(220,377+ascend,_T("                                "));
	pDC->TextOutA(280,377+ascend,_T("                               "));
	pDC->TextOutA(220,334+ascend,buffer1);
	pDC->TextOutA(280,334+ascend,buffer2);	
	pDC->TextOutA(220,377+ascend,buffer3);
	pDC->TextOutA(280,377+ascend,buffer4);	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExperimentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//************************************     
// ��������: DownLoad     �Զ��庯��
// ����˵�����������ļ�һ֡һ֡�ط��͵�DMD�У�ͬʱ����ASCILL�Ա��ļ�     
// �� �� ֵ: void    
// ��    ��: CString strFileName �ļ���(������׺)    
// ��    ��: CString strFilePath �ļ�·��    
//************************************  
void CExperimentDlg::DownLoad(CString strFileName,CString strFilePath)
{
	//����һЩ����
	
	unsigned char *p;
	unsigned char *dmd_buf_org		= new unsigned char[m_FrameCount*LINECOUNT*COLUMNCOUNT/8];//ԭʼ����
	unsigned char *dmd_buf_temp		= new unsigned char[LINECOUNT*COLUMNCOUNT];  //��ʱ���ݣ���������ѡ��	һ������һ֡
	unsigned char *dmd_buf			= new unsigned char[LINECOUNT*COLUMNCOUNT/8];  //���շ�������

	memset(dmd_buf,0,LINECOUNT*COLUMNCOUNT/8);
	memset(dmd_buf_temp,0,LINECOUNT*COLUMNCOUNT);
	memset(dmd_buf_org,0,m_FrameCount*LINECOUNT*COLUMNCOUNT/8);

	//��ѡ����ļ�
	CString strFileNameAndPath;
	strFileNameAndPath = strFilePath + strFileName;
	strFileNameAndPath += ".txt";
	CFile file(strFileNameAndPath,CFile::modeRead);
	
	//���ļ����ص�dmd_buf_org��
	if(file.GetLength() >= m_FrameCount*LINECOUNT*COLUMNCOUNT/8)//�ļ���С����Ҫ��
		file.Read(dmd_buf_org,m_FrameCount*LINECOUNT*COLUMNCOUNT/8);
	else
		file.Read(dmd_buf_org,file.GetLength());
	file.Close();

	CString strtemp;
	strtemp.Format("���������ļ�%s...",strFileName); 
	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);

	//////////////////////////////////////////////////////////////////////////
	int savecount = 2*1024*768+768;//ÿ����Ҫ���ϼ��ϻس���
	unsigned char *save = new unsigned char[savecount];	//���ڴ洢����ģʽ�µı�������
	memset(save,'0',savecount);//��ascii��洢��
	//����д���ļ��ĵ�ַ�͸�ʽstr    text1_234_42.txt
	char ch1[10];
	char ch2[10];
	memset(ch1,'\0',10);
	memset(ch2,'\0',10);
	itoa(dataLength(),ch2,10);
	itoa(dataWidth(),ch1,10);
	CString StrAscillFileNameAndPath;
	//m_strSavePath = "C:\Users\PC\Desktop\aaa\\";
	StrAscillFileNameAndPath = _T(m_strSavePath)+ strFileName + '_' + ch1 +  '_' + ch2 + ".txt";
	
	//MessageBox(m_strSavePath);
	//m_strSavePathΪ�����ļ���Ŀ¼��ַ���ڹ��캯�������á�
	CFile fileSave;
	if (fileSave.Open(StrAscillFileNameAndPath,CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		MessageBox(m_strSavePath + "��ʧ�ܣ�");
		return ;
	}
	
	
 
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��������+����ȶ��ļ�
	p=dmd_buf_org;
	for(int count=0;count<m_FrameCount;count++)//����600֡������
	{	
		//����ѡ������dmd_buf_temp����****************************************************
		ExpandData(p,dmd_buf_temp);//��չ���ݵ�1024*768�ֽ�
		if(m_startPoint!=m_finishPoint)//��������ѡ��
		{
			for(int i=0;i<1024*768;i++)
			dmd_buf_temp[i]&=reference[i];
		}else{
			MessageBox("ѡ�����Ϸ���������ѡ��");
			return;
		}
		//��dmd_buf_temp�����õ�����ת����ASCII�����save��
   		int i,j,k=0;
		for(j=0;j<768;j++)
		{
			for(i=0;i<1024;i++)
			{
				
				save[k++]=dmd_buf_temp[j*1024+i]+48;
				save[k++]=' ';
	
			}
			save[k++]='\n';

		}
		//��save����д���ļ� ******
		fileSave.SeekToEnd();
		fileSave.Write(save,savecount);
		
		//�����ݻ�д
		CompressData(dmd_buf_temp,dmd_buf);		//�������ݵ�768*1024/8�ֽ�
		//���������ص�������
		SendData(dmd_buf);//����һ֡�������    pΪҪ���ص����ݡ�dmd_buf_tempΪ��չ�����ʱ���ݡ�dmd_bufΪʵ�����ص�����
		p+=LINECOUNT*COLUMNCOUNT/8;//ָ����һ֡
	}
	fileSave.Close();

	delete[](save);
	
	delete[](dmd_buf_temp);		//����ʱ��������ʱ����
	delete[](dmd_buf);			//����Ҫ���ص�����
	delete[](dmd_buf_org);		//Ҫ���ص�����

	strtemp.Format("�ļ�%s�������!",strFileName); 
	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);
}

//************************************     
// ��������: OnBnClickedFileDownload     
// ����˵�������ļ��Ի������������ļ���DMD��     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedFileDownload()
{
	UpdateData(TRUE);
	
	if(!IsOpenDevice()){
		return;
	}
	
	
	if(m_FrameCount == 0 ){
		MessageBox("֡������Ϊ0!");
		return;
	}
	CFileDialog fileDialog(true);
	fileDialog.m_ofn.lpstrTitle="���ļ�";
	fileDialog.m_ofn.lpstrFilter="Text(*.txt)\0*.txt\0All File(*.*)\0*.*\0\0";
	if(IDOK!=fileDialog.DoModal())
		return;
	//���ļ������в��ִ���  ��ȡ�ļ����ļ�����������.txtȥ����ֻ���ļ�������data.txt��õ��ļ���Ϊdata����str��
	CString strFileName;
	strFileName=fileDialog.GetFileName();
	strFileName.Delete(strFileName.ReverseFind('.'),strFileName.GetLength()-strFileName.ReverseFind('.'));
	
	//�õ��ļ�·��
	CString strFilePath=fileDialog.GetPathName();
	
	/*
	CString strtemp;
	strtemp.Format("%s",strFilePath);
	AppendText(IDC_EDIT_StatusBar,strtemp);
	*/
	strFilePath.Delete(strFilePath.ReverseFind('\\')+1,strFilePath.GetLength()-strFilePath.ReverseFind('\\')-1);
	m_strSearchPath = strFilePath;
	GetDlgItem(IDC_EDIT_SearchPath)->SetWindowTextA(m_strSearchPath);
	
	SendCommand(0x00, 0x00);//ֹͣ��ʾ;
	
	AppendText(IDC_EDIT_StatusBar, "DMDֹͣ��ʾ!");
	
	DownLoad(strFileName,strFilePath);
	MessageBox("�������");
	
}
//δ�޸�
void CExperimentDlg::SendLine(int Line,unsigned char *dmd_buf_org_scan,unsigned char *dmd_buf_temp_scan,unsigned char *dmd_buf_scan,unsigned char *save_scan,unsigned char *dmd_buf_org)
{
	ExpandData(dmd_buf_org_scan,dmd_buf_temp_scan);//��չ���鵽FEAMECOUNT*1024�ֽ� ��������ѡ��


	if(m_startPoint!=m_finishPoint)
	{
		for(int frame = 0;frame < m_FrameCount;frame++)//����ÿһ�е�ÿһ֡
			for(int column = 0;column < COLUMNCOUNT;column++)
				dmd_buf_temp_scan[column+frame*COLUMNCOUNT] &= reference[column+Line*1024];
	}

	CompressData(dmd_buf_temp_scan,dmd_buf_scan);		//�������鵽FRAMECOUNT*128�ֽ�

   	int i,j;	

	for(i=0,j=0;i<m_FrameCount*COLUMNCOUNT;i++)
		{
			save_scan[j]=dmd_buf_temp_scan[i] + '0';
			j++;
			save_scan[j] = ' ';
			j++;
		}

	
	//��һ�е�dmd_buf_scan�����ݴ浽dmd_buf_org��
	
	for(int i=0;i<m_FrameCount*COLUMNCOUNT/8;i++)
		dmd_buf_org[Line*m_FrameCount*COLUMNCOUNT/8+i] = dmd_buf_scan[i];

	if(Line == (LINECOUNT-1))
	{
		unsigned char *p=dmd_buf_org;
		for(int frame=0;frame<m_FrameCount;frame++)
		{
			long length = LINECOUNT*COLUMNCOUNT/8;//����Ҫ���͵����ݳ���
			OVERLAPPED OvLap;
			OvLap.hEvent  = CreateEvent(NULL, false, false, "CYUSB_CTL");
			LONG rLen;
			PUCHAR Context;	// Sync
			CCyControlEndPoint  *ept = USBDevice->ControlEndPt;
			LONG  length1   = 0;
			ept->Target    = TGT_DEVICE;
			ept->ReqType   = REQ_VENDOR;
			ept->Direction = DIR_TO_DEVICE;
			ept->ReqCode   = 0xb1;
			ept->Value     = 0;
			ept->Index     = 0;

			Context = ept->BeginDataXfer(p, length1, &OvLap);
			ept->WaitForXfer(&OvLap,100);
			ept->FinishDataXfer(p, length1, &OvLap,Context);
			for(int i=0;i<500000;i++){};
			if (USBDevice->BulkOutEndPt)  
				USBDevice->BulkOutEndPt->XferData(p, length); 
			for(int i=0;i<1000000;i++)	{};

			p = p + LINECOUNT*COLUMNCOUNT/8;
		}
	}
}

//************************************     
// ��������: OnBnClickedResetDMD     
// ����˵����DMD��λ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedResetDMD()
{
	
	if(!IsOpenDevice()){
		return;
	}
	//����DMD��λָ��
	SendCommand(0x04, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD��λ!");
}

//************************************     
// ��������: OnBnClickedShowDMD     
// ����˵����DMD��ʼ��ʾ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedShowDMD()//������ʼ��ʾ��ť
{
	if(!IsOpenDevice()){
		return;
	}
	//���Ϳ�ʼ��ʾָ��
	SendCommand(0x00, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD��ʼ��ʾ!");
}

//************************************     
// ��������: OnBnClickedStopDMD     
// ����˵����DMDֹͣ��ʾ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedStopDMD()
{
	if(!IsOpenDevice()){
		return;
	}
	//����ֹͣ��ʾָ��
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMDֹͣ��ʾ!");
}

//************************************     
// ��������: OnBnClickedSiteSelect     
// ����˵��������ѡ������ť��û�б�ѡ�е�������������Ϊ0     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//flagDraw=true;
	draw.DoModal();
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
	savePoint();
	//readPoint();
	OnPaint();
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
}


//************************************     
// ��������: SendData     �Զ��庯��
// ����˵������������    
// �� �� ֵ: void     
// ��    ��: unsigned char * dmd_buf     
//************************************  
void CExperimentDlg::SendData(unsigned char *dmd_buf)//ע��ó���ֻ�ܷ���һ֡������
{	
	long length = 1024*96;//����Ҫ���͵����ݳ���
	OVERLAPPED OvLap;
	OvLap.hEvent  = CreateEvent(NULL, false, false, "CYUSB_CTL");
	PUCHAR Context;	// Sync
	CCyControlEndPoint  *ept = USBDevice->ControlEndPt;
	LONG  length1   = 0;
	ept->Target    = TGT_DEVICE;
	ept->ReqType   = REQ_VENDOR;
	ept->Direction = DIR_TO_DEVICE;
	ept->ReqCode   = 0xb2;
	ept->Value     = 0;
	ept->Index     = 0;

	Context = ept->BeginDataXfer(dmd_buf, length1, &OvLap);
	ept->WaitForXfer(&OvLap,100);
	ept->FinishDataXfer(dmd_buf, length1, &OvLap,Context);
	for(int i=0;i<500000;i++){};
	if (USBDevice->BulkOutEndPt)  
		USBDevice->BulkOutEndPt->XferData(dmd_buf, length); 
	for(int i=0;i<1000000;i++)	{};

}


//************************************     
// ��������: CompressData   �Զ��庯��
// ����˵������768*1024�ֽڵ�����������768*128�ֽڵ����ݣ���һ�ֽ�������һλ    
// �� �� ֵ: void     
// ��    ��: unsigned char * ibuf     
// ��    ��: unsigned char * obuf     
//************************************  
void CExperimentDlg::CompressData(unsigned char *ibuf, unsigned char *obuf)
{
    int m,i,j,k;
    int tt;
//	if(mode == NORMAL)								//��768*1024�ֽڵ�����������768*128�ֽڵ�����
	{
		ZeroMemory(obuf, 128*768);

		for(m=0;m<768;m++) //�����Ƿֱ��768�е�1028λ���ݴ���
			for(j=0;j<16;j++) //��Ϊ16��ʱ������
				for(i=0;i<8;i++) //ÿ�����ڵ�64λ���ݷ�Ϊ8���ֽڴ���
					             //ÿλ��16λ΢��������ôһ���ֽڸ�128λ΢������
				{
					tt = i;
					for(k=0;k<8;k++)
						obuf[128*m + 8*j + i] = (obuf[128*m + 8*j + i] << 1) | (ibuf[1024*m - j + 128*i + (127-16*k)] & 1);
				 }
	}
/*	else if(mode == SCAN)						//��FRAMECOUNT*1024�ֽڵ�����������FRAMECOUNT*128�ֽڵ����� 
	{
		ZeroMemory(obuf, 128*FRAMECOUNT);
		for(m=0;m<FRAMECOUNT;m++)
			for(j=0;j<16;j++)
				for(i=0;i<8;i++)
				{
					tt = i;
					for(k=0;k<8;k++)
						obuf[128*m + 8*j + i] = (obuf[128*m + 8*j + i] << 1) | (ibuf[1024*m - j + 128*i + (127-16*k)] & 1);
				 }
	}*/
}
//************************************     
// ��������: OnBnClickedRandomNumDownload     
// ����˵��������α�����     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedRandomNumDownload()//��������α�����
{
	UpdateData(TRUE);
	
	if(!IsOpenDevice()){
		return;
	}
	
	if(m_FrameCount == 0 ){
		MessageBox("֡������Ϊ0!");
		return;
	}
	//����һЩ����
	unsigned char *dmd_buf_org		=	new unsigned char[LINECOUNT*COLUMNCOUNT/8];		//����ģʽ��һ�����ص�ԭʼ����
	unsigned char *dmd_buf_org_scan =	new unsigned char[m_FrameCount*COLUMNCOUNT/8];	//ɨ��ģʽ��һ�����ص�ԭʼ����

	unsigned char *dmd_buf_temp		=	new unsigned char[LINECOUNT*COLUMNCOUNT];		//����ģʽ�µ���ʱ���ݣ���������ѡ��
	unsigned char *dmd_buf_temp_scan=	new unsigned char[m_FrameCount*COLUMNCOUNT];		//ɨ��ģʽ�µ���ʱ���ݣ���������ѡ��

	unsigned char *dmd_buf			=	new unsigned char[LINECOUNT*COLUMNCOUNT/8];		//����ģʽ�����շ�������
	int savecount =2*1024*768+768;//ÿ����Ҫ���ϼ��ϻس���
	unsigned char *save				=	new unsigned char[savecount];	//����ģʽ�µ����ݱ���
	unsigned char *save_scan		=	new unsigned char[2*m_FrameCount*COLUMNCOUNT];	//ɨ��ģʽ�µ����ݱ���

	unsigned char *dmd_buf_scan_all	=	new unsigned char[m_FrameCount*LINECOUNT*COLUMNCOUNT/8];	//ɨ�跽ʽ������ȫ�����͵�����
	unsigned char *p;
	
	//��ʼ����Щ����
	memset(dmd_buf_org		,0,	LINECOUNT*COLUMNCOUNT/8				);
	memset(dmd_buf_org_scan	,0,	m_FrameCount*COLUMNCOUNT/8			);
	memset(dmd_buf_temp		,0,	LINECOUNT*COLUMNCOUNT				);
	memset(dmd_buf_temp_scan,0,	m_FrameCount*COLUMNCOUNT				);
	memset(dmd_buf			,0,	LINECOUNT*COLUMNCOUNT/8				);
	memset(save				,'0',	savecount			);
	memset(save_scan		,'0',	2*m_FrameCount*COLUMNCOUNT			);
	memset(dmd_buf_scan_all	,0,	m_FrameCount*LINECOUNT*COLUMNCOUNT/8	);

	SYSTEMTIME time;

	
    //����ǰֹͣ��ʾ
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMDֹͣ��ʾ!");

	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText("����α�������..");
	//MessageBox("��ʼ����α�����");		

	if(m_Mode == NORMAL)				//����α�����
	{
		//�õ�������Ա��ļ���
		CString str="Random_";
		char ch1[10];
		char ch2[10];
		memset(ch1,'\0',10);
		memset(ch2,'\0',10);
		itoa(dataLength(),ch2,10);
		itoa(dataWidth(),ch1,10);
		str	+= ch1;
		str	+= '_';
		str	+= ch2;
		str	+= ".txt";
		str = m_strSavePath + str;
		CFile fileSave(str,CFile::modeCreate|CFile::modeWrite);
		for(int count=0;count<m_FrameCount;count++)
		{
			for(int i=0;i<LINECOUNT;i++)
				for(int j=0;j<128;j++)	//128=1024/8		һ�ε��ò���һ�ֽڵ������
				{
					GetSystemTime(&time);
					srand(j*time.wMilliseconds);
					dmd_buf_org[i*128+j]=rand();
				}

			ExpandData(dmd_buf_org,dmd_buf_temp);//��չ���鵽1024*768��������ѡ��
			if(m_startPoint!=m_finishPoint)
			{
				for(int i=0;i<1024*768;i++)
					dmd_buf_temp[i]&=reference[i];
			}
			else
			{
				MessageBox("ѡ���Ƿ���������ѡ��");
				return;
			}
			//��dmd_buf_temp�����õ�����ת����ASCII�����save��
   			int i,j,k=0;
			for(j=0;j<768;j++)
			{
				for(i=0;i<1024;i++)
				{
					
					save[k++]=dmd_buf_temp[j*1024+i]+48;	
					save[k++]=' ';
				
				}
				save[k++]='\n';
			}

			//��save����д���ļ� ******
			fileSave.SeekToEnd();
			fileSave.Write(save,savecount);
			//�����ݻ�д
			CompressData(dmd_buf_temp,dmd_buf);		//�������鵽1024*96
			//���������ص�������
			SendData(dmd_buf);//����һ֡�������    pΪҪ���ص����ݡ�dmd_buf_tempΪ��չ�����ʱ���ݡ�dmd_bufΪʵ�����ص�����
		}
		fileSave.Close();
		
		GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText("α�����������ϣ�");
		MessageBox("�������");
	}
	else if(m_Mode == SCAN)				//����α�����
	{
		CString strPath_temp = m_strSavePath;
		strPath_temp += "Random";
		char ch1[10];
		char ch2[10];
		memset(ch1,'\0',10);
		memset(ch2,'\0',10);
		itoa(dataLength(),ch2,10);
		itoa(dataWidth(),ch1,10);
		strPath_temp += '_';
		strPath_temp += ch1;
		strPath_temp += '_';
		strPath_temp += ch2;	

		if(!PathFileExists(strPath_temp))//���������
			CreateDirectory(strPath_temp,NULL);
		strPath_temp += '\\';
		CString str;
		str = strPath_temp +"Ramdom";
		str+='_';
		
		for(int Line=0;Line<LINECOUNT;Line++)
		{
			for(int i=0;i<m_FrameCount;i++)
			{
				for(int j=0;j<128;j++)	//128=1024/8
				{
					GetSystemTime(&time);
					srand(j*time.wMilliseconds);
					dmd_buf_org_scan[i*128+j]=rand();
				}
			}
			//��չ
			for(int frame=0;frame<m_FrameCount;frame++)
				for(int column=0;column<128;column++)
					for(int k=0;k<8;k++)
						dmd_buf_temp_scan[frame*1024+8*column+k] = (dmd_buf_org_scan[frame*128+column]>>(7-k))&1;
			//��ͼ
			if(m_startPoint!=m_finishPoint)
			{
				for(int frame = 0;frame < m_FrameCount;frame++)//����ÿһ�е�ÿһ֡
					for(int column = 0;column < COLUMNCOUNT;column++)
						dmd_buf_temp_scan[column+frame*COLUMNCOUNT] &= reference[column+Line*1024];
			}
			//����д���ļ��ĸ�ʽ���������ݵ����ݡ�
   			int i,j;	
			for(i=0,j=0;i<m_FrameCount*COLUMNCOUNT;i++)
				{
					save_scan[j]=dmd_buf_temp_scan[i] + '0';
					j++;
					save_scan[j] = ' ';
					j++;
				}
			str = str.Left(str.ReverseFind('_')+1);
			char buffer[10];
			itoa(Line,buffer,10);
			str += buffer;
			str += ".txt";
			CFile fileSave(str,CFile::modeCreate|CFile::modeWrite);
			fileSave.Write(save_scan,2*m_FrameCount*COLUMNCOUNT);
			fileSave.Close();
		
			//����ΪFRAMECOUNT*1024/8�ֽ�
			for(int frame=0;frame<m_FrameCount;frame++)
					for(int column=0;column<128;column++)
						for(int k=0;k<8;k++)
							dmd_buf_scan_all[Line*m_FrameCount*128+frame*128+column] = (dmd_buf_scan_all[Line*m_FrameCount*128+frame*128+column] << 1) | (dmd_buf_temp_scan[frame*COLUMNCOUNT+column*8+k] & 0x01);
		}
		p = dmd_buf_scan_all;
		for(int count=0;count<m_FrameCount;count++)//����FRAMECOUNT֡������
		{	
			ExpandData(p,dmd_buf_temp);//��չ���鵽1024*768��������ѡ��
			CompressData(dmd_buf_temp,dmd_buf);		//�������鵽1024*96
			SendData(dmd_buf);//����һ֡�������    pΪҪ���ص����ݡ�dmd_buf_tempΪ��չ�����ʱ���ݡ�dmd_bufΪʵ�����ص�����
			p+=LINECOUNT*COLUMNCOUNT/8;//ָ����һ֡
		}
	}

	//CloseUSB();

	//������Ϻ���Ҫ��λ��ַ,��Ҫ�޸�
	//ch = 0x62;
	//Port.WriteToPort(&ch,1);

	delete[](dmd_buf_org		);
	delete[](dmd_buf_org_scan	);
	delete[](dmd_buf_temp		);
	delete[](dmd_buf_temp_scan	);
	delete[](dmd_buf			);
	delete[](save				);
	delete[](save_scan			);
	delete[](dmd_buf_scan_all	);

}

//************************************     
// ��������: OnBnClickedCancel     
// ����˵����ȡ��  
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

//************************************     
// ��������: OnBnClickedOK     
// ����˵����ȷ��     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedOK()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	hMutex_flag = FALSE;
	//OnOK();
}

//************************************     
// ��������: OnBnClickedSiteUp     
// ����˵����ѡ����������һ��     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteUp()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_startPoint.y-=1;
	m_finishPoint.y-=1;
	if(m_startPoint.y<0||m_finishPoint.y<0)
	{
		m_startPoint.y+=1;
		m_finishPoint.y+=1;		
	}
	savePoint();
	OnPaint();
}

//************************************     
// ��������: OnBnClickedSiteDown     
// ����˵����ѡ����������һ��        
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteDown()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_startPoint.y+=1;
	m_finishPoint.y+=1;
	if(m_startPoint.y>767||m_finishPoint.y>767)
	{
		m_startPoint.y-=1;
		m_finishPoint.y-=1;
	}
	savePoint();
	OnPaint();
}

//************************************     
// ��������: OnBnClickedSiteLeft     
// ����˵����ѡ����������һ��        
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteLeft()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_startPoint.x-=1;
	m_finishPoint.x-=1;
	if(m_startPoint.x<0||m_finishPoint.x<0)
	{
		m_startPoint.x+=1;
		m_finishPoint.x+=1;	
	}
	savePoint();
	OnPaint();
}

//************************************     
// ��������: OnBnClickedSiteRight     
// ����˵����ѡ����������һ��        
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteRight()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_startPoint.x+=1;
	m_finishPoint.x+=1;
	if(m_startPoint.x>1023||m_finishPoint.x>1023)
	{
		m_startPoint.x-=1;
		m_finishPoint.x-=1;
	}
	savePoint();
	OnPaint();
}

//δ�޸�
char CExperimentDlg::GetBit(int count,UINT data)
{
	char dat=0;
	switch(count)
	{
	case 0:
		dat = (char) (data & 0x000F);
		break;
	case 1:
		dat = (char) ((data & 0x00F0) >> 4);
		break;
	case 2:
		dat = (char) ((data & 0x0F00) >> 8);
		break;
	case 3:
		dat = (char) ((data & 0xF000) >> 12);
		break;
	case 4:
		dat = (char) (data & 0x001F);
		break;
	case 5:
		dat = (char) ((data & 0x03E0) >> 5);
		break;
	case 6:
		dat = (char) ((data>>10)&0x001F);
	}
	return dat;
}

//************************************     
// ��������: OnBnClickedPluseWidth     
// ����˵�����ύ����     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedPluseWidth() //PluseWidth
{
	UpdateData(TRUE);
	if (m_PluseWidth == 0)
	{
		MessageBox("������Ϊ0!");
		return;
	}
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(((m_PluseWidth>>8) & 0x0f)+ 0x40, m_PluseWidth & 0xff);
	CString strtemp;
	strtemp.Format(_T("ͬ����������趨��%d"),m_PluseWidth);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}

//************************************     
// ��������: OnBnClickedChangeMode     
// ����˵�����ı���������ģʽ(Ŀǰֻ�ܴ�ʱ����ģʽ)     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedChangeMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_Mode == NORMAL)
	{
		//������ʾ���û���
		//GetDlgItem(IDC_BTN_ChangeMode)->SetWindowTextA("ɨ��ģʽ������л�");
		MessageBox("ɨ��ģʽ����δ��ӣ�");
		return;
		m_Mode = SCAN;
		//�л�Ϊɨ��ģʽ
		//1��ֹͣ��ʾ
		//char ch = 0x80;
		//Port.WriteToPort(&ch,1);
		//for(int i=0;i<50000;i++);
		//2���л�Ϊɨ��ģʽ
		//ch = 0xe0;
		//Port.WriteToPort(&ch,1);

		//3����������
		//OnBnClickedButton2();	//�൱�ڵ���������ݰ�ť
	}
	else
	{
		//������ʾ���û���
		m_Mode = NORMAL;
		SetDlgItemText(IDC_BTN_ChangeMode,"����ģʽ������л�");

		//�л�Ϊ����ģʽ
		//1��ֹͣ��ʾ
		//char ch = 0x80;
		//Port.WriteToPort(&ch,1);
	
		//2���л�Ϊ����ģʽ
		//for(int i=0;i<50000;i++);
		//ch = 0xe1;
		//Port.WriteToPort(&ch,1);

		//3����������
		//OnBnClickedButton2();	//�൱�ڵ���������ݰ�ť

	}
}

//************************************     
// ��������: OnBnClickedInterSync     
// ����˵������ͬ������    
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedInterSync()
{
	if(!IsOpenDevice()){
		return;
	}
	//������ͬ��ָ��
	SendCommand(0x0c, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD��ͬ��!");
}

//************************************     
// ��������: OnBnClickedExterSync     
// ����˵������ͬ������     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedExterSync()
{
	if(!IsOpenDevice()){
		return;
	}
	//������ͬ��ָ��
	SendCommand(0x0c, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD��ͬ��!");
}

//************************************     
// ��������: OnBnClickedFramcount     
// ����˵�����ύ�޸ĵ�֡����    
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedFrameCount()//�޸�֡Ƶ  
{
	UpdateData(TRUE);

	if(m_FrameCount == 0 ){
		MessageBox("֡������Ϊ0!");
		return;
	}
	
	if(!IsOpenDevice()){
		return;
	}
	
	//����ָ��
	SendCommand(((m_FrameCount>>8) & 0x07)+ 0x50, m_FrameCount & 0xff);

	CString strtemp;
	strtemp.Format(_T("֡����%d"),m_FrameCount);     
	AppendText(IDC_EDIT_StatusBar, strtemp);

	
	//cmd_buf[0] = (m_FrameCount>>11) & 0xff;       //��λ
	//cmd_buf[1] = ((m_FrameCount>>19) & 0x07)+ 0x58;  //��λ
}


//************************************     
// ��������: OnBnClickedResetAddr     
// ����˵�����ڴ��ַ��λ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedResetAddr()
{
	if(!IsOpenDevice()){
		return;
	}
	//���͵�ַ��λָ��
	SendCommand(0x24, 0x01);
	AppendText(IDC_EDIT_StatusBar, "��ַ��λ!");
}

//************************************     
// ��������: OnBnClickedSavePath    
// ����˵�������ö��ļ����ص�����·��     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSavePath()
{
	m_strSavePath = SelFilePath();
	m_strSavePath += "\\";
	//SetEditText(IDC_EDIT_SavePath,m_strSavePath);
	GetDlgItem(IDC_EDIT_SavePath)->SetWindowTextA(m_strSavePath);
	savePoint();
}


//************************************     
// ��������: SetEditText    �Զ��庯�� 
// ����˵��������Ӧ���ı�����ʾ�ַ���,�����ݳ����Զ�����
// �� �� ֵ: void     
// ��    ��: int nID  �ı��༭��ID   
// ��    ��: CString str     ����ʾ�ַ���
//************************************  
void CExperimentDlg::SetEditText(int nID,CString str)
{
	CString show;
	CClientDC dc(this);
	CRect rect;
	GetDlgItem(nID)->GetWindowRect(&rect);
	int editSize=rect.Width();
	int len=0;
	while(!str.IsEmpty())
	{
		len=0;
		while (len<str.GetLength()&&dc.GetTextExtent(str.Left(len)).cx<editSize)
			len++;
		show+=str.Left(len);
		show+="\r\n";
		str.Delete(0,len);
	}
	GetDlgItem(nID)->SetWindowTextA(show);
}
//************************************     
// ��������: OnBnClickedSearchPath     
// ����˵�������ö��ļ����ص�����·��    
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedSearchPath()
{
	
	m_strSearchPath = SelFilePath();
	m_strSearchPath += "\\";
	//SetEditText(IDC_EDIT_LeftUP,m_strSearchPath);
	GetDlgItem(IDC_EDIT_SearchPath)->SetWindowTextA(m_strSearchPath);
	savePoint();
}

//************************************     
// ��������: OnBnClickedMultfileDownload     
// ����˵�������ļ����أ�ͬʱ����ASCILL�Ա��ļ�     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedMultfileDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(m_FrameCount == 0 ){
		MessageBox("֡������Ϊ0!");
		return;
	}

	if(m_FreqSet == 0 ){
		MessageBox("֡Ƶ����Ϊ0!");
		return;
	}

	IntStartIndex = m_StartIndex;
	IntEndIndex = m_EndIndex;
	if(m_MultfileName.IsEmpty()|| m_MultfileName.ReverseFind('.') != -1 || m_EndIndex==-1 || m_StartIndex >= m_EndIndex +1)
	{
		MessageBox("��������ȷ�����ַ�Χ���ļ���(������׺��");
		return;
	}

	//����ֹͣ��ʾָ��
	
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMDֹͣ��ʾ!");

	SetTimer(1, 1000/m_FreqSet*m_FrameCount*1.5, NULL); 
	
	//SetTimer(1, 3000, NULL); 
	
	MessageBox("�������");
}




//************************************     
// ��������: OnBnClickedUsbInit     
// ����˵������ʼ��USB���������Ҳ���DMD�豸     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedUsbInit()
{
	//��ʼ��usb����
	USBDevice =  new CCyUSBDevice(USBHandle); 

	int devices = USBDevice->DeviceCount();
	if(devices == 0)
	{		
		AppendText(IDC_EDIT_StatusBar, "USBδ���ӣ��޷����豸��");
		return;
	}

	CString strtemp;
	strtemp.Format(_T("����%d\��USB�豸��\r\nDMD��������.."), devices);
	AppendText(IDC_EDIT_StatusBar, strtemp);
	for(int i=0;i<devices;i++)
	{
		int vID;
		int pID;
		//�������豸
		for(i=0;i<devices;i++)
		{
			USBDevice->Open(i); 
			if (! USBDevice->IsOpen()) 		
			{
				continue;
			}
			vID = USBDevice->VendorID;
			pID = USBDevice->ProductID;
			if(pID == 0x1002)
			{
				AppendText(IDC_EDIT_StatusBar, "USB-PID1002������! \r\n");
				isOpenDevice = true;
				//return;
			}		
			else
			{
				USBDevice->Close();
			}
			//usb�����˵㣨�����࣬���16����
			OutEndpt = USBDevice->EndPoints[1];
		}
	}
}


//************************************     
// ��������: OnBnClickedCycleShow     
// ����˵����ѭ��/��������ʾģʽ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedCycleShow()
{
	CButton* m_CycleShow  = (CButton*)GetDlgItem(IDC_RBTN_CycleShow);
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		m_CycleShow->SetCheck(FALSE);
		return;
	}
	
	if(m_CycleShow->GetCheck())
	{	//����ָ��
		SendCommand(0x08, 0x01);
		CString strtemp;    
		AppendText(IDC_EDIT_StatusBar, "ѭ����ʾģʽ!");
	}
	else
	{
		SendCommand(0x08, 0x00);
		CString strtemp;    
		AppendText(IDC_EDIT_StatusBar, "��������ʾģʽ!");
	}
}

//************************************     
// ��������: OnBnClickedFreqSet     
// ����˵�����ύ֡Ƶ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedFreqSet()
{
	UpdateData(TRUE);
	if (m_FreqSet == 0)
	{
		MessageBox("֡Ƶ����Ϊ0!");
		return;
	}
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(((m_FreqSet>>8) & 0x0f)+ 0x30, m_FreqSet & 0xff);
	CString strtemp;
	strtemp.Format(_T("DMD����֡���趨��%d"),m_FreqSet);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// ��������: OnBnClickedForShow     
// ����˵����DMD������ʾ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedForShow()
{
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	//SendCommand(0x10, 0x01);
	SendCommand(0x10, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD������ʾ!");
}


//************************************     
// ��������: OnBnClickedRevShow     
// ����˵����DMD������ʾ     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedRevShow()
{
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	//SendCommand(0x10, 0x00);
	SendCommand(0x10, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD������ʾ!");
}


//************************************     
// ��������: OnBnClickedCmdTrigger     
// ����˵���������     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedCmdTrigger()
{
	if(!IsOpenDevice()){
		return;
	}
	//���������ָ��
	SendCommand(0x18, 0x01);
	AppendText(IDC_EDIT_StatusBar, "�����!");
}


void CExperimentDlg::OnBnClickedRstdmd()
{
	if(!IsOpenDevice()){
		return;
	}
	//����DMD��λָ��
	SendCommand(0x04, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD��λ!");
}


//************************************     
// ��������: OnBnClickedAllOn     
// ����˵����DMDȫ��     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedAllOn()
{
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(0x1c, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMDȫ����ת��+12�ȣ���Ҫ����������������!");  
}


//************************************     
// ��������: OnBnClickedAllOff     
// ����˵����DMDȫ��    
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedAllOff()
{
	
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(0x20, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMDȫ����ת��-12�ȣ���Ҫ����������������!");
}


//************************************     
// ��������: OnBnClickedRevoke     
// ����˵����DMDȫ��ȫ��ģʽ����     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedRevoke()
{
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(0x20, 0x00);
	SendCommand(0x1c, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMDȫ��ȫ��ģʽ����!");
}

//************************************     
// ��������: OnBnClickedClearMem     
// ����˵��������ڴ�     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedClearMem()
{
	if(!IsOpenDevice()){
		return;
	}
	UpdateData(TRUE);
	//���͵�ַ��λָ��
	SendCommand(0x24, 0x01);
	
	unsigned char *obuf=new unsigned char[0x18000];   //768x1080 bits 
	ZeroMemory(obuf,0x18000);
	if (m_FrameCount == 0)
	{
		MessageBox("�������ú�֡��!");
		return;
	}

	for(int i=0;i<m_FrameCount;i++)
	{
		SendData(obuf);
	}
	//////////////////
	delete [] obuf;
	AppendText(IDC_EDIT_StatusBar, "�ڴ����!");
}


//************************************     
// ��������: OnBnClickedMemmag     
// ����˵������������     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedMemmag()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(((m_MemMag>>8) & 0x0f)+ 0x90, m_MemMag & 0xff);
	CString strtemp;
	strtemp.Format(_T("������ʾ�����趨��%d��\r\n������������ʾһ֡��"),m_MemMag);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// ��������: OnBnClickedReadAddr     
// ����˵��������ַ������һ�������źŵ�ʱ������ʾ�ڴ��е�m_MemReadAddr֡�����ݡ�     
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedReadAddr()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(((m_MemReadAddr>>8) & 0x0f)+ 0x60, m_MemReadAddr & 0xff);
	CString strtemp;
	strtemp.Format(_T("����ַ�趨(��λ֡):%d��\r\n������������ʾһ֡��"),m_MemReadAddr);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// ��������: OnBnClickedWriterAddr     
// ����˵����д��ַ����д���ڴ��е�֡������    
// �� �� ֵ: void     
//************************************  
void CExperimentDlg::OnBnClickedWriterAddr()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//����ָ��
	SendCommand(((m_MemWriterAddr>>8) & 0x0f)+ 0x70, m_MemWriterAddr & 0xff);
	CString strtemp;
	strtemp.Format(_T("д��ַ�趨(��λ֡):%d��"),m_MemWriterAddr);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}

//************************************     
// ��������: SendCommand    �Զ��庯��
// ����˵����ͨ��USB2.0�����������ֽ�ָ��      
// �� �� ֵ: void     
// ��    ��: UCHAR hbyte     ���ֽ�ָ��
// ��    ��: UCHAR lbyte     ���ֽ�ָ��
//************************************  
void CExperimentDlg::SendCommand(UCHAR hbyte, UCHAR lbyte ){
	unsigned char cmd_buf[2];
	cmd_buf[0] = lbyte;  //��λ
	cmd_buf[1] = hbyte;  //��λ
	
	OVERLAPPED OvLap;
	OvLap.hEvent  = CreateEvent(NULL, false, false, _T("CYUSB_CTL"));
	PUCHAR Context;
	LONG  length   = 0;
	CCyControlEndPoint  *ept = USBDevice->ControlEndPt;
	// cmd on
	ept->Target    = TGT_DEVICE;
	ept->ReqType   = REQ_VENDOR;
	ept->Direction = DIR_TO_DEVICE;
	ept->ReqCode   = 0xb0;
	ept->Value     = 0;
	ept->Index     = 0;

	Context = ept->BeginDataXfer(cmd_buf, length, &OvLap);
	ept->WaitForXfer(&OvLap,100);
	ept->FinishDataXfer(cmd_buf, length, &OvLap,Context);


	length = 2;  
	Context = OutEndpt->BeginDataXfer(cmd_buf, length, &OvLap);
	OutEndpt->WaitForXfer(&OvLap,1000);
	OutEndpt->FinishDataXfer(cmd_buf, length, &OvLap,Context);

	// cmd off
	
	ept->Target    = TGT_DEVICE;
	ept->ReqType   = REQ_VENDOR;
	ept->Direction = DIR_TO_DEVICE;
	ept->ReqCode   = 0xb1;
	ept->Value     = 0;
	ept->Index     = 0;
    length   = 0;
	Context = ept->BeginDataXfer(cmd_buf, length, &OvLap);
	ept->WaitForXfer(&OvLap,100);
	ept->FinishDataXfer(cmd_buf, length, &OvLap,Context);

	//
	CloseHandle(OvLap.hEvent);
}

//************************************     
// ��������: AppendText     �Զ��庯��
// ����˵���� ׷���ı���EditControl       
// �� �� ֵ: void     
// ��    ��: int controlId     
// ��    ��: CString strAdd     
//************************************  
void CExperimentDlg::AppendText(int controlId, CString strAdd)
{
	//�õ�ϵͳʱ��
	CTime time;  
	time = CTime::GetCurrentTime();  
	CString strTime = time.Format("[%X]");  

	((CEdit*)GetDlgItem(controlId))->SetSel(GetDlgItem(controlId)->GetWindowTextLength(),GetDlgItem(controlId)->GetWindowTextLength());
	((CEdit*)GetDlgItem(controlId))->ReplaceSel(strTime + strAdd+L"\r\n");
}

bool CExperimentDlg::IsOpenDevice(){
	if(!isOpenDevice){
		MessageBox("�豸δ���ӣ�");
		return false;
	}else{
		return true;
	}
}
//************************************     
// ��������: SelFilePath     �Զ��庯��
// ����˵����Ѱ���ļ��жԻ���     
// �� �� ֵ: CString �ļ���·��    
//************************************  
CString CExperimentDlg::SelFilePath()
{
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("��ѡ�������洢·��");  
	sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN;  
	sInfo.lpfn     = NULL;  

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  

	return strFolderPath;  
}
//************************************     
// ��������: ExpandData       �Զ��庯��
// ����˵������������չ��768*1024�ֽڣ���ÿһλ��չ��һ���ֽ�     
// �� �� ֵ: void     
// ��    ��: unsigned char * dmd_buf_org     
// ��    ��: unsigned char * dmd_buf_temp     
//************************************  
void CExperimentDlg::ExpandData(unsigned char *dmd_buf_org, unsigned char *dmd_buf_temp)
{
	//if(mode == NORMAL)
	{
		for(int i=0;i<768;i++)
			for(int j=0;j<128;j++)
				for(int k=0;k<8;k++)
					dmd_buf_temp[i*1024+8*j+k] = (dmd_buf_org[i*128+j]>>(7-k))&1;
	}
	/*else if(mode == SCAN)																	//��������չ��1024*FRAMECOUNT�ֽ�
	{
		for(int frame=0;frame<FRAMECOUNT;frame++)
			for(int column=0;column<128;column++)
				for(int k=0;k<8;k++)
					dmd_buf_temp[frame*1024+8*column+k] = (dmd_buf_org[frame*128+column]>>(7-k))&1;
	}*/
}


//************************************     
// ��������: OnTimer     
// ����˵�������ļ����ض�ʱ��     
// �� �� ֵ: void     
// ��    ��: UINT_PTR nIDEvent     
//************************************  
void CExperimentDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		{
			CString strtemp = NULL;
			CString strNameTemp = NULL;

			//���ض���ļ�
			if(IntStartIndex < IntEndIndex+1)
			{
				strNameTemp.Format("%d",IntStartIndex);
				strNameTemp = m_MultfileName + strNameTemp;

				IntStartIndex++;
				
				//�����ļ�
				DownLoad(strNameTemp, m_strSearchPath);
				
				//���͵�ַ��λָ��
				SendCommand(0x24, 0x01);
				AppendText(IDC_EDIT_StatusBar, "��ַ��λ!");
				
				//���Ϳ�ʼ��ʾָ��
				SendCommand(0x00, 0x01);
				AppendText(IDC_EDIT_StatusBar, "DMD��ʼ��ʾ!");
				
				
				strtemp.Format("�ļ�%s������ʾ..",strNameTemp);
				GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);
				
			}
			else
			{
				//����ļ��������
		
				char statusBuffer[NAME_MAX];
				memset(statusBuffer,0,NAME_MAX);
				strtemp.Format("�ļ�%s��ʾ���,�����ļ���ʾ���!",m_MultfileName);
				GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);

				//����DMDֹͣ��ʾָ��
				
				SendCommand(0x00, 0x00);
				AppendText(IDC_EDIT_StatusBar, "DMDֹͣ��ʾ!");
				
				AppendText(IDC_EDIT_StatusBar, "���ļ����ؽ���!");
				KillTimer(1);
			}  
			break;   		
		}
	}
	CDialog::OnTimer(nIDEvent);
}