// This file for test HMAC.
#include <iostream>
#include <string>
#include <fstream>

#include <cryptopp/osrng.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>

using namespace CryptoPP;

void generate_key(std::string key_filename)
{
    AutoSeededRandomPool rng;
    int key_length = 16;
    byte key[key_length];
    rng.GenerateBlock(key, key_length);

    // Write key into file.
    std::ofstream key_file(key_filename, std::ios::binary|std::ios::out);
    key_file.write((char*)&key, sizeof(key));
    key_file.close();
}

void load_key(std::string key_filename, byte key[])
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
}

std::string generate_HMAC(std::string message, byte key[])
{
    std::string mac;
    HMAC<SHA256> hmac(key, sizeof(key));
    StringSource ss(
        message,
        true,
        new HashFilter(
            hmac,
            new StringSink(mac))
    );

    return mac;
}

bool verify_HMAC(std::string message, byte* key, std::string mac)
{
    try
    {
        HMAC<SHA256> hmac(key, sizeof(key));
        const int flags = HashVerificationFilter::THROW_EXCEPTION |
                          HashVerificationFilter::HASH_AT_END;
        StringSource(
            message + mac,
            true,
            new HashVerificationFilter(hmac, NULL, flags)
        );

        return true;
    }
    catch(const Exception& e)
    {
        return false;
    }
}

int main()
{
    std::string key_filename = "./hmac_key.txt";
    std::cout << "Generate key." << std::endl;
    generate_key(key_filename);

    std::cout << "Load key from file." << std::endl;
    int key_len = 16;
    byte key[key_len];
    load_key(key_filename, key);

    // Generate HMAC.
    std::string message_1 = "hello world.";
    std::string mac_1 = generate_HMAC(message_1, key);
    std::cout << "message_1 = " << message_1 << std::endl;
    std::cout << "mac_1 = " << mac_1 << std::endl;

    // Verify HMAC.
    bool is_valid_1 = verify_HMAC(message_1, key, mac_1);
    if (is_valid_1) std::cout << "verified." << std::endl;
    else std::cout << "not verified." << std::endl;


    // Generate HMAC.
    std::string message_2 = "hello cat.";
    std::string mac_2 = generate_HMAC(message_2, key);
    std::cout << "message_2 = " << message_2 << std::endl;
    std::cout << "mac_2 = " << mac_2 << std::endl;

    // Verify HMAC.
    bool is_valid_2 = verify_HMAC(message_2, key, mac_1);
    if (is_valid_2) std::cout << "verified." << std::endl;
    else std::cout << "not verified." << std::endl;

}