//
// Created by DJ Cook on 8/2/2023.
//

#ifndef ENCRYPTION_ENCRYPTION_H
#define ENCRYPTION_ENCRYPTION_H

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

enum EncryptionType
{
    ADDITIVE,
    XOR,
    CAESAR
};

class Encryption
{
private:
    char c;

    // Additive encryption.
    void encryptAdditive();
    void decryptAdditive();

    // XOR encryption.
    void encryptXOR() const;
    void decryptXOR();

    // Caesar cypher encryption.
    void encryptCaesar() const;
    void decryptCaesar() const;

public:
    EncryptionType type;
    string filepath;
    int key; // The encryption key.

    // Constructors.
    explicit Encryption(EncryptionType type, string filepath);
    Encryption(EncryptionType type, string filepath, int key);

    // Methods.
    void encrypt();
    void decrypt();
};


#endif //ENCRYPTION_ENCRYPTION_H
