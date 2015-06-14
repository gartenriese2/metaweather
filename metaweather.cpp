#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
using namespace std;

// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
static size_t WriteCallback( void *contents, size_t size, size_t nmemb, void *userp )
{
    ( ( std::string* )userp )->append( ( char* )contents, size * nmemb );
    return size * nmemb;
}

float temperature( const string& location )
{
    CURL *curl;
    CURLcode res;

    string request = "http://api.openweathermap.org/data/2.5/weather?q=" + location + ",de&units=metric";
    string readBuffer;

    // cout << "requesting '"  << request << "'" << endl;

    curl = curl_easy_init();
    if( !curl ) 
        cerr << "curl error" << endl;

    curl_easy_setopt( curl, CURLOPT_URL, request.c_str() );
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteCallback );
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );
    res = curl_easy_perform( curl );
    curl_easy_cleanup( curl );

    // cout << readBuffer << endl;

    size_t found = readBuffer.find( "\"temp\":" );
    if( found == string::npos )
        cerr << "temperature could not be found" << endl;

    string temp = readBuffer.substr( found );
    size_t magic = 7;
    temp = temp.substr( magic, temp.find( "," ) - magic );

    return strtof( temp.c_str(), nullptr );
}

int main( int argc, char** argv )
{
    float t = temperature( "erlangen" );
    cout << "Temperature in Erlangen is " << t << " degrees"  << endl;
    return 0;
}
