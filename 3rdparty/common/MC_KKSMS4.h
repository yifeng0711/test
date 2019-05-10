//---------------------------------------------------------------------------
#ifndef MC_KKSMS4H
#define MC_KKSMS4H
//---------------------------------------------------------------------------
#include <Windows.h>
#define ROUND            32
#ifndef TYPE_KKSMS4_VAL
#define TYPE_KKSMS4_VAL
typedef unsigned char KKSMS4_VAL[16];
#endif
struct KKBuffer
{
	int		_len;
	char*	_dat;
};
//---------------------------------------------------------------------------
class MC_KKSMS4
{
private:
	unsigned long	T1(unsigned long    dwA);
	unsigned long	T2(unsigned long    dwA);

	void	convert_to_network_order(unsigned long* src,unsigned long* dst,int count);
	void	convert_to_host_order(unsigned long* src,unsigned long* dst,int count);

	void Key_Expansion		(unsigned long MK[],	unsigned long rk[]);
	void ECB_Encryption_Core(unsigned long X[],		unsigned long rk[], unsigned long Y[]);
	void ECB_Decryption_Core(unsigned long X[],		unsigned long rk[], unsigned long Y[]);
	void ECB_Encryption		(unsigned char plaintext[16],	unsigned char key[16], unsigned char ciphertext[16]);
	void ECB_Decryption		(unsigned char ciphertext[16],	unsigned char key[16], unsigned char plaintext[16]);

public:
	bool EncryptBuffer	(IN OUT KKBuffer* vBuf, IN char* vKey);
	bool DecryptBuffer	(IN OUT KKBuffer* vBuf, IN char* vKey);
//    bool EncryptBuffer	(KKBuffer* vBuf, char* vKey);
//    bool DecryptBuffer	(KKBuffer* vBuf, char* vKey);
};
//---------------------------------------------------------------------------
extern MC_KKSMS4 kinkoo_SMS4;
//---------------------------------------------------------------------------
#endif
