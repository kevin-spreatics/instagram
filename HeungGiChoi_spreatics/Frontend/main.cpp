#include "httplib.h"
#include "json.hpp"
#include <iostream>

using namespace std;
using namespace httplib;
using namespace nlohmann;

// 회원가입(사용자 생성)
void user_create() {
    Client cli("http://127.0.0.1:5000");

    string nickname;
    string password;
    string name;
    int age;
    string email;

    cout << "사용자 생성!" << endl;
    cout << "사용자 정보를 입력하세요." << endl;
    cout << "nickname: ";
    cin >> nickname;
    cout << "password: ";
    cin >> password;
    cout << "name: ";
    cin >> name;
    cout << "age: ";
    cin >> age;
    cout << "email: ";
    cin >> email;

    json body;
    
    body["nickname"] = nickname;
    body["password"] = password;
    body["name"] = name;
    body["age"] = age;
    body["email"] = email;
    
    auto res = cli.Post("/create_user", body.dump(), "application/json");

    if(res)
        cout << res->body << endl;
    else
        cout << res.error() << endl;
}

// 사용자 로그인
void user_auth() {
    Client cli("http://127.0.0.1:5000");

    string nickname;
    string password;
    
    cout << "로그인" << endl;
    cout << "nickname: ";
    cin >> nickname;
    cout << "password: ";
    cin >> password;

    json body;
    
    body["nickname"] = nickname;
    body["password"] = password;
    
    auto res = cli.Post("/Auth_users", body.dump(), "application/json");

    if(res)
        cout << res->body << endl;
    else
        cout << res.error() << endl;
}

// 사용자 정보 조회
void select_user() {
    Client cli("http://127.0.0.1:5000");

    cout << "user_id 입력: " << endl;
    int user_id;
    cin >> user_id;

    string endpoint = "/users/" + to_string(user_id);

    auto res = cli.Get(endpoint);
        
    if(res)
        cout << res->body << endl;
    else
        cout << res.error() << endl;
}

// 사용자 정보 수정
void user_info_update() {
    Client cli("http://127.0.0.1:5000");
    
    int user_id;
    
    cout << "user_id를 입력하세요: " << endl;
    cin >> user_id;
    
    string auth_nickname;
    string auth_password;
    string nickname;
    string password;
    string email;

    cout << "사용자 정보 수정을 위해선 재인증해야합니다. " << endl;
    cout << "nickname: " << endl;
    cin >> auth_nickname;
    cout << "password: " << endl;
    cin >> auth_password;
    
    cout << "수정할 정보를 입력하세요. " << endl;
    cout << "nickname: " << endl;
    cin >> nickname;
    cout << "password: " << endl;
    cin >> password;
    cout << "email: " << endl;
    cin >> email;

    json body;

    body["auth_nickname"] = auth_nickname;
    body["auth_password"] = auth_password;
    body["nickname"] = nickname;
    body["password"] = password;
    body["email"] = email;

    string endpoint = "/user_update/" + to_string(user_id);

    auto res = cli.Put(endpoint, body.dump(), "application/json");

    if(res)
        cout << res->body << endl;
    else
        cout << res.error() << endl;
}

void choise_func(){
    while(1) {
        cout << "기능을 선택하세요." << endl;
        cout << "(1) 사용자 생성" << endl;
        cout << "(2) 로그인" << endl;
        cout << "(3) 사용자 조회" << endl;
        cout << "(4) 사용자 정보 수정" << endl;
        cout << "(0) 종료" << endl;

        int input;
        cin >> input;

        switch(input){
            case 1:
                user_create();
                break;
            case 2:
                user_auth();
                break;
            case 3:
                select_user();
                break;
            case 4:
                user_info_update();
                break;
            case 0:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
        }
    }
}

int main() {

    choise_func();

    return 0;
}