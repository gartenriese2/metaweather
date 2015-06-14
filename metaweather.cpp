#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

class Curl {
public:
    Curl()
     :  m_handle{curl_easy_init()}
    {
        if (!m_handle)
        {
            std::cerr << "Curl could not be initialized!\n";
        }
    }
    ~Curl()
    {
        curl_easy_cleanup(m_handle);
    }
    void request(const std::string& url) const
    {
        curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
    }
    void setWriteCallback() const
    {
        curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    }
    void writeData(std::string* buf)
    {
        curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, buf);
    }
    const CURLcode perform()
    {
        return curl_easy_perform(m_handle);
    }
private:
    CURL* m_handle;
};

// https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    (static_cast<std::string*>(userp))->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

float temperature(const std::string& location)
{
    
    const std::string request {"http://api.openweathermap.org/data/2.5/weather?q=" + location + ",de&units=metric"};

    // std::cout << "requesting '"  << request << "'" << std::endl;

    auto curl{std::make_unique<Curl>()};
    curl->request(request);
    curl->setWriteCallback();
    std::string readBuffer;
    curl->writeData(&readBuffer);
    
    const auto res = curl->perform();

    // std::cout << readBuffer << std::endl;

    const size_t found = readBuffer.find("\"temp\":");
    if (found == std::string::npos)
    {
        std::cerr << "temperature could not be found" << std::endl;
    }

    std::string temp = readBuffer.substr(found);
    constexpr size_t magic = 7;
    temp = temp.substr(magic, temp.find(",") - magic);

    return strtof(temp.c_str(), nullptr);
}

int main()
{
    const float t = temperature("erlangen");
    std::cout << "Temperature in Erlangen is " << t << " degrees (Celsius)" << std::endl;
    return EXIT_SUCCESS;
}
