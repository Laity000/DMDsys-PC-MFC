
// ExperimentDlg.cpp : 实现文件
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
CSerialPort Port;//端口对象Port
void ChangeMap(unsigned char *ibuf, unsigned char *obuf);
bool StopPrint=false;
//bool hMutex_flag = TRUE;
//int FrameFrequency=0;//帧频
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
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

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
	//FrameFrequency=0;//帧频
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


// CExperimentDlg 消息处理程序
//************************************     
// 函数名称: readPoint     
// 函数说明：从配置文件中读取上一次程序保存的坐标及文件路径。注意：保存文件路径默认是在当前程序路径下     
// 返 回 值: void     
//************************************  
void CExperimentDlg::readPoint()		//初始化文件信息。
{
	FILE *fp;								
	m_startPoint.x=0;
	m_startPoint.y=0;
	m_finishPoint.x=0;
	m_finishPoint.y=0;

	
	
	if((fp=fopen((LPSTR)(LPCTSTR)m_strLocationPathAndName,"r"))!=NULL)//文件存在
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
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//在矩形框内
				reference[j*1024+i]=true;
			else
				reference[j*1024+i]=false;	
}
//************************************     
// 函数名称: savePoint     
// 函数说明：保存坐标及文件路径到配置文件，便于下次启动时提取     
// 返 回 值: void     
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
			if((i>=m_startPoint.x)&&(i<=m_finishPoint.x)&&(j>=m_startPoint.y)&&(j<=m_finishPoint.y))//在矩形框内
				reference[j*1024+i]=true;
			else
				reference[j*1024+i]=false;	
}

//************************************     
// 函数名称: OnInitDialog     
// 函数说明：对话框初始化     
// 返 回 值: BOOL     
//************************************  
BOOL CExperimentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	char buffer[PATH_MAX];
	GetCurrentDirectory(PATH_MAX,buffer);
	//生成数据对比文件的默认保存路径
	m_strSavePath.Format("%s\\",buffer);
	//配置文件路径及文件名
	m_strLocationPathAndName.Format("%s\\location.txt",buffer);//注意//是转义字符
	readPoint();
	//初始化矩形的两个特征点
	SetDlgItemText(IDC_BTN_ChangeMode,"正常模式，点此切换");

	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowTextA("未下载任何数据");
	
	SetEditText(IDC_EDIT_SavePath,m_strSavePath);

	//SetEditText(IDC_EDIT_LeftUP,m_strSearchPath);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExperimentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExperimentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//************************************     
