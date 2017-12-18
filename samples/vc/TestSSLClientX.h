#pragma once

class CTestSSLClientX
{
public:
	CTestSSLClientX(void);
	~CTestSSLClientX(void);

	/* ����SSL��Connect() */
	void SSLclient(LPCTSTR addr, int port);
	/* ����SSL��ConnectSocket() */
	void SSLclient2(LPCTSTR addr, int port);
	/* ����SSL��Connect(), ���߳� */
	void SSLclient3(LPCTSTR addr, int port);
	void SSLclient3Thread(CComPtr<NetONEX::ISSLClientX> s, LPCTSTR addr, int port);
	/* ˫��SSL */
	void SSLclient4(LPCTSTR addr, int port);

private:
	void SendRecv(CComPtr<NetONEX::ISSLClientX> s);
	HANDLE m_hMutex;
};

