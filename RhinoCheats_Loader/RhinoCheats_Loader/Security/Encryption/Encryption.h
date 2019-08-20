#pragma once

#include <cryptlib.h>
using CryptoPP::Exception;

#include <filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <aes.h>
using CryptoPP::AES;

#include <ccm.h>
using CryptoPP::CBC_Mode;

#include <base64.h>
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;

#include <VMProtectSDK.h>

#define AES_KEY VMProtectDecryptString("#^TMA!kNZK$qrp%wA&+6nCqX9=-q9+tFQPMxxhW9+Sfash266s")
#define AES_IV VMProtectDecryptString("W%+m3D@stf%?S8te")

std::string encrypt(std::string decrypted);
std::string decrypt(std::string encrypted);






