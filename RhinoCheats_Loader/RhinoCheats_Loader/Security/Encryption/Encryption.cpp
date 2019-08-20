#include <Windows.h>
#include "Encryption.h"

std::string encrypt(std::string decrypted)
{
	if (decrypted.empty())
		return decrypted;

	std::string aes_key = AES_KEY, aes_iv = AES_IV, encrypted;

	CBC_Mode< AES >::Encryption e;
	e.SetKeyWithIV((byte*)aes_key.c_str(), sizeof(aes_key), (byte*)aes_iv.c_str());

	// The StreamTransformationFilter removes
	// padding as required.
	StringSource(decrypted, true,
		new StreamTransformationFilter(e,
			new Base64Encoder(
				new StringSink(encrypted)
			) // Base64Encoder
		) // StreamTransformationFilter
	); // StringSource

	return encrypted;
}

std::string decrypt(std::string encrypted)
{
	if (encrypted.empty())
		return encrypted;

	std::string aes_key = AES_KEY, aes_iv = AES_IV, decrypted;

	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV((byte*)aes_key.c_str(), sizeof(aes_key), (byte*)aes_iv.c_str());

	// The StreamTransformationFilter removes
	// padding as required.
	StringSource(encrypted, true,
		new Base64Decoder(
			new StreamTransformationFilter(d,
				new StringSink(decrypted)
			) // StreamTransformationFilter
		) // Base64Decoder
	); // StringSource

	return decrypted;
}