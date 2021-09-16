#include "dcmanon.h"
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <cstdint>
#include <dcmtk/dcmdata/dcwcache.h>
#include "dcostrms.h"
#include <vector>
#include <cassert>
#include <curl/curl.h>
#include <boost/network/uri.hpp>
#include <dcmtk/dcmdata/dcostrmb.h>
#include <future>

struct CurlUpload
{
	void *buffer;
	size_t bsize;
};

size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	auto cu = reinterpret_cast<CurlUpload *>(userdata);

	memcpy(buffer, cu->buffer, cu->bsize);
	return cu->bsize;
}

DcmAnon::DcmAnon(const char *filepath) : filepath(filepath)
{
	ff.loadFile(filepath);
	ds = ff.getDataset();
}
const char *DcmAnon::getValue(patientInfo pInfo)
{
	const char *tmp = "";
	if (pInfo == PatientID)
		ds->findAndGetString(DCM_PatientID, tmp);
	else if (pInfo == PatientName)
		ds->findAndGetString(DCM_PatientName, tmp);
	else if (pInfo == PatientBirthDate)
		ds->findAndGetString(DCM_PatientBirthDate, tmp);
	else if (pInfo == PatientBirthTime)
		ds->findAndGetString(DCM_PatientBirthTime, tmp);
	else if (pInfo == PatientAge)
		ds->findAndGetString(DCM_PatientAge, tmp);
	else if (pInfo == PatientSex)
		ds->findAndGetString(DCM_PatientSex, tmp);
	else if (pInfo == PatientWeight)
		ds->findAndGetString(DCM_PatientWeight, tmp);
	else if (pInfo == PatientSize)
		ds->findAndGetString(DCM_PatientSize, tmp);
	else if (pInfo == PatientPosition)
		ds->findAndGetString(DCM_PatientPosition, tmp);
	if (!tmp)
		tmp = "";
	return tmp;
}
const char *DcmAnon::getValue(const DcmTagKey &key)
{
	const char *tmp = "";
	ds->findAndGetString(key, tmp);
	if (!tmp)
		tmp = ""; // checking OFCondition isn't safe!! that's not null-safe
	return tmp;
}
DcmAnon &DcmAnon::setValue(patientInfo pInfo, const char *substitute)
{
	if (pInfo == PatientID)
		ds->putAndInsertString(DCM_PatientID, substitute);
	else if (pInfo == PatientName)
		ds->putAndInsertString(DCM_PatientName, substitute);
	else if (pInfo == PatientBirthDate)
		ds->putAndInsertString(DCM_PatientBirthDate, substitute);
	else if (pInfo == PatientBirthTime)
		ds->putAndInsertString(DCM_PatientBirthTime, substitute);
	else if (pInfo == PatientAge)
		ds->putAndInsertString(DCM_PatientAge, substitute);
	else if (pInfo == PatientSex)
		ds->putAndInsertString(DCM_PatientSex, substitute);
	else if (pInfo == PatientWeight)
		ds->putAndInsertString(DCM_PatientWeight, substitute);
	else if (pInfo == PatientSize)
		ds->putAndInsertString(DCM_PatientSize, substitute);
	else if (pInfo == PatientPosition)
		ds->putAndInsertString(DCM_PatientPosition, substitute);
	return *this;
}
DcmAnon &DcmAnon::setValue(const DcmTagKey &tag, const char *substitute)
{
	ds->putAndInsertString(tag, substitute);
	return *this;
}

DcmAnon &DcmAnon::save(const char *filename)
{
	using boost::network::uri::uri;

	//process stdout
	if (strcmp(filename, "-") == 0)
	{
		DcmOutputStream *filestream = new DcmStdoutStream(filename);
		DcmWriteCache wcache;

		if (filestream->status().good())
		{
			ff.transferInit();
			ff.write(*filestream, EXS_Unknown, EET_UndefinedLength, &wcache, EGL_recalcGL, EPD_noChange, 0, 0, 0, EWM_createNewMeta);
			ff.transferEnd();
		}
		delete filestream;
	}
	else
	{
		uri url(filename);
		CurlUpload cu = {new char[123], 123};
		if (url.is_valid() && url.scheme() == "ftp")
		{
			auto future = std::async([url, cu]
									 {
										 curl_global_init(CURL_GLOBAL_ALL);
										 const auto upload_as = "ftp://" + url.host() + url.path();
										 const auto port = url.port() == "" ? 21 : stoi(url.port());
										 std::string result = "initialization failed";

										 CURL *curl = curl_easy_init();
										 if (curl)
										 {
											 curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
											 curl_easy_setopt(curl, CURLOPT_URL, upload_as.c_str());
											 curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
											 curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)cu.bsize);
											 curl_easy_setopt(curl, CURLOPT_USERPWD, url.user_info().c_str());
											 curl_easy_setopt(curl, CURLOPT_READDATA, cu);
											 result = curl_easy_strerror(curl_easy_perform(curl));
										 }
										 curl_global_cleanup();
										 return result;
									 });
			try
			{
				std::cout << "future get : " << future.get() << std::endl;
			}
			catch (...)
			{
				std::cerr << "exception occured" << std::endl;
			}
		}
		else
		{
			ff.saveFile(filename);
		}
	}
	return *this;
}
DcmAnon &DcmAnon::makeAnonFile(int pInfo, const char *substitute)
{
	if (pInfo | PatientID)
	{
		ds->putAndInsertString(DCM_PatientID, substitute);
	}
	if (pInfo | PatientName)
	{
		ds->putAndInsertString(DCM_PatientName, substitute);
	}
	if (pInfo | PatientBirthDate)
	{
		ds->putAndInsertString(DCM_PatientBirthDate, substitute);
	}
	if (pInfo | PatientBirthTime)
	{
		ds->putAndInsertString(DCM_PatientBirthTime, substitute);
	}
	if (pInfo | PatientAge)
	{
		ds->putAndInsertString(DCM_PatientAge, substitute);
	}
	if (pInfo | PatientSex)
	{
		ds->putAndInsertString(DCM_PatientSex, substitute);
	}
	if (pInfo | PatientWeight)
	{
		ds->putAndInsertString(DCM_PatientWeight, substitute);
	}
	if (pInfo | PatientSize)
	{
		ds->putAndInsertString(DCM_PatientSize, substitute);
	}
	if (pInfo | PatientPosition)
	{
		ds->putAndInsertString(DCM_PatientPosition, substitute);
	}
	return *this;
}

extern "C" EXPORT int getDcmSize()
{
	return sizeof(DcmAnon);
}

#ifdef _WIN32
extern "C" EXPORT void test(const char *input, const char *output)
{
	DcmAnon d(input);
	d.save(output);
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD ulreason, LPVOID)
{
	switch (ulreason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		break;
	default:
		break;
	}
	return TRUE;
}

#elif __linux__
static void please_do_not_optimize(const char *x)
{
	DcmAnon dcm(x);

	auto y = dcm.getValue((patientInfo)1);
	dcm.setValue((patientInfo)1, "").makeAnonFile(1).save(x);
}

int main()
{
	return 0;
}
#endif