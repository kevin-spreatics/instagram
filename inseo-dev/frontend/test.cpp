#include "httplib.h"
#include "json.hpp"
#include <iostream>

using namespace std;
using namespace httplib;
using namespace nlohmann;

void test(){
    Client cli("3.34.112.5:5000");
    string endpoint = "/users/search"; 
}
void search_user(string search=""){
    //Client cli("127.0.0.1:5000");
    Client cli("3.34.112.5:5000");
    string endpoint = "/users/search";

    json body;
    body["search"] = search;

    auto res = cli.Post(endpoint,body.dump(),"application/json");

    if (res && res->status == 200){
        //cout << res->body << endl;
        json response = json::parse(res->body);
        cout << "status" << response["status"] << endl;
        cout << "user" << response["user"] << endl;
        //cout << res->body << endl;
    }
    else
        cout << res.error() << endl;
}

void delete_me(int user_id){
    Client cli("127.0.0.1:5000");

    string endpoint = "/users/" + to_string(user_id);

    auto res = cli.Delete(endpoint);

    if (res && res->status == 200){
        cout << res->body << endl;
    }
    else
        cout << res.error() << endl;
}

int main() {

    //delete_me(7);
    search_user();
    return 0;
}