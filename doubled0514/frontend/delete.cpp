#include "httplib.h"
#include <iostream>

using namespace std;

int main(){
    httplib::Client cli("127.0.0.1:5000");
    string endpoint = "/users/1/qwerty1234";
    auto res = cli.Delete(endpoint);
    if(res)
        cout << res -> body << endl;
    else 
        cout << res.error() << endl;
    return 0;

}