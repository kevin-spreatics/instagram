#include "httplib.h"
#include <iostream>

int main(){
    httplib::Client cli("127.0.0.1:5000");

    std::string body = R"({
      
    })";
    auto res = cli.Post("/users/search",body,"application/json");
 
    if(res)
        std::cout << res -> body << std::endl;
    else 
        std::cout << res.error() << std::endl;
    return 0;

}
