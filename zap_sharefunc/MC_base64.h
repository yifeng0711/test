#ifndef MC_base64_H
#define MC_base64_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_base64
{
public:
	static	std::string			File2Base64		(std::string vFilePath);	//��ͼƬתΪbase64
	static	const std::string	encode64		(const unsigned char* vData, unsigned int vLen);


	/**
	 * @Method		:	base64_encode
	 * @brief		:	base64����
	 * @param[in]	:	bindata		��Ҫ����������ַ���
	 * @param[out]	:	binlength	�����ַ����ĳ���(������'\0')
	 * @param[out]	:	base64		������base64�ַ���
	 * @return		:	
	 */
	static	char*				base64_encode	(const unsigned char* bindata, int binlength, char* base64);

	/**
	 * @Method		:	base64_decode
	 * @brief		:	base64����
	 * @param[in]	:	base64		��Ҫ�����base64��
	 * @param[out]	:	bindata		�������������
	 * @return		:	���ĳ���
	 */
	static	int					base64_decode	(const char* base64, unsigned char* bindata);
};
//---------------------------------------------------------------------------
#endif
/*
unsigned char tStr[] = "123456789";
char tBuffer[1024] = {0};
cout << MC_base64::base64_encode(tStr,sizeof(tStr) - 1,  tBuffer) << endl;

unsigned char tBuffer1[1024] = {0};
cout << MC_base64::base64_decode(tBuffer, tBuffer1) << endl;
cout << tBuffer1 << endl;
*/

/*
std::string tFile = File2Base64("E:\\MyProject\\MyProgram\\MyQt\\1.jpg");
QByteArray tByteArray(tFile.c_str(), tFile.length());
QPixmap tImageResult;
tImageResult.loadFromData(QByteArray::fromBase64(tByteArray));
ui->label->setPixmap(tImageResult);	//����Qlabel��ʾ
*/