#include <windows.h>
#include "Encryption.h"

void encrypt(std::string input, std::string output)
{
	std::string aes_key = AES_KEY, aes_iv = AES_IV;

	CBC_Mode< AES >::Encryption e;
	e.SetKeyWithIV((byte*)aes_key.c_str(), sizeof(aes_key), (byte*)aes_iv.c_str());

	// The StreamTransformationFilter removes
	// padding as required.
	FileSource(input.c_str(), true,
		new StreamTransformationFilter(e,
			new Base64Encoder(
					new FileSink(output.c_str())
			) // Base64Encoder
		) // StreamTransformationFilter
	); // FileSource
}

void decrypt(std::string input, std::string output)
{
	std::string aes_key = AES_KEY, aes_iv = AES_IV;

	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV((byte*)aes_key.c_str(), sizeof(aes_key), (byte*)aes_iv.c_str());

	// The StreamTransformationFilter removes
	// padding as required.
	FileSource(input.c_str(), true,
		new Base64Decoder(
			new StreamTransformationFilter(d,
				new FileSink(output.c_str())
			) // StreamTransformationFilter
		) // Base64Decoder
	); // FileSource
}