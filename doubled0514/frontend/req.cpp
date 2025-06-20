#include "httplib.h"
#include <iostream>
#include "json.hpp"

using namespace std;
using namespace httplib;
using namespace nlohmann;
int main(){
    httplib::Client cli("127.0.0.1:5000");
    string nickname = "rlagustj1234";
    json body;
    body["nickname"] = nickname;

    auto res = cli.Post("/users/search",body.dump(),"application/json");

    if(res) {
        json response = json::parse (res->body);
        cout << "user_id: " <<response["user_id"]<<endl;
    }
    else{
        cout << res.error() <<endl;
    }
}