// 函数名称: DownLoad     自定义函数
// 函数说明：将数据文件一帧一帧地发送到DMD中，同时生成ASCILL对比文件     
// 返 回 值: void    
// 参    数: CString strFileName 文件名(不带后缀)    
// 参    数: CString strFilePath 文件路径    
//************************************  
void CExperimentDlg::DownLoad(CString strFileName,CString strFilePath)
{
	//定义一些数组
	
	unsigned char *p;
	unsigned char *dmd_buf_org		= new unsigned char[m_FrameCount*LINECOUNT*COLUMNCOUNT/8];//原始数据
	unsigned char *dmd_buf_temp		= new unsigned char[LINECOUNT*COLUMNCOUNT];  //临时数据：用于区域选择	一次下载一帧
	unsigned char *dmd_buf			= new unsigned char[LINECOUNT*COLUMNCOUNT/8];  //最终发送数据

	memset(dmd_buf,0,LINECOUNT*COLUMNCOUNT/8);
	memset(dmd_buf_temp,0,LINECOUNT*COLUMNCOUNT);
	memset(dmd_buf_org,0,m_FrameCount*LINECOUNT*COLUMNCOUNT/8);

	//打开选择的文件
	CString strFileNameAndPath;
	strFileNameAndPath = strFilePath + strFileName;
	strFileNameAndPath += ".txt";
	CFile file(strFileNameAndPath,CFile::modeRead);
	
	//将文件下载到dmd_buf_org中
	if(file.GetLength() >= m_FrameCount*LINECOUNT*COLUMNCOUNT/8)//文件大小满足要求
		file.Read(dmd_buf_org,m_FrameCount*LINECOUNT*COLUMNCOUNT/8);
	else
		file.Read(dmd_buf_org,file.GetLength());
	file.Close();

	CString strtemp;
	strtemp.Format("正在下载文件%s...",strFileName); 
	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);

	//////////////////////////////////////////////////////////////////////////
	int savecount = 2*1024*768+768;//每行需要加上加上回车符
	unsigned char *save = new unsigned char[savecount];	//用于存储正常模式下的保存数据
	memset(save,'0',savecount);//以ascii码存储，
	//生成写入文件的地址和格式str    text1_234_42.txt
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
	//m_strSavePath为保存文件的目录地址，在构造函数中设置。
	CFile fileSave;
	if (fileSave.Open(StrAscillFileNameAndPath,CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		MessageBox(m_strSavePath + "打开失败！");
		return ;
	}
	
	
 
	//////////////////////////////////////////////////////////////////////////
	//开始下载数据+保存比对文件
	p=dmd_buf_org;
	for(int count=0;count<m_FrameCount;count++)//产生600帧的数据
	{	
		//区域选择生成dmd_buf_temp数组****************************************************
		ExpandData(p,dmd_buf_temp);//扩展数据到1024*768字节
		if(m_startPoint!=m_finishPoint)//用于区域选择
		{
			for(int i=0;i<1024*768;i++)
			dmd_buf_temp[i]&=reference[i];
		}else{
			MessageBox("选区不合法，请重新选择！");
			return;
		}
		//将dmd_buf_temp中有用的数据转换成ASCII后存入save中
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
		//将save数组写入文件 ******
		fileSave.SeekToEnd();
		fileSave.Write(save,savecount);
		
		//将数据回写
		CompressData(dmd_buf_temp,dmd_buf);		//收缩数据到768*1024/8字节
		//将数据下载到板子中
		SendData(dmd_buf);//下载一帧的随机数    p为要下载的数据、dmd_buf_temp为扩展后的临时数据、dmd_buf为实际下载的数据
		p+=LINECOUNT*COLUMNCOUNT/8;//指向下一帧
	}
	fileSave.Close();

	delete[](save);
	
	delete[](dmd_buf_temp);		//下载时产生的临时数据
	delete[](dmd_buf);			//最终要下载的数据
	delete[](dmd_buf_org);		//要下载的数据

	strtemp.Format("文件%s下载完成!",strFileName); 
	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);
}

