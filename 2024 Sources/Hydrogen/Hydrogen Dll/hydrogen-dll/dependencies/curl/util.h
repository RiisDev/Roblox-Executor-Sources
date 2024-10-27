#include <dependencies/curl/curl.h>

#include <string>

int init = curl_global_init( CURL_GLOBAL_ALL );

unsigned int writeToString( void* ptr, unsigned int size, unsigned int count, void* stream ) {
    ( ( std::string* ) stream )->append( ( char* ) ptr, 0, size * count );
    return size * count;
}

inline std::string post( const std::string& url, const std::string& content ) {
    CURL* curl;
    CURLcode res;
    std::string data;

    curl_global_init( CURL_GLOBAL_ALL );

    curl = curl_easy_init( );

    if ( curl ) {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_POSTFIELDS, content.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, writeToString );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &data );
        curl_easy_setopt( curl, CURLOPT_TIMEOUT_MS, 10000 );

        res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
    }

    return data;
}

inline std::string get( const std::string& url )
{
    CURL* curl;
    CURLcode res;
    std::string data;
    std::string header_string;

    curl_global_init( CURL_GLOBAL_ALL );

    curl = curl_easy_init( );

    if ( curl ) {
        curl_easy_setopt( curl, CURLOPT_URL, url.c_str( ) );
        curl_easy_setopt( curl, CURLOPT_NOPROGRESS, 1L );
        curl_easy_setopt( curl, CURLOPT_MAXREDIRS, 50L );
        curl_easy_setopt( curl, CURLOPT_TCP_KEEPALIVE, 1L );
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );

        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, writeToString );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &data );
        curl_easy_setopt( curl, CURLOPT_HEADERDATA, &header_string );
        curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1" );
		
        res = curl_easy_perform( curl );

        curl_easy_cleanup( curl );
    }

    return data;
}