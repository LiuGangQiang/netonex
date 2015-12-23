#include "stdafx.h"
#include "TestNetONEX.h"

using namespace NetONEX;

CTestNetONEX::CTestNetONEX(void)
{
	m_pMainX = NULL;
	HRESULT hr;

	hr = m_pMainX.CreateInstance(__uuidof(NetONEX::MainX));
	if (!SUCCEEDED(hr))
	{
		MSGprintf(_T("��ʼ��ʵ��ʧ�ܣ�"));
		exit(-1);
	}
}


CTestNetONEX::~CTestNetONEX(void)
{
	if (m_pMainX) {
		m_pMainX->Release();
	}
}

void CTestNetONEX::Base64X() {
	IBase64XPtr pBase64X = NULL;
	pBase64X = m_pMainX->CreateBase64XInstance();
	BSTR b64(_T("1tDOxGFiY2Q="));
	BSTR out;
	out = pBase64X->DecodeHexString(b64);
	ATL::CString x(out);
	MSGprintf(_T("�����%s"), x);
}