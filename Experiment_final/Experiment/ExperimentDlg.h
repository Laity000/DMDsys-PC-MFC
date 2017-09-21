
// ExperimentDlg.h : 头文件
//

#pragma once
#include"serialport.h"
#include<stdarg.h>
#include "afxwin.h"
#include "CyAPI.h"
//#define FRAMECOUNT 620
#define LINECOUNT 768
#define COLUMNCOUNT 1024

#define NORMAL 1
#define SCAN  2
// CExperimentDlg 对话框
class CExperimentDlg : public CDialog
{
// 构造
public:
	CExperimentDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CExperimentDlg::SendData(unsigned char *dmd_buf);//注意该程序只能发送一帧的数据

	void CExperimentDlg::CompressData(unsigned char *ibuf, unsigned char *obuf);
	void CExperimentDlg::ExpandData(unsigned char *dmd_buf_org, unsigned char *dmd_buf_temp);
	void CExperimentDlg::readPoint();
	void CExperimentDlg::savePoint();
	char CExperimentDlg::GetBit(int count,UINT data);
	void CExperimentDlg::SendLine(int Line,unsigned char *dmd_buf_org_scan,unsigned char *dmd_buf_temp_scan,unsigned char *dmd_buf_scan,unsigned char *save_scan,unsigned char *dmd_buf_scan_all);//注意该程序只能发送一帧的数据
	void CExperimentDlg::DownLoad(CString str,CString strSearchPath);
	void CExperimentDlg::SendCommand(UCHAR hbyte, UCHAR lbyte);
	void CExperimentDlg::MessageBox(char *str,...);
	void CExperimentDlg::SetEditText(int nID,CString str);
	void CExperimentDlg::AppendText(int controlId, CString strAdd);
    bool CExperimentDlg::IsOpenDevice();
	CString CExperimentDlg::SelFilePath();
	int m_Mode;
	bool isOpenDevice;
	//正常和多文件下载的生成数据对比文件的保存路径
	CString m_strSavePath;
	//多文件下载的文件夹搜索路径
	CString m_strSearchPath;
	//选区坐标及路径配置文件所在的路径和名字location.txt
	CString m_strLocationPathAndName;
	//USB驱动对象
	CCyUSBDevice *USBDevice;	
	CCyUSBEndPoint *OutEndpt;
	CCyUSBEndPoint *InEndpt;
// 对话框数据
	enum { IDD = IDD_EXPERIMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	int SerialNumber;
	int angle;
	CString fileName;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedButton9();

	afx_msg void OnBnClickedFileDownload();

	afx_msg void OnBnClickedResetDMD();
	afx_msg void OnBnClickedShowDMD();
	afx_msg void OnBnClickedStopDMD();
	afx_msg void OnBnClickedSiteSelect();
	afx_msg void OnBnClickedRandomNumDownload();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedSiteUp();
	afx_msg void OnBnClickedSiteDown();
	afx_msg void OnBnClickedSiteLeft();
	afx_msg void OnBnClickedSiteRight();
//	afx_msg LRESULT OnComm(WPARAM ch,LPARAM nouse);
	UINT m_PluseWidth;
	afx_msg void OnBnClickedPluseWidth();
//	afx_msg void OnBnClickedChangemode();
//	afx_msg void OnStnClickedChangmode();
	afx_msg void OnBnClickedChangeMode();
	afx_msg void OnBnClickedInterSync();
	afx_msg void OnBnClickedExterSync();
	int m_FrameCount;
	afx_msg void OnBnClickedFrameCount();
	int FrameFre[30];
//	afx_msg void OnBnClickedSendcommand();
	afx_msg void OnBnClickedResetAddr();
	afx_msg void OnBnClickedSavePath();


//	afx_msg LONG OnComm(WPARAM ch,LPARAM no);
	//afx_msg void OnCbnSelchangeCombo3();
	
	afx_msg void OnBnClickedSearchPath();
	int m_StartIndex;
	int m_EndIndex;
	int IntStartIndex;
	int IntEndIndex;
//	CString strFileName;
//	CString m_strFileName;
	afx_msg void OnBnClickedMultfileDownload();
//	int editSerialNumber;
//	CString m_usb_num_sel;
	afx_msg void OnBnClickedUsbInit();
	CEdit m_EditSynShow;
	afx_msg void OnBnClickedCycleShow();
	int m_FreqSet;
	afx_msg void OnBnClickedFreqSet();
	afx_msg void OnBnClickedForShow();
	afx_msg void OnBnClickedRevShow();
	afx_msg void OnBnClickedCmdTrigger();
	afx_msg void OnBnClickedRstdmd();
	afx_msg void OnBnClickedAllOn();
	afx_msg void OnBnClickedAllOff();
	afx_msg void OnBnClickedRevoke();
	afx_msg void OnBnClickedClearMem();
	int m_MemMag;
	int m_MemReadAddr;
	int m_MemWriterAddr;
	afx_msg void OnBnClickedMemmag();
	afx_msg void OnBnClickedReadAddr();
	afx_msg void OnBnClickedWriterAddr();
	CString m_MultfileName;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
