#include "httplib.h"
#include "json.hpp"
#include <iostream>
using namespace std;
using namespace httplib;
using namespace nlohmann;


// 0. 메인
//      1. 로그인/회원가입
//          1.1 로그인
//          1.2 회원 가입
//      2. 사용자
//          2.1 비밀번호 바꾸기
//      3. 소셜
//          3.1 포스팅 생성
//          3.2 포스팅 전체 스레드 보기
//          3.3 포스팅 삭제 

Client cli("http://43.201.118.45:5001");  // EC2 퍼블릭 IP

//Global 변수
//1.로그인 한 user_id 
int user_id = 0;


void m_3_3_postdelete(){
    string post_id;

    cout << "포스팅 삭제" << endl;
    cout << "포스트 아이디" << endl;
    cin >> post_id;

    json body;
    body["post_id"] = post_id;

    string endpoint = "/posting/delete";

    auto res = cli.Delete(endpoint, body.dump(), "application/json");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;

}

void m_3_2_allpost(){

    cout << "작성한 포스팅 스레드" << endl;

    string endpoint = "/posting/" + to_string(user_id) + "/check";

    auto res = cli.Get(endpoint);

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;

        if(response.contains("result") && response["result"].is_array()){
            for (auto& post : response["result"]) {
            cout << "post_id: " << post["post_id"] << endl;
            cout << "title: " << post["title"] << endl;
            cout << "text: " << post["text"] << endl;
            cout << "user_id: " << post["user_id"] << endl;
            cout << "----------------------" << endl;
        }
        }
        else{
            cout << "포스팅이 없습니다" << endl;
        }
        cout << "-----------" << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;
}

void m_3_1_posting(){
    string title, post_text;

    cout << "포스팅 생성" << endl;
    cout << "포스트 제목:" << endl;
    cin >> title;
    cout << "포스트 내용:" << endl;
    cin >> post_text;

    json body;
    body["title"] = title;
    body["post_text"] = post_text;

    string endpoint = "/posting/" + to_string(user_id) + "/create";

    auto res = cli.Post(endpoint, body.dump(), "application/json");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;
        cout << "post_id " << response["post_id"] << endl;
        cout << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;

}


void m_3_social(){
    while(1){
        int i;
        cout << "[소셜 기능]" << endl;
        cout << "1. 포스팅 생성" << endl;
        cout << "2. 포스팅 전체 스레드 보기" << endl;
        cout << "3. 포스팅 삭제" << endl;
        cout << "4. 돌아가기" << endl;

        cin >> i;

        switch(i){
            case 1:
                m_3_1_posting();
                break;
            case 2:
                m_3_2_allpost();
                break;
            case 3:
                m_3_3_postdelete();
                return;
            case 4:
                return;
            default:
                break;
        }
    }
}

void m_2_user_pwchange(){
    string pw, new_pw;
    cin.ignore();

    cout << "비밀번호 변경" << endl;
    cout << "기존 비밀번호" << endl;
    getline(cin, pw);
    cout << "새 비밀번호" << endl;
    getline(cin, new_pw);

    json body;
    body["pw"] = pw;
    body["newPW"] = new_pw;

    string endpoint = "/user/" + to_string(user_id) + "/password_change";

    auto res = cli.Patch(endpoint, body.dump(), "application/json");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;
        cout << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;
}

void m_2_user(){
    while(1){
        int i;
        cout << "[사용자 기능]" << endl;
        cout << "1. 비밀번호 바꾸기" << endl;
        cout << "3. 돌아가기" << endl;

        cin >> i;

        switch(i){
            case 1:
                m_2_user_pwchange();
                break;
            case 3:
                return;
            default:
                break;
        }

    }

}

void m_1_2_signup(){
    string nickname, pw, name, age, email;

    cout << "[회원가입]" << endl;
    
    cout << "nickname (필수)" << endl;
    cin >> nickname;

    cout << "pw(필수)" << endl;
    cin >> pw;

    cout << "name(필수)" << endl;
    cin >> name;

    cout << "age(선택), 없으면 enter" << endl;
    getline(cin, age);

    cout << "email(선택), 없으면 enter" << endl;
    getline(cin, email);


    json body;
    body["nickname"] = nickname;
    body["pw"] = pw;
    body["name"] = name;

    if (!age.empty()){
        body["age"] = age;
    }
    if (!email.empty()){
        body["email"] = email;
    }

    auto res = cli.Post("/user/create", body.dump(), "application/json");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;
        cout << "new_user_id: " << response["new user id"];
        user_id = response["new user id"];
        cout << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;

}

void m_1_1_login(){
   string nickname, pw;

    cout << "[로그인]" << endl;
    
    cout << "nickname (필수)" << endl;
    cin >> nickname;

    cout << "pw(필수)" << endl;
    cin >> pw;

    json body;
    body["nickname"] = nickname;
    body["pw"] = pw;

    auto res = cli.Post("/user/login", body.dump(), "application/json");

    if (res) {
        json response = json::parse(res->body);  // 문자열 → json 객체
        cout << "status: " << response["status"] << endl;
        cout << "login user: " << response["login user"] << endl;
        user_id = response["login user"];
        cout << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl; 
}

void m_1_loginandsignup() {
    while (user_id == 0) {
        // 메뉴 출력
        cout << "[1. 로그인 및 회원 가입]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 회원가입" << endl;
        cout << "3. 돌아가기" << endl;
 
        // 사용자 입력 처리
        int input;
        cin >> input;

        switch(input) {
            case 1:
                m_1_1_login();
                break;
            case 2:
                m_1_2_signup();
            case 3:
                return;
            default:
                break;
        }
    }
}

void m_0_mainMenu() {

    while (1) {
        // 메뉴 출력
        cout << "[홈메뉴]" << endl;
        cout << "1. 로그인 및 회원 가입" << endl;
        cout << "2. 사용자" << endl;
        cout << "3. 소셜" << endl;

        // 사용자 입력 처리
        int input;
        cin >> input;
        switch(input) {
            case 1:
                m_1_loginandsignup();
                break;
            case 2:
                if (user_id > 0) {
                    m_2_user();
                } else {
                    cout << "[알림] 로그인 후 이용 가능한 메뉴입니다." << endl;
                }
                break;
            case 3:
                if(user_id > 0) {
                    m_3_social();
                }else{
                    cout << "[알림] 로그인 후 이용 가능한 메뉴입니다." << endl;
                }
            default:
                break;
        }
    }
}


int main() {
    m_0_mainMenu();
    
    return 0;

}
