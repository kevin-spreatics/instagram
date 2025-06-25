#include "httplib.h"
#include "json.hpp"
#include <iostream>
using namespace std;
using namespace httplib;
using json = nlohmann::json;

Client cli("http://43.200.10.134:5001");  // EC2 서버 주소
int user_id = -1;

// 로그인 함수
void m_1_login() {
    string nickname, password;

    cout << "로그인" << endl;
    cout << "nickname : ";
    cin >> nickname;

    cout << "password : ";
    cin >> password;

    json body;
    body["nickname"] = nickname;
    body["password"] = password;

    auto res = cli.Post("/users/login", body.dump(), "application/json");

    if (res) {

        try {
            json response = json::parse(res->body);
            if (response["status"] == "success") {
                cout << "== 로그인 성공 ==" << endl;
                user_id = response["user_id"];
                cout << "user_id: " << user_id << endl;
            } else {
                cout << "== 로그인 실패 ==" << endl;
                if (response.contains("reason"))
                    cout << "이유: " << response["reason"] << endl;
            }
        } catch (json::parse_error& e) {
            cout << "== 응답 파싱 실패 ==" << endl;
            cout << "에러 내용: " << e.what() << endl;
        }
    } else {
        cout << "요청 실패. 오류 코드: " << res.error() << endl;
    }
}

// 회원가입 함수
void m_1_signup() {
    string nickname, password, name, age, email;
    cin.ignore(); 

    cout << "[회원가입]" << endl;

    cout << "nickname: ";
    getline(cin, nickname);

    cout << "password: ";
    getline(cin, password);

    cout << "name: ";
    getline(cin, name);

    cout << "age (없으면 엔터): ";
    getline(cin, age);

    cout << "email (없으면 엔터): ";
    getline(cin, email);

    json body;
    body["nickname"] = nickname;
    body["password"] = password;
    body["name"] = name;
    if (!age.empty()) body["age"] = age;
    if (!email.empty()) body["email"] = email;

    auto res = cli.Post("/users", body.dump(), "application/json");

    if (res) {

        try {
            json response = json::parse(res->body);
            if (response["status"] == "created") {
                cout << "== 회원가입 성공 ==" << endl;
                user_id = response["user_id"];
                cout << "user_id: " << user_id << endl;
            } else {
                cout << "== 회원가입 실패 ==" << endl;
                if (response.contains("reason"))
                    cout << "이유: " << response["reason"] << endl;
            }
        } catch (json::parse_error& e) {
            cout << "== 응답 파싱 실패 ==" << endl;
            cout << "에러 내용: " << e.what() << endl;
        }
    } else {
        cout << "요청 실패. 오류 코드: " << res.error() << endl;
    }
}

// 로그인/회원가입 메뉴
void m_1_userLogin() {
    while (true) {
        cout << "[ 1. 로그인 & 회워가입 ]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 회원가입" << endl;
        cout << "3. 뒤로가기" << endl;

        int input;
        cin >> input;
        switch (input) {
            case 1:
                m_1_login();
                break;
            case 2:
                m_1_signup();
                break;
            case 3:
                return;
                break;
            default:
                break;
        }
    }
}

void m_2_1_self() {
    cout << "본인 메뉴 처리" << endl;
}

void m_2_2_others() {
    cout << "다른 사용자 메뉴 처리" << endl;
}

void m_2_user() {
    while (1) {
        // 메뉴 출력
        cout << "[2. 사용자]" << endl;
        cout << "1. 본인" << endl;
        cout << "2. 다른 유저" << endl;
        cout << "3. 뒤로가기" << endl;

        // 사용자 입력 처리
        int input;
        cin >> input;
        switch(input) {
            case 1:
                m_2_1_self();
                break;
            case 2:
                m_2_2_others();
                break;
            case 3:
                return;
                break;
            default:
                break;
        }
    }
}
// 메인 메뉴
void m_0_mainMeun() {
    while (1) {
        cout << "[홈메뉴]" << endl;
        cout << "1. 로그인" << endl;
        cout << "2. 사용자" << endl;
        cout << "3. 소셜" << endl;
        cout << "4. DM" << endl;
        cout << "5. 종료" << endl;

        int input;
        cin >> input;
        switch(input) {
            case 1:
                m_1_userLogin();
                break;
            case 2:
                m_2_user();
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                return;
                break;
            default:
                break;
        }
    }
}

int main() {
    m_0_mainMeun();  
    return 0;
}