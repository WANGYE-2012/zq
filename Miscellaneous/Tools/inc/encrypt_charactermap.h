#ifndef ENCRYPT_CHARACTERMAP_H_
#define ENCRYPT_CHARACTERMAP_H_

//����һ���ַ�ӳ����Լ����㷨
//1. ���Ǹ���ʵ����


#ifdef WIN32
#pragma managed(push,off)
#endif 



class CEncrypt_Charactermap
{
public:
	//ȡ���󣬱�Ҫʱ��������
	static CEncrypt_Charactermap& getObj(void);

    void EncryptString(char* pString);
    void DecryptString(char* pString);

private:
	unsigned char DecryptMap[256];
	static CEncrypt_Charactermap* m_pObj;

	CEncrypt_Charactermap();
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif
