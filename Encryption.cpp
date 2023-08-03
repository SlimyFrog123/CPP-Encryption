//
// Created by DJ Cook on 8/2/2023.
//

#include <sstream>
#include "Encryption.h"


Encryption::Encryption(EncryptionType type, string filepath)
{
    this->type = type;
    this->filepath = std::move(filepath);
    this->c = '\0';

    // Generate a random number for the key, since the user did not specify which key to use.
    // Seed the random number engine with a random value from the system.
    random_device randomDevice;
    mt19937 rng(randomDevice());

    // Number range to generate from.
    int minimum = 1;
    int maximum = type == EncryptionType::CAESAR ? 25 : 9999;

    // Create a uniform_int_distribution to generate random integers within the specified range.
    uniform_int_distribution<int> dist(minimum, maximum);

    // Generate the random number and set the key to its value.
    this->key = dist(rng);
}

Encryption::Encryption(EncryptionType type, string filepath, int key)
{
    this->type = type;
    this->filepath = std::move(filepath);
    this->key = key;
    this->c = '\0';

    // Ensure that the key is valid for the given encryption method.
    if (type == EncryptionType::CAESAR)
    {
        if (!(key >= 1 && key <= 25))
        {
            cerr << "Your key must be between 1 and 25 for the Caesar cipher encryption.";
            exit(-1);
        }
    }
}

void Encryption::encrypt()
{
    switch (type)
    {
        case EncryptionType::ADDITIVE:
            encryptAdditive();

            break;
        case EncryptionType::XOR:
            encryptXOR();

            break;
        case EncryptionType::CAESAR:
            encryptCaesar();

            break;
    }
}

void Encryption::decrypt()
{
    switch (type)
    {
        case EncryptionType::ADDITIVE:
            decryptAdditive();

            break;
        case EncryptionType::XOR:
            decryptXOR();

            break;
        case EncryptionType::CAESAR:
            decryptCaesar();

            break;
    }
}

void Encryption::encryptAdditive()
{
    fstream inStream, outStream;
    vector<int> encryptedContents;

    // Open the file in read mode (fstream::in).
    inStream.open(filepath, fstream::in);

    // Read the file and add every character to a list once it is encrypted.
    while (inStream >> noskipws >> c)
    {
        // Get the new (encrypted) value of the current character by adding the key to the ASCII value of the character.
        int value = c + key;

        // Add the integer value of the character to the list of encrypted characters.
        encryptedContents.push_back(value);
    }

    // Close the file.
    inStream.close();

    // Open the file in write mode (fstream::out).
    outStream.open(filepath, fstream::out);

    // Add each encrypted character to the file.
    for (int character : encryptedContents)
    {
        // Convert the character's ASCII value into a character by casting it to a char.
        char encryptedCharacter = (char)character;

        // Write the character to the file.
        outStream << encryptedCharacter;
    }

    // Close the file.
    outStream.close();
}

void Encryption::decryptAdditive()
{
    fstream inStream, outStream;
    vector<int> decryptedContents;

    // Open the file in read mode (fstream::in).
    inStream.open(filepath, fstream::in);

    // Read the file and add every character to a list once it is decrypted.
    while (inStream >> noskipws >> c)
    {
        // Get the new (decrypted) value of the current character by subtracting the key from the ASCII value of the character.
        int value = c - key;

        // Add the integer value of the character to the list of encrypted characters.
        decryptedContents.push_back(value);
    }

    // Close the file.
    inStream.close();

    // Open the file in write mode (fstream::out).
    outStream.open(filepath, fstream::out);

    // Add each decrypted character to the file.
    for (int character : decryptedContents)
    {
        // Convert the character's ASCII value into a character by casting it to a char.
        char decryptedCharacter = (char)character;

        // Write the character to the file.
        outStream << decryptedCharacter;
    }

    // Close the file.
    outStream.close();
}

void Encryption::encryptXOR() const
{
    ifstream inputFile(filepath, ios::binary);

    if (!inputFile)
    {
        cerr << "Error opening file." << endl;
        return;
    }

    stringstream stringStream;
    stringStream << inputFile.rdbuf();
    inputFile.close();

    string data = stringStream.str();
    string keyString = to_string(key);
    int keyLength = keyString.length();

    for (size_t i = 0; i < data.length(); ++i)
    {
        data[i] ^= keyString[i % keyLength];
    }

    ofstream outputFile(filepath, ios::binary | ios::trunc);
    if (!outputFile)
    {
        cerr << "Error opening file." << endl;
        return;
    }

    // Write the data back to the file.
    outputFile.write(data.c_str(), (streamsize)data.length());
    outputFile.close();
}

void Encryption::decryptXOR()
{
    encryptXOR();
}

void Encryption::encryptCaesar() const
{
    // Read the file.
    string contents;

    // Open the file in read mode (ios::in).
    ifstream file(filepath, ios::in);

    // Ensure that the file is open.
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return;
    }

    // Actually read the file contents.
    ostringstream stringStream;
    stringStream << file.rdbuf();

    // Close the file.
    file.close();

    // Set the contents variable to the file contents.
    contents = stringStream.str();

    // Encrypt the file contents using the Caesar cipher.
    string encryptedContents;

    // Loop through the characters and apply the Caesar cipher to each if applicable.
    for (char character : contents)
    {
        int upperShift = 65;
        int lowerShift = 97;

        if (isalpha(character))
        {
            // Apply the Caesar cipher to the specific character.
            int shift = isupper(character) ? upperShift : lowerShift;
            encryptedContents += char(int(character + key - shift) % 26 + shift);
        }
        else
        {
            // The character is not in the alphabet, so just add it to the string as it is.
            encryptedContents += character;
        }
    }

    // Finally, write the newly encrypted contents back to the file, and close the file.
    ofstream outStream(filepath, ios::trunc);
    outStream << encryptedContents;
    outStream.close();
}

void Encryption::decryptCaesar() const
{
    // Read the file.
    string contents;

    // Open the file in read mode (ios::in).
    ifstream file(filepath, ios::in);

    // Ensure that the file is open.
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return;
    }

    // Actually read the file contents.
    ostringstream stringStream;
    stringStream << file.rdbuf();

    // Close the file.
    file.close();

    // Set the contents variable to the file contents.
    contents = stringStream.str();

    // Encrypt the file contents using the Caesar cipher.
    string encryptedContents;

    // Loop through the characters and apply the Caesar cipher to each if applicable.
    for (char character : contents)
    {
        int upperShift = 65;
        int lowerShift = 97;

        if (isalpha(character))
        {
            // Apply the Caesar cipher to the specific character.
            int shift = isupper(character) ? upperShift : lowerShift;
            encryptedContents += char(int(character + (26 - key) - shift) % 26 - shift);
        }
        else
        {
            // The character is not in the alphabet, so just add it to the string as it is.
            encryptedContents += character;
        }
    }

    // Finally, write the newly encrypted contents back to the file, and close the file.
    ofstream outStream(filepath, ios::trunc);
    outStream << encryptedContents;
    outStream.close();
}
