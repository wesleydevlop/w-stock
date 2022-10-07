#define CURL_STATICLIB // Static LIBS BECAUSE WHY NOT?

#include <iostream> 
#include "curl/curl.h"
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "Data.hpp"
#include "neural_network.hpp"


#pragma comment(lib, "curl/libcurl_a.lib")
#pragma comment (lib, "normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "comdlg32.lib")


using namespace std;

class CurlObj {
public:
    /**
     * @brief Constructor for a curl object.
     *
     * @param url The url of the page to scrape.
     */
    CurlObj(string url) {
        curl = curl_easy_init();
        if (!curl) {
            throw  string("Curl did not initialize.");
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlObj::curlWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_perform(curl);
    };

    static int curlWriter(char* data, int size, int nmemb, string* buffer) {
        int result = 0;
        if (buffer != NULL) {
            buffer->append(data, size * nmemb);
            result = size * nmemb;
        }

        return result;
    }

    string getData() {
        return curlBuffer;
    }

protected:
    CURL* curl;
    string curlBuffer;
};
//credit to stack overflow for help on the CurlObj class

int main() {

    //the tickers of companies
    vector< string> names;
    string numCompaniesString;

    //get number of companies to research
    cout << "Enter the number of companies whose stocks you would like to analyze: ";
    int numCompanies = -1;
    while (numCompanies < 0) {
        try {
            cin >> numCompaniesString;
            numCompanies = stoi(numCompaniesString);
            if (numCompanies < 0) {
                cout << "That is not a valid number. Enter any number greater than 0: ";
            }
        }
        catch (exception e) {
            cout << "That is not a valid number. Enter any number greater than 0: ";
        }
    }

    //enter the wanted companies' tickers
    for (int i = 0; i < numCompanies; ++i) {
        string name;
        cout << "Enter the ticker symbol of a company: ";
        cin >> name;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        names.push_back(name);
    }

    cout << "Please wait for your data. This may take a few seconds." << endl << endl;

    vector<Data> dataObjs;

    //get all the data and print it out
    for (size_t i = 0; i < names.size(); i++) {
        string name = names[i];
        string address = "https://www.marketbeat.com/stocks/NASDAQ/" + name;
        CurlObj temp(address);


        try {
            Data data = Data::Data(name, temp.getData());
            data.printData();
        }
        catch (exception e) {
            cout << "This is not a vaild.";
        }

    }




    cout << endl;

    cout << "Testing the w-stock AI trade :)" << endl;

    float fakecurrency = 1000;

    
    std::vector<uint32_t> topology = { 2,3,1 };
    sp::SimpleNeuralNetwork nn(topology, 0.1);

    //sample dataset
    std::vector<std::vector<float>> targetInputs = {
        {0.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f}
    };
    std::vector<std::vector<float>> targetOutputs = {
        {0.0f},
        {0.0f},
        {1.0f},
        {1.0f}
    };

    uint32_t epoch = 100000;

    //training the neural network with randomized data
    std::cout << "training start\n";

    for (uint32_t i = 0; i < epoch; i++)
    {
        uint32_t index = rand() % 4;
        nn.feedForword(targetInputs[index]);
        nn.backPropagate(targetOutputs[index]);
    }

    std::cout << "training complete\n";


    //testing the neural network
    for (std::vector<float> input : targetInputs)
    {
        nn.feedForword(input);
        std::vector<float> preds = nn.getPredictions();
        std::cout << input[0] << "," << input[1] << " => " << preds[0] << std::endl;
    }




    return 0;
}


