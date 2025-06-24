#include "httplib.h"
#include "json.hpp"
#include <iostream>

using namespace std;
using namespace httplib;
using namespace nlohmann;

//전역변수 user_id로 로그인 상태관리
int user_id = -1;

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
    json response = json::parse(res->body);

    if(response.contains("user_id") && !response["user_id"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << response["user_id"] << endl;
    }
    else{
        cout << "user_create_failed: " << response["reason"] << endl;
        return;
    }
}

// 사용자 정보 조회
void select_user() {
    Client cli("http://127.0.0.1:5000");

    string endpoint = "/users/" + to_string(user_id);

    auto res = cli.Get(endpoint);
        
    if(res){
        json response = json::parse(res->body);
        cout << "user_id: " << response["user_id"] << endl;
        cout << "status: " << response["status"] << endl;
        cout << "password: " << response["password"] << endl;
        cout << "nickname: " << response["nickname"] << endl;
        cout << "name: " << response["name"] << endl;
        cout << "email: " << response["email"] << endl;
        cout << "age: " << response["age"] << endl;
    }
    else{
        cout << res.error() << endl;
        return;
    }
}

// 사용자 정보 수정
void user_info_update() {
    Client cli("http://127.0.0.1:5000");
    
    string auth_nickname;
    string auth_password;
    string nickname;
    string password;
    string email;

    cout << "사용자 정보 수정을 위해선 재인증해야합니다. " << endl;
    cout << "nickname: ";
    cin >> auth_nickname;
    cout << "password: ";
    cin >> auth_password;
    
    cout << "수정할 정보를 입력하세요. " << endl;
    cout << "변경할 nickname: " ;
    cin >> nickname;
    cout << "변경할 password: ";
    cin >> password;
    cout << "변경할 email: ";
    cin >> email;

    json body;

    body["auth_nickname"] = auth_nickname;
    body["auth_password"] = auth_password;
    body["nickname"] = nickname;
    body["password"] = password;
    body["email"] = email;

    string endpoint = "/user_update/" + to_string(user_id);

    auto res = cli.Put(endpoint, body.dump(), "application/json");
    json response = json::parse(res->body);

    if(response.contains("nickname") && !response["nickname"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "chg_nickname: " << response["nickname"] << endl;
        cout << "chg_password: " << response["password"] << endl;
        cout << "chg_email: " << response["email"] << endl;
    }
    else {
        cout << "user_info_update_failed: " << response["reason"] << endl;
        return;
    }
}

// 로그아웃
void log_out(){
    user_id = -1;
}

// 사용자 계정 삭제
bool user_account_del(){
    Client cli("http://127.0.0.1:5000");

    string auth_nickname;
    string auth_password;

    cout << "사용자 계정 삭제를 위해선 재인증해야합니다. " << endl;
    cout << "nickname: ";
    cin >> auth_nickname;
    cout << "password: ";
    cin >> auth_password;

    json body;

    body["auth_nickname"] = auth_nickname;
    body["auth_password"] = auth_password;

    string yes_or_no;
    cout << "정말 계정을 삭제하시겠습니까?(Y/N)" << endl;
    cin >> yes_or_no;
    if ((yes_or_no == "n") || (yes_or_no == "N")){
        cout << "계정 삭제를 취소하셨습니다." << endl;
        return false;
    }

    string endpoint = "/users/" + to_string(user_id);

    auto res = cli.Delete(endpoint, body.dump(), "application/json");
    json response = json::parse(res->body);

    if(response.contains("result") && !response["result"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "result: " << response["result"] << endl;
        log_out();
        return true;
    }
    else{
        cout << "user_account_del_failed: " << response["reason"] << endl;
        return false;
    }
}

// 로그인 후 기능 선택
void choise_func(){
    while(1) {
        cout << "기능을 선택하세요." << endl;
        cout << "(1) 사용자 조회" << endl;
        cout << "(2) 사용자 정보 수정" << endl;
        cout << "(3) 사용자 계정 삭제" << endl;
        cout << "(4) 로그아웃" << endl;

        int input;
        cin >> input;

        switch(input){
            case 1:
                select_user();
                break;
            case 2:
                user_info_update();
                break;
            case 3:
                if (user_account_del()){
                    return;
                }
                break;
            case 4:
                cout << "로그아웃 되었습니다." << endl;
                log_out();
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
        }
    }
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
    
    json response = json::parse(res->body);
    
    if(response.contains("user_id") && !response["user_id"].is_null()){
        string here_user_id = to_string(response["user_id"]);
        int new_user_id = stoi(here_user_id);
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << new_user_id << endl;

        user_id = new_user_id;

        choise_func();
    }
    else{
        cout << "res.error()" << endl;
        cout << "log_in_error: " << response["reason"] << endl;
        return;
    }
}

// 초입부
void log_in(){
    while(1){
        cout << "로그인 / 회원가입" << endl;
        cout << "(1) 로그인" << endl;
        cout << "(2) 회원가입" << endl;
        cout << "(0) 종료" << endl;

        int input;
        cin >> input;

        switch(input) {
            case 1:
                user_auth();
                break;
            case 2:
                user_create();
                break;
            case 0:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
                break;
        }
    }
}

int main() {

    log_in();

    return 0;
}