//************************************     
// 函数名称: OnBnClickedFileDownload     
// 函数说明：打开文件对话框，下载数据文件到DMD中     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedFileDownload()
{
	UpdateData(TRUE);
	
	if(!IsOpenDevice()){
		return;
	}
	
	
	if(m_FrameCount == 0 ){
		MessageBox("帧数不能为0!");
		return;
	}
	CFileDialog fileDialog(true);
	fileDialog.m_ofn.lpstrTitle="打开文件";
	fileDialog.m_ofn.lpstrFilter="Text(*.txt)\0*.txt\0All File(*.*)\0*.*\0\0";
	if(IDOK!=fileDialog.DoModal())
		return;
	//对文件名进行部分处理  获取文件的文件名，将最后的.txt去掉，只用文件名例如data.txt获得的文件名为data存入str中
	CString strFileName;
	strFileName=fileDialog.GetFileName();
	strFileName.Delete(strFileName.ReverseFind('.'),strFileName.GetLength()-strFileName.ReverseFind('.'));
	
	//得到文件路径
	CString strFilePath=fileDialog.GetPathName();
	
	/*
	CString strtemp;
	strtemp.Format("%s",strFilePath);
	AppendText(IDC_EDIT_StatusBar,strtemp);
	*/
	strFilePath.Delete(strFilePath.ReverseFind('\\')+1,strFilePath.GetLength()-strFilePath.ReverseFind('\\')-1);
	m_strSearchPath = strFilePath;
	GetDlgItem(IDC_EDIT_SearchPath)->SetWindowTextA(m_strSearchPath);
	
	SendCommand(0x00, 0x00);//停止显示;
	
	AppendText(IDC_EDIT_StatusBar, "DMD停止显示!");
	
	DownLoad(strFileName,strFilePath);
	MessageBox("下载完毕");
	
}
//未修改
void CExperimentDlg::SendLine(int Line,unsigned char *dmd_buf_org_scan,unsigned char *dmd_buf_temp_scan,unsigned char *dmd_buf_scan,unsigned char *save_scan,unsigned char *dmd_buf_org)
{
	ExpandData(dmd_buf_org_scan,dmd_buf_temp_scan);//扩展数组到FEAMECOUNT*1024字节 用于区域选择


	if(m_startPoint!=m_finishPoint)
	{
		for(int frame = 0;frame < m_FrameCount;frame++)//对于每一行的每一帧
			for(int column = 0;column < COLUMNCOUNT;column++)
				dmd_buf_temp_scan[column+frame*COLUMNCOUNT] &= reference[column+Line*1024];
	}

	CompressData(dmd_buf_temp_scan,dmd_buf_scan);		//收缩数组到FRAMECOUNT*128字节

   	int i,j;	

	for(i=0,j=0;i<m_FrameCount*COLUMNCOUNT;i++)
		{
			save_scan[j]=dmd_buf_temp_scan[i] + '0';
			j++;
			save_scan[j] = ' ';
			j++;
		}

	
	//将一行的dmd_buf_scan的内容存到dmd_buf_org中
	
	for(int i=0;i<m_FrameCount*COLUMNCOUNT/8;i++)
		dmd_buf_org[Line*m_FrameCount*COLUMNCOUNT/8+i] = dmd_buf_scan[i];

	if(Line == (LINECOUNT-1))
	{
		unsigned char *p=dmd_buf_org;
		for(int frame=0;frame<m_FrameCount;frame++)
		{
			long length = LINECOUNT*COLUMNCOUNT/8;//最终要发送的数据长度
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
// 函数名称: OnBnClickedResetDMD     
// 函数说明：DMD复位     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedResetDMD()
{
	
	if(!IsOpenDevice()){
		return;
	}
	//发送DMD复位指令
	SendCommand(0x04, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD复位!");
}

//************************************     
// 函数名称: OnBnClickedShowDMD     
// 函数说明：DMD开始显示     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedShowDMD()//单击开始显示按钮
{
	if(!IsOpenDevice()){
		return;
	}
	//发送开始显示指令
	SendCommand(0x00, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD开始显示!");
}

//************************************     
// 函数名称: OnBnClickedStopDMD     
// 函数说明：DMD停止显示     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedStopDMD()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送停止显示指令
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD停止显示!");
}

//************************************     
// 函数名称: OnBnClickedSiteSelect     
// 函数说明：单击选择区域按钮，没有被选中的区域所有数据为0     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	//flagDraw=true;
	draw.DoModal();
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
	savePoint();
	//readPoint();
	OnPaint();
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}


//************************************     
// 函数名称: SendData     自定义函数
// 函数说明：发送数据    
// 返 回 值: void     
// 参    数: unsigned char * dmd_buf     
//************************************  
void CExperimentDlg::SendData(unsigned char *dmd_buf)//注意该程序只能发送一帧的数据
{	
	long length = 1024*96;//最终要发送的数据长度
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
// 函数名称: CompressData   自定义函数
// 函数说明：将768*1024字节的数据收缩到768*128字节的数据，即一字节收缩到一位    
// 返 回 值: void     
// 参    数: unsigned char * ibuf     
// 参    数: unsigned char * obuf     
//************************************  
void CExperimentDlg::CompressData(unsigned char *ibuf, unsigned char *obuf)
{
    int m,i,j,k;
    int tt;
//	if(mode == NORMAL)								//将768*1024字节的数据收缩到768*128字节的数据
	{
		ZeroMemory(obuf, 128*768);

		for(m=0;m<768;m++) //以下是分别对768行的1028位数据处理
			for(j=0;j<16;j++) //分为16个时钟周期
				for(i=0;i<8;i++) //每个周期的64位数据分为8个字节处理，
					             //每位隔16位微镜处理，那么一个字节隔128位微镜处理
				{
					tt = i;
					for(k=0;k<8;k++)
						obuf[128*m + 8*j + i] = (obuf[128*m + 8*j + i] << 1) | (ibuf[1024*m - j + 128*i + (127-16*k)] & 1);
				 }
	}
/*	else if(mode == SCAN)						//将FRAMECOUNT*1024字节的数据收缩到FRAMECOUNT*128字节的数据 
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
// 函数名称: OnBnClickedRandomNumDownload     
// 函数说明：下载伪随机数     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedRandomNumDownload()//单击下载伪随机数
{
	UpdateData(TRUE);
	
	if(!IsOpenDevice()){
		return;
	}
	
	if(m_FrameCount == 0 ){
		MessageBox("帧数不能为0!");
		return;
	}
	//定义一些数组
	unsigned char *dmd_buf_org		=	new unsigned char[LINECOUNT*COLUMNCOUNT/8];		//正常模式下一次下载的原始数据
	unsigned char *dmd_buf_org_scan =	new unsigned char[m_FrameCount*COLUMNCOUNT/8];	//扫描模式下一次下载的原始数据

	unsigned char *dmd_buf_temp		=	new unsigned char[LINECOUNT*COLUMNCOUNT];		//正常模式下的临时数据：用于区域选择
	unsigned char *dmd_buf_temp_scan=	new unsigned char[m_FrameCount*COLUMNCOUNT];		//扫描模式下的临时数据：用于区域选择

	unsigned char *dmd_buf			=	new unsigned char[LINECOUNT*COLUMNCOUNT/8];		//正常模式下最终发送数据
	int savecount =2*1024*768+768;//每行需要加上加上回车符
	unsigned char *save				=	new unsigned char[savecount];	//正常模式下的数据保存
	unsigned char *save_scan		=	new unsigned char[2*m_FrameCount*COLUMNCOUNT];	//扫描模式下的数据保存

	unsigned char *dmd_buf_scan_all	=	new unsigned char[m_FrameCount*LINECOUNT*COLUMNCOUNT/8];	//扫描方式下最终全部发送的数据
	unsigned char *p;
	
	//初始化这些数组
	memset(dmd_buf_org		,0,	LINECOUNT*COLUMNCOUNT/8				);
	memset(dmd_buf_org_scan	,0,	m_FrameCount*COLUMNCOUNT/8			);
	memset(dmd_buf_temp		,0,	LINECOUNT*COLUMNCOUNT				);
	memset(dmd_buf_temp_scan,0,	m_FrameCount*COLUMNCOUNT				);
	memset(dmd_buf			,0,	LINECOUNT*COLUMNCOUNT/8				);
	memset(save				,'0',	savecount			);
	memset(save_scan		,'0',	2*m_FrameCount*COLUMNCOUNT			);
	memset(dmd_buf_scan_all	,0,	m_FrameCount*LINECOUNT*COLUMNCOUNT/8	);

	SYSTEMTIME time;

	
    //下载前停止显示
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD停止显示!");

	GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText("下载伪随机数中..");
	//MessageBox("开始下载伪随机数");		

	if(m_Mode == NORMAL)				//下载伪随机数
	{
		//得到随机数对比文件名
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
				for(int j=0;j<128;j++)	//128=1024/8		一次调用产生一字节的随机数
				{
					GetSystemTime(&time);
					srand(j*time.wMilliseconds);
					dmd_buf_org[i*128+j]=rand();
				}

			ExpandData(dmd_buf_org,dmd_buf_temp);//扩展数组到1024*768用于区域选择
			if(m_startPoint!=m_finishPoint)
			{
				for(int i=0;i<1024*768;i++)
					dmd_buf_temp[i]&=reference[i];
			}
			else
			{
				MessageBox("选区非法，请重新选择！");
				return;
			}
			//将dmd_buf_temp中有用的数据转换成ASCII后存入save中
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

			//将save数组写入文件 ******
			fileSave.SeekToEnd();
			fileSave.Write(save,savecount);
			//将数据回写
			CompressData(dmd_buf_temp,dmd_buf);		//收缩数组到1024*96
			//将数据下载到板子中
			SendData(dmd_buf);//下载一帧的随机数    p为要下载的数据、dmd_buf_temp为扩展后的临时数据、dmd_buf为实际下载的数据
		}
		fileSave.Close();
		
		GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText("伪随机数下载完毕！");
		MessageBox("下载完毕");
	}
	else if(m_Mode == SCAN)				//下载伪随机数
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

		if(!PathFileExists(strPath_temp))//如果不存在
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
			//扩展
			for(int frame=0;frame<m_FrameCount;frame++)
				for(int column=0;column<128;column++)
					for(int k=0;k<8;k++)
						dmd_buf_temp_scan[frame*1024+8*column+k] = (dmd_buf_org_scan[frame*128+column]>>(7-k))&1;
			//截图
			if(m_startPoint!=m_finishPoint)
			{
				for(int frame = 0;frame < m_FrameCount;frame++)//对于每一行的每一帧
					for(int column = 0;column < COLUMNCOUNT;column++)
						dmd_buf_temp_scan[column+frame*COLUMNCOUNT] &= reference[column+Line*1024];
			}
			//生成写入文件的格式并保存数据的内容。
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
		
			//收缩为FRAMECOUNT*1024/8字节
			for(int frame=0;frame<m_FrameCount;frame++)
					for(int column=0;column<128;column++)
						for(int k=0;k<8;k++)
							dmd_buf_scan_all[Line*m_FrameCount*128+frame*128+column] = (dmd_buf_scan_all[Line*m_FrameCount*128+frame*128+column] << 1) | (dmd_buf_temp_scan[frame*COLUMNCOUNT+column*8+k] & 0x01);
		}
		p = dmd_buf_scan_all;
		for(int count=0;count<m_FrameCount;count++)//产生FRAMECOUNT帧的数据
		{	
			ExpandData(p,dmd_buf_temp);//扩展数组到1024*768用于区域选择
			CompressData(dmd_buf_temp,dmd_buf);		//收缩数组到1024*96
			SendData(dmd_buf);//下载一帧的随机数    p为要下载的数据、dmd_buf_temp为扩展后的临时数据、dmd_buf为实际下载的数据
			p+=LINECOUNT*COLUMNCOUNT/8;//指向下一帧
		}
	}

	//CloseUSB();

	//下载完毕后需要复位地址,需要修改
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
// 函数名称: OnBnClickedCancel     
// 函数说明：取消  
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//************************************     
// 函数名称: OnBnClickedOK     
// 函数说明：确定     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedOK()
{
	// TODO: 在此添加控件通知处理程序代码
//	hMutex_flag = FALSE;
	//OnOK();
}

//************************************     
// 函数名称: OnBnClickedSiteUp     
// 函数说明：选区坐标向上一格     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteUp()//上
{
	// TODO: 在此添加控件通知处理程序代码
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
// 函数名称: OnBnClickedSiteDown     
// 函数说明：选区坐标向下一格        
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteDown()//下
{
	// TODO: 在此添加控件通知处理程序代码
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
// 函数名称: OnBnClickedSiteLeft     
// 函数说明：选区坐标向左一格        
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteLeft()//左
{
	// TODO: 在此添加控件通知处理程序代码
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
// 函数名称: OnBnClickedSiteRight     
// 函数说明：选区坐标向右一格        
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedSiteRight()//右
{
	// TODO: 在此添加控件通知处理程序代码
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

//未修改
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
// 函数名称: OnBnClickedPluseWidth     
// 函数说明：提交脉宽     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedPluseWidth() //PluseWidth
{
	UpdateData(TRUE);
	if (m_PluseWidth == 0)
	{
		MessageBox("脉宽不能为0!");
		return;
	}
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(((m_PluseWidth>>8) & 0x0f)+ 0x40, m_PluseWidth & 0xff);
	CString strtemp;
	strtemp.Format(_T("同步输出脉宽设定：%d"),m_PluseWidth);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}

//************************************     
// 函数名称: OnBnClickedChangeMode     
// 函数说明：改变数据下载模式(目前只能此时正常模式)     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedChangeMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Mode == NORMAL)
	{
		//用于显示给用户看
		//GetDlgItem(IDC_BTN_ChangeMode)->SetWindowTextA("扫描模式，点此切换");
		MessageBox("扫描模式功能未添加！");
		return;
		m_Mode = SCAN;
		//切换为扫描模式
		//1、停止显示
		//char ch = 0x80;
		//Port.WriteToPort(&ch,1);
		//for(int i=0;i<50000;i++);
		//2、切换为扫描模式
		//ch = 0xe0;
		//Port.WriteToPort(&ch,1);

		//3、下载数据
		//OnBnClickedButton2();	//相当于点击下载数据按钮
	}
	else
	{
		//用于显示给用户看
		m_Mode = NORMAL;
		SetDlgItemText(IDC_BTN_ChangeMode,"正常模式，点此切换");

		//切换为正常模式
		//1、停止显示
		//char ch = 0x80;
		//Port.WriteToPort(&ch,1);
	
		//2、切换为正常模式
		//for(int i=0;i<50000;i++);
		//ch = 0xe1;
		//Port.WriteToPort(&ch,1);

		//3、下载数据
		//OnBnClickedButton2();	//相当于点击下载数据按钮

	}
}

//************************************     
// 函数名称: OnBnClickedInterSync     
// 函数说明：内同步设置    
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedInterSync()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送内同步指令
	SendCommand(0x0c, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD内同步!");
}

//************************************     
// 函数名称: OnBnClickedExterSync     
// 函数说明：外同步设置     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedExterSync()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送外同步指令
	SendCommand(0x0c, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD外同步!");
}

//************************************     
// 函数名称: OnBnClickedFramcount     
// 函数说明：提交修改的帧数，    
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedFrameCount()//修改帧频  
{
	UpdateData(TRUE);

	if(m_FrameCount == 0 ){
		MessageBox("帧数不能为0!");
		return;
	}
	
	if(!IsOpenDevice()){
		return;
	}
	
	//发送指令
	SendCommand(((m_FrameCount>>8) & 0x07)+ 0x50, m_FrameCount & 0xff);

	CString strtemp;
	strtemp.Format(_T("帧数：%d"),m_FrameCount);     
	AppendText(IDC_EDIT_StatusBar, strtemp);

	
	//cmd_buf[0] = (m_FrameCount>>11) & 0xff;       //低位
	//cmd_buf[1] = ((m_FrameCount>>19) & 0x07)+ 0x58;  //高位
}


//************************************     
// 函数名称: OnBnClickedResetAddr     
// 函数说明：内存地址复位     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedResetAddr()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送地址复位指令
	SendCommand(0x24, 0x01);
	AppendText(IDC_EDIT_StatusBar, "地址复位!");
}

//************************************     
// 函数名称: OnBnClickedSavePath    
// 函数说明：设置多文件下载的搜索路径     
// 返 回 值: void     
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
// 函数名称: SetEditText    自定义函数 
// 函数说明：给对应的文本框显示字符串,并根据长度自动换行
// 返 回 值: void     
// 参    数: int nID  文本编辑框ID   
// 参    数: CString str     待显示字符串
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
// 函数名称: OnBnClickedSearchPath     
// 函数说明：设置多文件下载的搜索路径    
// 返 回 值: void     
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
// 函数名称: OnBnClickedMultfileDownload     
// 函数说明：多文件下载，同时保持ASCILL对比文件     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedMultfileDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(m_FrameCount == 0 ){
		MessageBox("帧数不能为0!");
		return;
	}

	if(m_FreqSet == 0 ){
		MessageBox("帧频不能为0!");
		return;
	}

	IntStartIndex = m_StartIndex;
	IntEndIndex = m_EndIndex;
	if(m_MultfileName.IsEmpty()|| m_MultfileName.ReverseFind('.') != -1 || m_EndIndex==-1 || m_StartIndex >= m_EndIndex +1)
	{
		MessageBox("请输入正确的数字范围或文件名(不带后缀）");
		return;
	}

	//发送停止显示指令
	
	SendCommand(0x00, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD停止显示!");

	SetTimer(1, 1000/m_FreqSet*m_FrameCount*1.5, NULL); 
	
	//SetTimer(1, 3000, NULL); 
	
	MessageBox("下载完毕");
}




//************************************     
// 函数名称: OnBnClickedUsbInit     
// 函数说明：初始化USB驱动，查找并打开DMD设备     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedUsbInit()
{
	//初始化usb驱动
	USBDevice =  new CCyUSBDevice(USBHandle); 

	int devices = USBDevice->DeviceCount();
	if(devices == 0)
	{		
		AppendText(IDC_EDIT_StatusBar, "USB未连接，无法打开设备！");
		return;
	}

	CString strtemp;
	strtemp.Format(_T("发现%d\组USB设备，\r\nDMD正在连接.."), devices);
	AppendText(IDC_EDIT_StatusBar, strtemp);
	for(int i=0;i<devices;i++)
	{
		int vID;
		int pID;
		//打开所有设备
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
				AppendText(IDC_EDIT_StatusBar, "USB-PID1002已连接! \r\n");
				isOpenDevice = true;
				//return;
			}		
			else
			{
				USBDevice->Close();
			}
			//usb驱动端点（抽象类，最多16个）
			OutEndpt = USBDevice->EndPoints[1];
		}
	}
}


//************************************     
// 函数名称: OnBnClickedCycleShow     
// 函数说明：循环/单周期显示模式     
// 返 回 值: void     
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
	{	//发送指令
		SendCommand(0x08, 0x01);
		CString strtemp;    
		AppendText(IDC_EDIT_StatusBar, "循环显示模式!");
	}
	else
	{
		SendCommand(0x08, 0x00);
		CString strtemp;    
		AppendText(IDC_EDIT_StatusBar, "单周期显示模式!");
	}
}

//************************************     
// 函数名称: OnBnClickedFreqSet     
// 函数说明：提交帧频     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedFreqSet()
{
	UpdateData(TRUE);
	if (m_FreqSet == 0)
	{
		MessageBox("帧频不能为0!");
		return;
	}
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(((m_FreqSet>>8) & 0x0f)+ 0x30, m_FreqSet & 0xff);
	CString strtemp;
	strtemp.Format(_T("DMD播放帧率设定：%d"),m_FreqSet);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// 函数名称: OnBnClickedForShow     
// 函数说明：DMD正向显示     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedForShow()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	//SendCommand(0x10, 0x01);
	SendCommand(0x10, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD正向显示!");
}


//************************************     
// 函数名称: OnBnClickedRevShow     
// 函数说明：DMD反向显示     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedRevShow()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	//SendCommand(0x10, 0x00);
	SendCommand(0x10, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD反向显示!");
}


//************************************     
// 函数名称: OnBnClickedCmdTrigger     
// 函数说明：命令触发     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedCmdTrigger()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送命令触发指令
	SendCommand(0x18, 0x01);
	AppendText(IDC_EDIT_StatusBar, "命令触发!");
}


void CExperimentDlg::OnBnClickedRstdmd()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送DMD复位指令
	SendCommand(0x04, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD复位!");
}


//************************************     
// 函数名称: OnBnClickedAllOn     
// 函数说明：DMD全开     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedAllOn()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(0x1c, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD全部翻转到+12度，需要撤销才能正常工作!");  
}


//************************************     
// 函数名称: OnBnClickedAllOff     
// 函数说明：DMD全关    
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedAllOff()
{
	
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(0x20, 0x01);
	AppendText(IDC_EDIT_StatusBar, "DMD全部翻转到-12度，需要撤销才能正常工作!");
}


//************************************     
// 函数名称: OnBnClickedRevoke     
// 函数说明：DMD全开全关模式撤销     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedRevoke()
{
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(0x20, 0x00);
	SendCommand(0x1c, 0x00);
	AppendText(IDC_EDIT_StatusBar, "DMD全开全关模式撤销!");
}

//************************************     
// 函数名称: OnBnClickedClearMem     
// 函数说明：清空内存     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedClearMem()
{
	if(!IsOpenDevice()){
		return;
	}
	UpdateData(TRUE);
	//发送地址复位指令
	SendCommand(0x24, 0x01);
	
	unsigned char *obuf=new unsigned char[0x18000];   //768x1080 bits 
	ZeroMemory(obuf,0x18000);
	if (m_FrameCount == 0)
	{
		MessageBox("请先设置好帧数!");
		return;
	}

	for(int i=0;i<m_FrameCount;i++)
	{
		SendData(obuf);
	}
	//////////////////
	delete [] obuf;
	AppendText(IDC_EDIT_StatusBar, "内存清空!");
}


//************************************     
// 函数名称: OnBnClickedMemmag     
// 函数说明：滚动步数     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedMemmag()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(((m_MemMag>>8) & 0x0f)+ 0x90, m_MemMag & 0xff);
	CString strtemp;
	strtemp.Format(_T("滚动显示步数设定：%d，\r\n按触发命令显示一帧。"),m_MemMag);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// 函数名称: OnBnClickedReadAddr     
// 函数说明：读地址：当下一个触发信号到时，将显示内存中第m_MemReadAddr帧的数据。     
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedReadAddr()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(((m_MemReadAddr>>8) & 0x0f)+ 0x60, m_MemReadAddr & 0xff);
	CString strtemp;
	strtemp.Format(_T("读地址设定(单位帧):%d，\r\n按触发命令显示一帧。"),m_MemReadAddr);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}


//************************************     
// 函数名称: OnBnClickedWriterAddr     
// 函数说明：写地址：将写入内存中第帧的数据    
// 返 回 值: void     
//************************************  
void CExperimentDlg::OnBnClickedWriterAddr()
{
	UpdateData(TRUE);
	if(!IsOpenDevice()){
		return;
	}
	//发送指令
	SendCommand(((m_MemWriterAddr>>8) & 0x0f)+ 0x70, m_MemWriterAddr & 0xff);
	CString strtemp;
	strtemp.Format(_T("写地址设定(单位帧):%d。"),m_MemWriterAddr);     
	AppendText(IDC_EDIT_StatusBar, strtemp);
}

//************************************     
// 函数名称: SendCommand    自定义函数
// 函数说明：通过USB2.0驱动发送两字节指令      
// 返 回 值: void     
// 参    数: UCHAR hbyte     高字节指令
// 参    数: UCHAR lbyte     低字节指令
//************************************  
void CExperimentDlg::SendCommand(UCHAR hbyte, UCHAR lbyte ){
	unsigned char cmd_buf[2];
	cmd_buf[0] = lbyte;  //低位
	cmd_buf[1] = hbyte;  //高位
	
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
// 函数名称: AppendText     自定义函数
// 函数说明： 追加文本到EditControl       
// 返 回 值: void     
// 参    数: int controlId     
// 参    数: CString strAdd     
//************************************  
void CExperimentDlg::AppendText(int controlId, CString strAdd)
{
	//得到系统时间
	CTime time;  
	time = CTime::GetCurrentTime();  
	CString strTime = time.Format("[%X]");  

	((CEdit*)GetDlgItem(controlId))->SetSel(GetDlgItem(controlId)->GetWindowTextLength(),GetDlgItem(controlId)->GetWindowTextLength());
	((CEdit*)GetDlgItem(controlId))->ReplaceSel(strTime + strAdd+L"\r\n");
}

bool CExperimentDlg::IsOpenDevice(){
	if(!isOpenDevice){
		MessageBox("设备未连接！");
		return false;
	}else{
		return true;
	}
}
//************************************     
// 函数名称: SelFilePath     自定义函数
// 函数说明：寻找文件夹对话框     
// 返 回 值: CString 文件夹路径    
//************************************  
CString CExperimentDlg::SelFilePath()
{
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("请选择处理结果存储路径");  
	sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
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
// 函数名称: ExpandData       自定义函数
// 函数说明：将数据扩展到768*1024字节，即每一位扩展成一个字节     
// 返 回 值: void     
// 参    数: unsigned char * dmd_buf_org     
// 参    数: unsigned char * dmd_buf_temp     
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
	/*else if(mode == SCAN)																	//将数组扩展到1024*FRAMECOUNT字节
	{
		for(int frame=0;frame<FRAMECOUNT;frame++)
			for(int column=0;column<128;column++)
				for(int k=0;k<8;k++)
					dmd_buf_temp[frame*1024+8*column+k] = (dmd_buf_org[frame*128+column]>>(7-k))&1;
	}*/
}


//************************************     
// 函数名称: OnTimer     
// 函数说明：多文件下载定时器     
// 返 回 值: void     
// 参    数: UINT_PTR nIDEvent     
//************************************  
void CExperimentDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		{
			CString strtemp = NULL;
			CString strNameTemp = NULL;

			//下载多个文件
			if(IntStartIndex < IntEndIndex+1)
			{
				strNameTemp.Format("%d",IntStartIndex);
				strNameTemp = m_MultfileName + strNameTemp;

				IntStartIndex++;
				
				//下载文件
				DownLoad(strNameTemp, m_strSearchPath);
				
				//发送地址复位指令
				SendCommand(0x24, 0x01);
				AppendText(IDC_EDIT_StatusBar, "地址复位!");
				
				//发送开始显示指令
				SendCommand(0x00, 0x01);
				AppendText(IDC_EDIT_StatusBar, "DMD开始显示!");
				
				
				strtemp.Format("文件%s正在显示..",strNameTemp);
				GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);
				
			}
			else
			{
				//多个文件下载完成
		
				char statusBuffer[NAME_MAX];
				memset(statusBuffer,0,NAME_MAX);
				strtemp.Format("文件%s显示完毕,所有文件显示完毕!",m_MultfileName);
				GetDlgItem(IDC_EDIT_ProgressBar)->SetWindowText(strtemp);

				//发送DMD停止显示指令
				
				SendCommand(0x00, 0x00);
				AppendText(IDC_EDIT_StatusBar, "DMD停止显示!");
				
				AppendText(IDC_EDIT_StatusBar, "多文件下载结束!");
				KillTimer(1);
			}  
			break;   		
		}
	}
	CDialog::OnTimer(nIDEvent);
}