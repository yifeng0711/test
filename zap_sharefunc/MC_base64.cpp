#include "MC_base64.h"
//---------------------------------------------------------------------------
static const std::string g_Alphabet64( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" );
static const char g_Pad	=	'=';

const char* base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//---------------------------------------------------------------------------
std::string MC_base64::File2Base64(std::string vFilePath)
{
	FILE* tFp = NULL;
	if(0 != fopen_s(&tFp,vFilePath.c_str(), "rb"))
	{
		return "";
	}

	fseek(tFp, 0L, SEEK_END);
	long tLen = ftell(tFp);
	fseek(tFp, 0L, SEEK_SET);

	unsigned char* tData = (unsigned char*)malloc(tLen * sizeof(unsigned char));
	if(NULL == tData)
	{
		fclose(tFp);
		return "";
	}

	if(tLen != fread(tData,sizeof(unsigned char), tLen, tFp))
	{
		fclose(tFp);
		free(tData);
		return "";
	}

	fclose(tFp);

	//std::string tEncode = encode64(tData, tLen);
	char * tBase64 = (char *)malloc(tLen * sizeof(char) * 2);
	if(NULL == tBase64)
	{
		free(tData);
		return "";
	}
	base64_encode(tData, tLen, tBase64);
	std::string tEncode = tBase64;
	free(tBase64);


	free(tData);
	return tEncode;
}
//---------------------------------------------------------------------------
const std::string MC_base64::encode64(const unsigned char* vData,unsigned int vLen)
{
	std::string tEncoded;
	char		tChar;

	if(NULL == vData)
	{
		return tEncoded;
	}

	tEncoded.reserve(vLen * 2);

	for(unsigned int i=0; i<vLen; ++i)
	{
		tChar = static_cast<char>( ( vData[i] >> 2 ) & 0x3f );
		tEncoded += g_Alphabet64[tChar];

		tChar = static_cast<char>( ( vData[i] << 4 ) & 0x3f );
		if( ++i < vLen )
			tChar = static_cast<char>( tChar | static_cast<char>( ( vData[i] >> 4 ) & 0x0f ) );
		tEncoded += g_Alphabet64[tChar];

		if( i < vLen )
		{
			tChar = static_cast<char>( ( vData[i] << 2 ) & 0x3c );
			if( ++i < vLen )
				tChar = static_cast<char>( tChar | static_cast<char>( ( vData[i] >> 6 ) & 0x03 ) );
			tEncoded += g_Alphabet64[tChar];
		}
		else
		{
			++i;
			tEncoded += g_Pad;
		}

		if( i < vLen )
		{
			tChar = static_cast<char>( vData[i] & 0x3f );
			tEncoded += g_Alphabet64[tChar];
		}
		else
		{
			tEncoded += g_Pad;
		}
	}

	return tEncoded;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
char* MC_base64::base64_encode(const unsigned char* bindata, int binlength, char* base64)
{
	int i, j;
	unsigned char current;

	for ( i = 0, j = 0 ; i < binlength ; i += 3 )
	{
		current = (bindata[i] >> 2) ;
		current &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
		if ( i + 1 >= binlength )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}
		current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
		if ( i + 2 >= binlength )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			break;
		}
		current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
		base64[j++] = base64char[(int)current];
	}
	base64[j] = '\0';
	return base64;
}
//---------------------------------------------------------------------------
int MC_base64::base64_decode(const char* base64, unsigned char* bindata)
{
	int i, j;
	unsigned char k;
	unsigned char temp[4];
	for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
	{
		memset( temp, 0xFF, sizeof(temp) );
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i] )
				temp[0]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+1] )
				temp[1]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+2] )
				temp[2]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+3] )
				temp[3]= k;
		}

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
			((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
		if ( base64[i+2] == '=' )
			break;

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
			((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
		if ( base64[i+3] == '=' )
			break;

		bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
			((unsigned char)(temp[3]&0x3F));
	}
	return j;
}
//---------------------------------------------------------------------------
