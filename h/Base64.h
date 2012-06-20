// Base64.h: interface for the CBase64 class.
// Author: Wes Clyburn (clyburnw@enmu.edu)
//
// El algoritmo de codificación en base64 se ha cambiado por motivos
// de rendimiento. El nuevo algoritmo está basado en el que utiliza
// Davide Libenzi <davidel@xmailserver.org> en su programa SndMail
// (http://www.xmailserver.org/)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64_H__FD6A25D1_EE0E_11D1_870E_444553540001__INCLUDED_)
#define AFX_BASE64_H__FD6A25D1_EE0E_11D1_870E_444553540001__INCLUDED_

#include <windows.h>

#include <pshpack8.h>
	#include <rope>
#include <poppack.h>


typedef	std::rope<char>	BufferSalida;


// CBase64
// An encoding agent that handles Base64
class CBase64
{
public:
	CBase64();
	virtual ~CBase64();

	BufferSalida Encode( LPCTSTR szEncoding, unsigned int nSize );

	int EncodeBuffer(const char *_in, unsigned inlen,
                     char *_out, unsigned outmax, unsigned *outlen);
	int DecodeBuffer(const char *in, unsigned inlen, char *out, unsigned *outlen);

protected:
	static char basis_64[];
	static char index_64[128];

private:
};


#endif
