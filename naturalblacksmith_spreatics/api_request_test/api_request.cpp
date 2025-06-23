#include "httplib.h"
#include <iostream>
using namespace std;
using namespace httplib;

int main() {
    Client cli("http://43.201.118.45:5001");  // EC2 퍼블릭 IP

    string body = R"({
        "title": "stra",
        "post_text" : "welcome to stra"
    })";

    auto res = cli.Post("/posting/1/create", body, "application/json");

    if (res)
        cout << res->status << ": " << res->body << endl;
    else
        cout << "HTTP 요청 실패: " << res.error() << endl;

    return 0;
}
