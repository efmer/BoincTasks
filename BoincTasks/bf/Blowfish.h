#pragma once

typedef struct {
  unsigned long P[16 + 2];
  unsigned long S[4][256];
} BLOWFISH_CTX;

class CBlowFish
{
public:
	void Test();
	void Encrypt(char *pcIn, char *pcOut, char *pcKey, int iBuffer);
	void Decrypt(char *pcIn, char *pcOut, char *pcKey, int iBuffer);

	void MakeHex(unsigned char *pcEncrypted, char *pcHex, int iLen);
	void ExtractHex(char *pcEncrypted, char *pcHex, int iLen);
private:
	void PackFromString(char **pcIn, unsigned long *xl, unsigned long *xr);
	void UnPackToString(char **pcOut, unsigned long *xl, unsigned long *xr, int iBuffer);
	void MakeString(char **pcOut, unsigned long *xl, unsigned long *xr, int iBuffer);
	void ExtractString(char **pcIn, unsigned long *xl, unsigned long *xr);



	static unsigned long F(BLOWFISH_CTX *ctx, unsigned long x);
	void Init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen);
	void Encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
	void Decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
};

/*

#include <string>
struct SBlock
	{
		SBlock(unsigned int l=0, unsigned int r=0) : m_uil(l), m_uir(r) {}

		SBlock(const SBlock& roBlock) : m_uil(roBlock.m_uil), m_uir(roBlock.m_uir) {}
		SBlock& operator^=(SBlock& b) { m_uil ^= b.m_uil; m_uir ^= b.m_uir; return *this; }
		unsigned int m_uil, m_uir;
	};


class CBlowFish
	{
		public:
			enum { ECB=0, CBC=1, CFB=2 };
			CBlowFish(unsigned char* ucKey, size_t n, const SBlock& roChain = SBlock(0UL,0UL));

			void ResetChain() { m_oChain = m_oChain0; }

			void Encrypt(unsigned char* buf, size_t n, int iMode=CFB);
			void Decrypt(unsigned char* buf, size_t n, int iMode=CFB);
			void Encrypt( unsigned char* in, unsigned char* out, size_t n, int iMode=CFB);
			void Decrypt( unsigned char* in, unsigned char* out, size_t n, int iMode=CFB);
			//static void Encrypt( const CString& strInput, CString& strOutput,const CString& strKey,int bitCount);
			//static void Decrypt( const CString& strInput, CString& strOutput,const CString& strKey,int bitCount);
			static void Encrypt( char* strInput, unsigned char *strOutput, char* strKey,int bitCount);
			static void Decrypt( char* strInput, unsigned char *strOutput, char* strKey,int bitCount);

		private:

			unsigned int F(unsigned int ui);
			void Encrypt(SBlock&);
			void Decrypt(SBlock&);

		private:

			SBlock m_oChain0;
			SBlock m_oChain;
			unsigned int m_auiP[18];
			unsigned int m_auiS[4][256];
			static const unsigned int scm_auiInitP[18];
			static const unsigned int scm_auiInitS[4][256];
	};

inline unsigned char Byte(unsigned int ui)
	{
		return (unsigned char)(ui & 0xff);
	}


inline unsigned int CBlowFish::F(unsigned int ui)
	{
		return ((m_auiS[0][Byte(ui>>24)] + m_auiS[1][Byte(ui>>16)]) ^ m_auiS[2][Byte(ui>>8)]) + m_auiS[3][Byte(ui)];
	}

void Char2Hex(const unsigned char ch, char* szHex);
void Hex2Char(const char* szHex, unsigned char& rch);
void CharStr2HexStr(const unsigned char* pucCharStr, std::string& strOut, int iSize);
void HexStr2CharStr(const  char* pszHexStr, unsigned char* pucCharStr, int iSize);
//void Encode( const CString& strInput,char* szOut ,int count);
//void Decode( const CString& strInput,CString& strOutput );
*/