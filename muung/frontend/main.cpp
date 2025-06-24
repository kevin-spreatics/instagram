#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <locale.h> // 한글 출력을 위해 추가
#include <cstdlib> // 추가
#include <limits> // 맨 위에 추가

using std::cin;
using std::cout;
using std::endl;
using std::string;
using json = nlohmann::json;
using namespace std;

// 서버 주소 (수정 필요)
httplib::Client server("http://127.0.0.1:5000");

// 로그인 상태 저장
int g_user_id = -1;

// 회원가입 함수
void do_signup() {
    cout << "아이디를 입력하세요: ";
    string nickname, name, password, email, age;
    cout << "\n=== 회원가입 ===" << endl;
    cout << "닉네임: "; getline(cin, nickname);
    cout << "이름: "; getline(cin, name);
    cout << "비밀번호: "; getline(cin, password);
    cout << "이메일(선택): "; getline(cin, email);
    cout << "나이(선택): "; getline(cin, age);

    json req;
    req["nickname"] = nickname;
    req["name"] = name;
    req["password"] = password;
    if (!email.empty()) req["email"] = email;
    if (!age.empty()) req["age"] = age;

    auto res = server.Post("/users", req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    if (res->body.empty() || res->body[0] != '{') {
        cout << "[오류] 서버에서 올바른 JSON이 오지 않았습니다.\n응답 내용: " << res->body << endl;
        return;
    }
    json resp = json::parse(res->body);
    if (resp.contains("user_id")) {
        cout << "[회원가입 성공] user_id: " << resp["user_id"] << endl;
    } else {
        cout << "[회원가입 실패] ";
        if (resp.contains("reason")) cout << resp["reason"] << endl;
        else cout << "알 수 없는 오류" << endl;
    }
    cout << endl;
}

// 로그인 함수
void do_login() {
    string nickname, password;
    cout << "\n=== 로그인 ===" << endl;
    cout << "닉네임: "; cout.flush(); getline(cin, nickname);
    cout << "비밀번호: "; cout.flush(); getline(cin, password);

    json req;
    req["nickname"] = nickname;
    req["password"] = password;

    auto res = server.Post("/users/login", req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    if (res->body.empty() || res->body[0] != '{') {
        cout << "[오류] 서버에서 올바른 JSON이 오지 않았습니다.\n응답 내용: " << res->body << endl;
        return;
    }
    json resp = json::parse(res->body);
    if (resp.contains("status") && resp["status"] == "success") {
        cout << "[로그인 성공] user_id: " << resp["user_id"] << endl;
        g_user_id = resp["user_id"];
    } else {
        cout << "[로그인 실패] ";
        if (resp.contains("reason")) cout << resp["reason"] << endl;
        else cout << "알 수 없는 오류" << endl;
    }
    cout << endl;
}

// 사용자 정보 조회
void user_search() {
    cout << "\n=== 사용자 정보 조회 ===" << endl;
    string user_id, nickname, email;
    cout << "user_id(엔터시 생략): "; cout.flush(); getline(cin, user_id);
    cout << "nickname(엔터시 생략): "; cout.flush(); getline(cin, nickname);
    cout << "email(엔터시 생략): "; cout.flush(); getline(cin, email);

    json req;
    if (!user_id.empty()) req["user_id"] = user_id;
    if (!nickname.empty()) req["nickname"] = nickname;
    if (!email.empty()) req["email"] = email;

    auto res = server.Post("/users/search", req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    cout << "조회 결과: " << res->body << endl << endl;
}

// 포스팅 생성
void create_post() {
    cout << "\n=== 포스팅 생성 ===" << endl;
    if (g_user_id == -1) {
        cout << "로그인 후 이용해 주세요." << endl << endl;
        return;
    }
    string title, text;
    cout << "제목: "; cout.flush(); getline(cin, title);
    cout << "내용: "; cout.flush(); getline(cin, text);

    json req;
    req["user_id"] = g_user_id;
    req["title"] = title;
    req["text"] = text;

    auto res = server.Post("/post", req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    cout << "포스팅 결과: " << res->body << endl << endl;
}

// 포스팅 조회
void view_post() {
    cout << "\n=== 포스팅 조회 ===" << endl;
    string user_id;
    cout << "user_id(엔터시 전체): "; cout.flush(); getline(cin, user_id);

    json req;
    if (!user_id.empty()) req["user_id"] = user_id;

    auto res = server.Post("/post", req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    cout << "포스팅 목록: " << res->body << endl << endl;
}

// 댓글 작성
void write_comment() {
    cout << "\n=== 댓글 작성 ===" << endl;
    if (g_user_id == -1) {
        cout << "로그인 후 이용해 주세요." << endl << endl;
        return;
    }
    string post_id, text;
    cout << "댓글을 달 포스트 ID: "; cout.flush(); getline(cin, post_id);
    cout << "댓글 내용: "; cout.flush(); getline(cin, text);

    json req;
    req["text"] = text;

    string endpoint = "/post/" + post_id + "/comment/" + std::to_string(g_user_id);
    auto res = server.Post(endpoint.c_str(), req.dump(), "application/json");
    if (!res) {
        cout << "[오류] 서버에 연결할 수 없습니다." << endl;
        return;
    }
    cout << "댓글 결과: " << res->body << endl << endl;
}

int main() {
    cout << "프로그램 시작" << endl;
    setlocale(LC_ALL, "");
    while (true) {
        cout << "=== 인스타그램 콘솔 클라이언트 ===" << endl;
        cout << "1. 회원가입" << endl;
        cout << "2. 로그인" << endl;
        cout << "3. 사용자 정보 조회" << endl;
        cout << "4. 포스팅 생성" << endl;
        cout << "5. 포스팅 조회" << endl;
        cout << "6. 댓글 작성" << endl;
        cout << "0. 종료" << endl;
        cout << "메뉴를 선택하세요: ";
        int menu;
        cin >> menu;
        cin.ignore();

        if (menu == 1) {
            cout << "회원가입을 시작합니다." << endl;
            do_signup();
        } else if (menu == 2) {
            cout << "로그인을 시작합니다." << endl;
            do_login();
        } else if (menu == 3) {
            user_search();
        } else if (menu == 4) {
            create_post();
        } else if (menu == 5) {
            view_post();
        } else if (menu == 6) {
            write_comment();
        } else if (menu == 0) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        } else {
            cout << "잘못된 메뉴입니다." << endl;
        }
    }
    return 0;
} 