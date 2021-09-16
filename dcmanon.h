#pragma once
#include <dcmtk/dcmdata/dctk.h>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "IPHLPAPI.lib")
    #pragma comment(lib, "netapi32.lib")
    #define EXPORT __declspec(dllexport)
#elif __linux__
    #define EXPORT __attribute__ ((visibility("default"))) 
#endif

enum patientInfo {
	PatientID = 1 << 0,
	PatientName = 1 << 1,
	PatientBirthDate = 1 << 2,
	PatientBirthTime = 1 << 3,
	PatientAge = 1 << 4,
	PatientSex = 1 << 5,
	PatientWeight = 1 << 6,
	PatientSize = 1 << 7,
	PatientPosition = 1 << 8,
};


extern "C"
class EXPORT DcmAnon
{
private:
    std::string filepath;
    DcmFileFormat ff;
    DcmDataset *ds;
public:
    DcmAnon(const DcmAnon&) = delete;
    DcmAnon(const DcmAnon&&) = delete;
    DcmAnon& operator=(const DcmAnon&) = delete;
    DcmAnon& operator=(const DcmAnon&&) = delete;

    DcmAnon(const char* filepath);
    const char* getValue(patientInfo pInfo);
    const char* getValue(const DcmTagKey& key);

	DcmAnon& setValue(patientInfo pInfo, const char* substitute);

	DcmAnon& setValue(const DcmTagKey& tag, const char* substitute);

	DcmAnon& save(const char* filename);

	DcmAnon& makeAnonFile(int pInfo, const char* substitute = "");
};
