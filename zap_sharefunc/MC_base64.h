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
	static	std::string			File2Base64		(std::string vFilePath);	//将图片转为base64
	static	const std::string	encode64		(const unsigned char* vData, unsigned int vLen);


	/**
	 * @Method		:	base64_encode
	 * @brief		:	base64编码
	 * @param[in]	:	bindata		需要编码的明文字符串
	 * @param[out]	:	binlength	明文字符串的长度(不包括'\0')
	 * @param[out]	:	base64		编码后的base64字符串
	 * @return		:	
	 */
	static	char*				base64_encode	(const unsigned char* bindata, int binlength, char* base64);

	/**
	 * @Method		:	base64_decode
	 * @brief		:	base64解码
	 * @param[in]	:	base64		需要解码的base64串
	 * @param[out]	:	bindata		解码出来的明文
	 * @return		:	明文长度
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
ui->label->setPixmap(tImageResult);	//可用Qlabel显示
*/