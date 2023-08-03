#include <iostream>
#include "Encryption.h"

using namespace std;

int main()
{
    Encryption encryption = Encryption(EncryptionType::CAESAR,
                                       R"(C:\Users\Danie\Desktop\test.txt)", 50);
    encryption.decrypt();

    return 0;
}
