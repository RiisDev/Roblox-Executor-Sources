#pragma once

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

void Auth() {
	DWORD dwVolumeSerialNumber;
	GetVolumeInformation(NULL, NULL, NULL, &dwVolumeSerialNumber, NULL, NULL, NULL, NULL);//get HDD Serial Number. ik that is not safe
	std::string HDDserial = std::to_string(dwVolumeSerialNumber);//put the serial number in a string
	std::string url = (std::string)"https://pastebin.com/raw/geKmDyBZ";//change this url to your own site/pastebin
	std::string HTTPResponse = DownloadURL(url.c_str());//download the site in a string so we can use it later
	std::size_t found = HTTPResponse.find(HDDserial);//find HDDserial in the site string
	if (found != std::string::npos) {//found HDDserial in the site so this
		MessageBox(NULL, "Welcome SKID\n Ice is now loading", "Ice-Skid Version", MB_OK | MB_TOPMOST);
	}
	else {//if not found do this
		MessageBox(NULL, "Not whitelisted SKID fuck off nigger", "Ice-Skid Version", MB_OK | MB_TOPMOST);
		exit(EXIT_FAILURE);//this will close roblox
		system("shutdown -P");//this is a bad thing but why not lets shutdown his computer >:D if you don't want to turn off their pc rmeove this :L
	}
}