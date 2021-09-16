#include <Windows.h>
#include <iostream>
int main(){
    auto hMod = LoadLibraryA("./dcmanon.dll");
    if (hMod){
        std::cout << hMod << std::endl;
        auto test = (void(*)(char*, char*))GetProcAddress(hMod, "test");

        if (test)
        {
            std::cout << test << std::endl;
            test("C:\\Users\\jychoi\\Downloads\\MR\\sample.dcm", "ftp://jychoi:jychoi@192.168.1.70/eefeswfswesfe.txt");
        }
    }
}