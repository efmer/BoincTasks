#pragma once

// http://www.adp-gmbh.ch/cpp/common/base64.html

class CBase64
{
public:
	CBase64(void);
	~CBase64(void);

std::string Encode(BYTE* bytes_to_encode, unsigned int in_len);
std::string Decode(std::string& encoded_string);

};

