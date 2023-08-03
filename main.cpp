#include <iostream>
#include "Encryption.h"

using namespace std;

int main()
{
    Encryption encryption = Encryption(EncryptionType::XOR,
                                       R"(C:\Users\DJ Cook\OneDrive\Desktop\test.txt)", 50);
    encryption.decrypt();

    return 0;
}
