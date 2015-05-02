// cryptopp::AES test.
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;

void generate_key_and_iv(std::string key_filename, std::string iv_filename)
{
    byte key[AES::DEFAULT_KEYLENGTH];
    byte iv[AES::DEFAULT_KEYLENGTH];

    AutoSeededRandomPool rng;
    rng.GenerateBlock(key, AES::DEFAULT_KEYLENGTH);
    rng.GenerateBlock(iv, AES::DEFAULT_KEYLENGTH);

    // Write key and iv into file.
    std::ofstream key_file(key_filename, std::ios::binary|std::ios::out);
    key_file.write((char*)&key, sizeof(key));
    key_file.close();

    std::ofstream iv_file(iv_filename, std::ios::binary|std::ios::out);
    iv_file.write((char*)&iv, sizeof(iv));
    iv_file.close();
}

void load_key_and_iv(std::string key_filename, std::string iv_filename,
                    byte key[], byte iv[])
{
    std::ifstream key_file(key_filename, std::ios::binary);
    key_file.seekg(0, std::ios::end);
    size_t key_arr_len = key_file.tellg();
    char *key_char_arr = new char[key_arr_len];
    key_file.seekg(0, std::ios::beg);
    key_file.read(key_char_arr, key_arr_len);
    key_file.close();

    for (int i = 0; i < key_arr_len; i ++)
    {
        key[i] = (byte)key_char_arr[i];
    }

    std::ifstream iv_file(iv_filename, std::ios::binary);
    iv_file.seekg(0, std::ios::end);
    size_t iv_arr_len = iv_file.tellg();
    char *iv_char_arr = new char[iv_arr_len];
    iv_file.seekg(0, std::ios::beg);
    iv_file.read(iv_char_arr, iv_arr_len);
    iv_file.close();

    for (int i = 0; i < iv_arr_len; i ++)
    {
        iv[i] = (byte)iv_char_arr[i];
    }
}

std::string encrypt(std::string plaintext, byte key[], byte iv[])
{
    std::string cipher;

    AES::Encryption aes_encryption (key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbc_cipher_encryption(aes_encryption, iv);

    StreamTransformationFilter stf_encryptor(
        cbc_cipher_encryption,
        new StringSink(cipher)
    );

    stf_encryptor.Put(
        reinterpret_cast<const unsigned char*>(plaintext.c_str()),
        plaintext.length()
        );

    stf_encryptor.MessageEnd();

    return cipher;
}

std::string decrypt(std::string cipher, byte key[], byte iv[])
{
    std::string plaintext;

    AES::Decryption aes_decryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbc_cipher_decryption(aes_decryption, iv);

    StreamTransformationFilter stf_decryptor(
        cbc_cipher_decryption,
        new CryptoPP::StringSink(plaintext)
    );

    stf_decryptor.Put(
        reinterpret_cast<const unsigned char*>(cipher.c_str()),
        cipher.length()
        );

    stf_decryptor.MessageEnd();

    return plaintext;
}


int main()
{
    std::string key_filename = "./aes_key.txt";
    std::string iv_filename = "./aes_iv.txt";

    std::cout << "Generate key and iv." << std::endl;
    generate_key_and_iv(key_filename, iv_filename);

    // Load key and iv.
    byte key[AES::DEFAULT_KEYLENGTH];
    byte iv[AES::DEFAULT_KEYLENGTH];
    load_key_and_iv(
        key_filename, iv_filename,
        key, iv
    );

    std::stringstream key_ss;
    std::string key_s;
    for (int i = 0; i < sizeof(key); i ++)
    {
        key_ss << (int)(key[i]);
        if (i != sizeof(key) - 1)
        {
            key_ss << ",";
        }
    }
    key_s = key_ss.str();
    std::cout << "The key str = " << key_s << std::endl;

    std::stringstream iv_ss;
    std::string iv_s;
    for (int i = 0; i < sizeof(iv); i ++)
    {
        iv_ss << (int)(iv[i]);
        if (i != sizeof(iv) - 1)
        {
            iv_ss << ",";
        }
    }
    iv_s = iv_ss.str();
    std::cout << "The iv str = " << iv_s << std::endl;

    // Encrypt.
    std::string plaintext = "This is an AES test.";
    std::string cipher = encrypt(plaintext, key, iv);
    std::cout << "plaintext = " << plaintext << ", length = " << plaintext.size() << std::endl;
    std::cout << "cipher = " << cipher << ", length = " << cipher.size() << std::endl;

    // Encrypt.
    std::string cipher_base64;
    CryptoPP::StringSource str_source (
        cipher,
        true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(cipher_base64),
            false
        )
    );

    std::cout << "The cipher base64 = " << cipher_base64 << std::endl;

    // Decrypt.
    std::string raw_cipher;
    CryptoPP::StringSource(
        cipher_base64,
        true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(raw_cipher)
        )
    );

    std::cout << "The raw cipher = " << raw_cipher << std::endl;
    cipher = raw_cipher;


    // Decrypt.
    std::string decrypted_plaintext = decrypt(cipher, key, iv);
    std::cout << "decrypted_plaintext = " << decrypted_plaintext << ", length = " << decrypted_plaintext.size() << std::endl;
}