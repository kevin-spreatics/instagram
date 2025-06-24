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

// 포스트 올리기
void post_upload(){
    Client cli("http://127.0.0.1:5000");

    string title;
    string text;

    string endpoint = "/users/" + to_string(user_id) + "/posts";

    cout << "제목을 입력하세요: ";
    cin >> title;
    cout << "내용을 입력하세요: ";
    cin >> text;

    json body;

    body["title"] = title;
    body["text"] = text;

    auto res = cli.Post(endpoint, body.dump(), "application/json");

    json response = json::parse(res->body);

    if(response.contains("title") && !response["title"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << user_id << endl;
        cout << "title: " << response["title"] << endl;
        cout << "text: " << response["text"] << endl;
    }
    else{
        cout << "posting_failed: " << response["reason"] << endl;
        return;
    }
}

// 특정 포스트 조회하기
void select_post(){
    Client cli("http://127.0.0.1:5000");

    int post_id;
    cout << "조회할 post의 id를 입력하세요: ";
    cin >> post_id;

    string endpoint = "/users/" + to_string(user_id) + "/posts/" + to_string(post_id);
    
    auto res = cli.Get(endpoint);

    json response = json::parse(res->body);

    if(response.contains("user_id") && !response["user_id"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << response["user_id"] << endl;
        cout << "post_id: " << response["post_id"] << endl;
        cout << "title: " << response["title"] << endl;
        cout << "text: " << response["text"] << endl;
        cout << "create_at: " << response["created_at"] << endl;
    }
    else{
        cout << "특정 포스트를 조회할 수 없습니다." << endl;
        return;
    }
}

// 특정 포스트에 커맨트 달기
void post_comment(){
    Client cli("http://127.0.0.1:5000");
    
    int post_id;

    cout << "커맨트를 달 포스트의 id를 입력하세요: ";
    cin >> post_id;

    string text;
    string endpoint = "/users/" + to_string(user_id) + "/post/" + to_string(post_id) + "/comments";

    cout << "커맨트를 입력하세요: ";
    cin >> text;

    json body;
    body["text"] = text;

    auto res = cli.Post(endpoint, body.dump(), "application/json");

    json response = json::parse(res->body);

    if(response.contains("user_id") && !response["user_id"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << response["user_id"] << endl;
    }
    else{
        cout << "failed: " << response["reason"] << endl;
        return;
    }
}

// 특정 포스트에 달린 커맨트 조회하기
void select_post_comment(){
    Client cli("http://127.0.0.1:5000");

    int post_id;
    cout << "커맨트를 조회할 특정 포스트의 id를 입력하세요: ";
    cin >> post_id;

    string endpoint = "/users/" + to_string(user_id) + "/posts/" + to_string(post_id) + "/comments";

    auto res = cli.Get(endpoint);

    json response = json::parse(res->body);

    if(response.contains("comment_id") && !response["comment_id"].is_null()){
        cout << "status: " << response["status"] << endl;
        cout << "user_id: " << response["user_id"] << endl;
        cout << "post_id: " << response["post_id"] << endl;
        cout << "comment_id: " << response["comment_id"] << endl;
        cout << "text: " << response["text"] << endl;
        cout << "create_at: " << response["created_at"] << endl;
    }
    else{
        cout << "커맨트를 조회할 수 없습니다." << endl;
        return;
    }
}

// 올라온 포스트 조회하기
void collection_posts(){
    Client cli("http://127.0.0.1:5000");

    string endpoint = "/users/" + to_string(user_id) + "/posts";

    auto res = cli.Get(endpoint);

    if(res)
        cout << res->body << endl;
    else{
        cout << res.error() << endl;
        return;
    }

    while(1){
        cout << "(1) 특정 포스트 조회하기" << endl;
        cout << "(2) 특정 포스트에 커맨트 달기" << endl;
        cout << "(3) 특정 포스트에 달린 커맨트 조회하기" << endl;
        cout << "(4) 돌아가기" << endl;
        
        int input;
        cin >> input;

        switch(input){
            case 1:
                select_post();
                break;
            case 2:
                post_comment();
                break;
            case 3:
                select_post_comment();
                break;
            case 4:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }
    }
}

// 팔로우 하위기능 선택
void Follow(){}

// 메세지 하위기능 선택
void DM(){}

// 포스팅 하위 기능 선택
void posting(){
    cout << "Posting~~" << endl;

    cout << "(1) 포스트 올리기" << endl;
    cout << "(2) 올라온 포스트 조회하기" << endl;
    cout << "(3) 돌아가기" << endl;

    int input;
    cin >> input;

    switch(input){
        case 1:
            post_upload();
            break;
        case 2:
            collection_posts();
            break;
        case 3:
            return;
            break;
        default:
            cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
    }
}

// 마이페이지 하위 기능 선택
void my_page(){
    while(1){
        cout << "My page 입니다." << endl;

        cout << "(1) 사용자 정보 조회" << endl;
        cout << "(2) 사용자 정보 수정" << endl;
        cout << "(3) 사용자 계정 삭제" << endl;
        cout << "(4) 돌아가기" << endl;

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
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
        }
    }
}

// 로그인 후 기능 선택
void choise_func(){
    while(1) {
        cout << "기능을 선택하세요." << endl;
        cout << "(1) My page" << endl;
        cout << "(2) Posting" << endl;
        cout << "(3) Follow" << endl;
        cout << "(4) DM" << endl;
        cout << "(5) log_out" << endl;

        int input;
        cin >> input;

        switch(input){
            case 1:
                my_page();
                break;
            case 2:
                posting();
                break;
            case 3:
                Follow();
                break;
            case 4:
                DM();
                break;
            case 5:
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