#pragma once

#include <cryptlib.h>
using CryptoPP::Exception;

#include <files.h>
using CryptoPP::FileSink;
using CryptoPP::FileStore;
using CryptoPP::FileSource;
using CryptoPP::StreamTransformationFilter;

#include <aes.h>
using CryptoPP::AES;

#include <ccm.h>
using CryptoPP::CBC_Mode;

#include <base64.h>
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;

#define AES_KEY "#^TMA!kNZK$qrp%wA&+6nCqX9=-q9+tFQPMxxhW9+Sfash266s"
#define AES_IV "W%+m3D@stf%?S8te"

void encrypt(std::string input, std::string output);
void decrypt(std::string input, std::string output);