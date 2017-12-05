#include "stdafx.h"
#include "TestNetONEX.h"
#include "TestSSLClientX.h"

using namespace NetONEX;

CTestNetONEX::CTestNetONEX(void)
{
	HRESULT hr;
	hr = m_pMainX.CoCreateInstance(__uuidof(NetONEX::MainX));
	if (!SUCCEEDED(hr))
	{
		MSGprintf(_T("��ʼ��ʵ��ʧ�ܣ�"));
		exit(-1);
	}
}


CTestNetONEX::~CTestNetONEX(void)
{
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

void CTestNetONEX::XMLsign() {
	ICertificateCollectionXPtr p = NULL;
	ICertificateXPtr x = NULL;

	p = m_pMainX->CreateCertificateCollectionXInstance();
	p->Load();
	x = p->SelectCertificateDialog();
	if (x) {
		BSTR out(_T("111111"));
		//MSGprintf(_T("\n%d"), x->VerifyPIN(out));
		MSGprintf(_T("\n%d"), x->VerifyPIN(_T("111112")));
		out = x->XMLSign(_T("<root><a>test</a></root>"));
		MSGprintf(_T("\n%s"), out);
	}
}

void CTestNetONEX::TSA(LPCTSTR addr, int port) {
	NetONEX::ITSAClientXPtr h = m_pMainX->CreateTSAClientXInstance();
	// ����ʱ�����������ַ
	h->ServerAddress = addr;
	// ����ʱ����������˿�
	h->ServerPort = port;
	// �����Ҫ��debug״̬, uncomment the next line
	// h->_DEBUG_ = 1;

	// ����ʱ���
	// �����ǩ�������ַ���������ֱ��ʹ����������������v
	// CComVariant v("abc")

	// �����ǩ��������һ���ڴ�飬�����Ǹ�unsigned char*�� ����ʹ������Ĵ���������v
	unsigned char t[] = { 0x01, 0x02, 0x00, 0x03 };
	CComSafeArray<unsigned char> a;
	a.Add(4, t, TRUE);
	CComVariant v(a);

	NetONEX::ITSAResponseXPtr resp = h->TSACreate(&v);
	if (resp) {
		cout << "BASE64�����ʱ������: " << endl;
		cout << (LPCSTR)CT2A(resp->ToBASE64()) << endl;

		// Ҳ���Ի�ȡʱ�����������Ϣ
		cout << (LPCSTR)CT2A(resp->Imprint) << endl;
		cout << resp->Timestamp << endl;

		//��֤ʱ���, ��base64�����ʱ�����Ϊ����
		long n = h->TSAVerify(resp->ToBASE64());
		MSGprintf(_T("result: %d. verify %s"), n, (n == 200) ? _T("OK") : _T("FAILED"));
	}
	else {
		MSGprintf(_T("create tsa failed."));
	}
}

void CTestNetONEX::Crt() {
	NetONEX::ICertificateCollectionXPtr crtlist = m_pMainX->CreateCertificateCollectionXInstance();
	crtlist->CryptoInterface = 3; // ͬʱ֧��CSP��SM2
	crtlist->Load();
	// crtlist->Size�����ǵ�ǰϵͳ�з���������֤����� ��ȱʡ�����Ǵ�˽Կ��֧��ǩ����
	cout << crtlist->Size << endl;
	if (crtlist->Size == 0) {
		cout << "no certificate found, exit" << endl;
		return;
	}
	NetONEX::ICertificateXPtr crt;
	if (crtlist->Size == 1) { // ֻ��һ������������֤�飬ֱ��ѡ������֤��
		crt = crtlist->GetAt(0);
	}
	else {
		// ʹ��֤��ѡ��Ի��������û�ѡ��֤��
		crt = crtlist->SelectCertificateDialog();
		if (!crt) {
			cout << "no certificate is selected, exit" << endl;
			return;
		}
	}
	// ��ʾ֤����Ϣ
	cout << "Friendly Name: " << crt->FriendlyName << endl; //FriendlyName��
	cout << "SerialNumber: " << crt->SerialNumber << endl; //���кţ�16���Ʊ�ʾ
	cout << "ThumbprintSHA1: " << crt->ThumbprintSHA1 << endl; //ָ�ƣ�sha1��ʽ��ʾ
	cout << "Content: " << crt->Content << endl; //֤�鱾��base64���뷽ʽ��ʾ
	// ���кܶ������ʾ�����ݣ���һһ�о���

	// ��svs��������ȥ��֤֤����Ч��
	NetONEX::ISVSClientXPtr svsx = m_pMainX->CreateSVSClientXInstance();
	svsx->ServerAddress = _T("192.168.161.161");
	if (200 == svsx->SVSVerifyCertificate(crt->Content)) {
		cout << "valid certificate" << endl;
	}
	else {
		cout << "invalid certificate" << endl;
	}

	//��ѡ�е�֤����һ��pkcs1��ǩ��
	CComBSTR origdata("abc");
	//crt->Quiet = 1;
	BSTR sigb64 = crt->PKCS1String(origdata.m_str);
	if (SysStringLen(sigb64) == 0) {
		cout << "create pkcs1 failed" << endl;
		return;
	}
	cout << sigb64 << endl;

	NetONEX::IBase64XPtr b64x = m_pMainX->CreateBase64XInstance();
	//���ǰ��ʹ����PKCS1Bytes�����ڷ��ص�ǩ���Ƕ����Ƹ�ʽ��������Ҫ�͵�SVS��ǩ������Ҫ�Ѷ����Ƶ�ǩ��ת����base64�����ʽ��string
	//_bstr_t sigb64 = b64x->EncodeBytes(sig); 

	//��ǩ���͵�SVS��������ȥ��ǩ
	_bstr_t origdatab64 = b64x->EncodeString(origdata.m_str); //ԭ�ı����Ⱦ���base64����
	if (200 == svsx->SVSVerifyPKCS1(crt->Content, sigb64, origdatab64)) {
		cout << "verify ok" << endl;
	}
	else {
		cout << "verify failed" << endl;
	}

	//������ʾһ����δ�֤�����к��ҵ���Ӧ��NetONEX::ICertificateXPtr����
	BSTR s0 = crt->SerialNumber;
	crtlist->Load();
	for (int n = 0; n < crtlist->Size; n ++) {
		NetONEX::ICertificateXPtr x = crtlist->GetAt(n);
		if (x) {
			BSTR s1 = x->SerialNumber;
			if (_tcscmp(s1, s0) == 0) {
				cout << "found! pos=" << n << endl;
				break;
			}
		}
	}
}

void CTestNetONEX::Run() {
	CTestSSLClientX c;
	c.SSLclient3(_T("www.baidu.com"), 443);
	//c.SSLclient2(_T("192.168.161.161"), 9108);
}