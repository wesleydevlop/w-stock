
#define CURL_STATICLIB

#include "Data.hpp"
#include <vector>
#include <iostream>

using namespace std;


#include "Data.hpp"
#include <vector>
#include <iostream>

string getAfter(string a, string n, int b, string web) {
    for (int i = 0; i < b; i++) {
        n = n + web[web.find(a) + a.size() + i];
    }
    return n;
}


Data::Data(string NAME, string HTML) {
    name = NAME;
    //all the html of the website
    string t;
    html = HTML;
    fillData();
}

void Data::fillData() {
    string pChangePlace = "&nbsp;(";
    string lowPlace = "Today's Range</dt><dd class='range-meter'>$";
    //string highPlace = "64%'><span class='range-arrow'>Γû╝</span></span></div>";
    string currentPlace = "<div class='price'><strong>$";
    string time = "class='price-updated'>(As of ";

    string tTime; tTime = getAfter(time, tTime, 21, html);
    
    string tlowTime; tlowTime = getAfter(lowPlace, tlowTime, 6, html);
    low = stod(tlowTime);

    string tcurrentPlace; tcurrentPlace = getAfter(currentPlace, tcurrentPlace, 6, html);
    current = stod(tcurrentPlace);

    string tpChange; tpChange = getAfter(pChangePlace, tpChange, 6, html);




    TIME = tTime;
    CHNG = tpChange;

}

void Data::printData() {
    cout << "\n" << name << ":" << endl;
    cout << "\tCURRENT\t\t" << "$ " << current << "\n\tTODAY LOW\t$ " << low <<
        "\n\t%CHANGE\t\t" << CHNG << 
        "\n\tTIMESTAMP\t" << TIME << "\n" << endl;

}




