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
        cout << "new_user_id" << response["new user id"];
        cout << endl;

        if (response.contains("reason"))
        cout << "reason: " << response["reason"] << endl;
    }
    else
        cout << "요청 실패: " << res.error() << endl;

}


void m_1_loginandsignup() {
    while (1) {
        // 메뉴 출력
        cout << "[1. 로그인 및 회원 가입]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 회원가입" << endl;

        // 사용자 입력 처리
        int input;
        cin >> input;

        switch(input) {
            case 1:

                break;
            case 2:
                m_1_2_signup();
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

                break;
            case 3:
  
                break;
            default:
                break;
        }
    }
}


int main() {
    m_0_mainMenu();
    
    return 0;

}
