
// Experiment.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExperimentApp:
// �йش����ʵ�֣������ Experiment.cpp
//

class CExperimentApp : public CWinAppEx
{
public:
	CExperimentApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExperimentApp theApp;