#include "httplib.h"
#include "json.hpp"
#include <iostream>
using namespace std;
using namespace httplib;
using namespace nlohmann;


int main() {
    Client cli("http://43.201.118.45:5001");  // EC2 퍼블릭 IP

    auto res = cli.Get("/user/check?user_id=5");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;
        cout << "nickname: " << response["nickname"] << endl;
        cout << "password: " << response["password"] << endl;
        cout << "name: " << response["name"] << endl;
        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;

    return 0;

}
