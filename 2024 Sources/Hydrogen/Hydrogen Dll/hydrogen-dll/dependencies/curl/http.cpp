#include "http.hpp"

#include <curl/curl.h>

#include <sstream>
#define NDEBUG_LIB

#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

namespace http
{
	std::vector<std::string> last_headers;
	long http_code = 0;

	struct data_container {
		const char* buffer;
		std::size_t sizeleft;
	};

	static std::size_t header_callback(char* buffer, std::size_t size, std::size_t nitems, void* userdata)
	{
		auto write_size = nitems * size;

		last_headers.push_back(std::string(buffer, write_size));

		return write_size;
	}

	static std::size_t read_callback(void* ptr, std::size_t size, std::size_t nmemb, void* userp)
	{
		data_container* data = reinterpret_cast<data_container*>(userp);

		if (size * nmemb && data->sizeleft) {
			*reinterpret_cast<char*>(ptr) = data->buffer[0]; /* copy one single byte */
			data->buffer++;                 /* advance pointer */
			data->sizeleft--;                /* less data left */
			return 1;                        /* we return 1 byte at a time! */
		}

		return 0;                          /* no more data left to deliver */
	}

	std::string format_parameters(CURL* handle, const std::vector<parameter>& parameters)
	{
		std::stringstream buffer;
		std::size_t i = 0;

		buffer.unsetf(std::ios::skipws);

		for (const auto& parameter : parameters)
		{
			auto* key = curl_easy_escape(handle, parameter.key.c_str(), parameter.key.size());
			auto* value = curl_easy_escape(handle, parameter.value.c_str(), parameter.value.size());

			buffer << key << '=' << value;

			curl_free(key);
			curl_free(value);

			if (i++ < parameters.size() - 1)
				buffer << "&";
		}

		return buffer.str();
	}

	std::size_t write_function(void* buffer, std::size_t size, std::size_t members, void* str)
	{
		reinterpret_cast<std::string*>(str)->append(reinterpret_cast<const char*>(buffer), size * members);

		return size * members;
	}

	std::optional<std::string> get(const std::string& url, const std::vector<parameter>& parameters)
	{
		std::string result;

		auto* handle = curl_easy_init();

		curl_easy_setopt(handle, CURLOPT_URL, (url + "?" + format_parameters(handle, parameters)).c_str());
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);

		auto success = curl_easy_perform(handle);

		curl_easy_cleanup(handle);

		if (success == CURLE_OK)
			return result;
		else
			return std::nullopt;
	}

	std::optional<std::string> post(const std::string& url, const std::vector<parameter>& parameters)
	{
		std::string result;

		auto* handle = curl_easy_init();

		std::string params = format_parameters(handle, parameters);

		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(handle, CURLOPT_POSTFIELDS, params.c_str());
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);

		auto success = curl_easy_perform(handle);

		curl_easy_cleanup(handle);

		if (success == CURLE_OK)
			return result;
		else
			return std::nullopt;
	}

	std::optional<std::string> send_request(const std::string& url, std::string method, std::vector<std::string>& headers, const std::string& data)
	{
		std::string result;

		auto* handle = curl_easy_init();

		last_headers.clear();

		if (headers.size())
		{
			struct curl_slist* chunk = NULL;

			for (const std::string& header : headers)
				chunk = curl_slist_append(chunk, header.c_str());
			//chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
			/* set our custom set of headers */
			curl_easy_setopt(handle, CURLOPT_HTTPHEADER, chunk);
		}

		curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, method.c_str());
		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10);

		if (data.length())
		{
			data_container body;

			body.buffer = &data[0];
			body.sizeleft = data.length();
			curl_easy_setopt(handle, CURLOPT_POST, 1L);
			/* we want to use our own read function */
			//curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, data.length());
			curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);

			/* pointer to pass to our read function */
			curl_easy_setopt(handle, CURLOPT_READDATA, &body);
			curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (curl_off_t)body.sizeleft);

		}

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);

		curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);

		auto success = curl_easy_perform(handle);

		curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);

		curl_easy_cleanup(handle);

		if (success == CURLE_OK)
			return result;
		else
			return std::nullopt;
	}

	std::map<int, std::string> status_message =
	{
		{100, "Continue"},
		{101, "Switching Protocols"},
		{102, "Processing"},
		{103, "Early Hints"},

		{200, "OK"},
		{201, "Created"},
		{202, "Accepted"},
		{203, "Non-Authoritative Information"},
		{204, "No Content"},
		{205, "Reset Content"},
		{206, "Partial Content"},
		{207, "Multi-Status"},
		{208, "Already Reported"},
		{209, "IM Used"},

		{300, "Multiple Choice"},
		{301, "Moved Permanently"},
		{302, "Found"},
		{303, "See Other"},
		{304, "Not Modified"},
		{305, "Use Proxy"},
		{307, "Temporary Redirect"},
		{308, "Permanent Redirect"},

		{400, "Bad Request"},
		{401, "Unauthorized"},
		{402, "Payment Required"},
		{403, "Forbidden"},
		{404, "Not Found"},
		{405, "Method Not Allowed"},
		{406, "Not Acceptable"},
		{407, "Proxy Authentication Required"},
		{408, "Request Timeout"},
		{409, "Conflict"},
		{410, "Gone"},
		{411, "Length Required"},
		{412, "Precondition Failed"},
		{413, "Payload Too Large"},
		{414, "URI Too Long"},
		{415, "Unsupported Media Type"},
		{416, "Range Not Satisfiable"},
		{417, "Expectation Failed"},
		{418, "I'm a teapot"},

		{421, "Misdirected Request"},
		{422, "Unprocessable Entity"},
		{423, "Locked"},
		{424, "Failed Dependency"},
		{425, "Too Early"},
		{426, "Upgrade Required"},
		{428, "Precondition Required"},
		{429, "Too Many Requests"},
		{431, "Request Header Fields Too Large"},
		{451, "Unavailable For Legal Reasons"},

		{500, "Internal Server Error"},
		{501, "Not Implemented"},
		{502, "Bad Gateway"},
		{503, "Service Unavailable"},
		{504, "Gateway Timeout"},
		{505, "HTTP Version Not Supported"},
		{506, "Variant Also Negotiates"},
		{507, "Insufficient Storage"},
		{508, "Loop Detected"},
		{510, "Not Extended"},
		{511, "Network Authentication Required"}
	};
}
