#pragma once

// ���������õ�NetONEX.dll��·��
#import "../../../windows/x86/Release/NetONEX.dll" rename("DEBUG", "_DEBUG_")

class CTestNetONEX
{
public:
	CTestNetONEX(void);
	~CTestNetONEX(void);

	void Base64X();

private:
	NetONEX::IMainXPtr m_pMainX;
};

