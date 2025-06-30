#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace httplib;
using json = nlohmann::json;


Client cli("52.63.106.255", 5000);  // EC2 서버 주소
string user_input;
string session_cookie;  // 서버가 Set-Cookie로 준 값 저장
string current_post_id;

// 로그인 여부를 저장하는 변수 (true면 로그인 상태)
bool isLoggedIn = false;

// 현재 로그인한 사용자의 닉네임 (user_id는 백엔드에서 관리)
string currentUserNickname;

bool end_input = false;

// 함수 선언부
void showLoginMenu(); // 로그인 및 회원가입
void showUserMenu(); // 사용자 기능(프로필 확인, 포스트)
void showSocialMenu(); // 소셜 기능(사용자 검색)
void showDMMenu(); // dm 기능(특정 사용자와의 대화 보기)
void showMyProfileMenu(const string& nickname); // 내 프로필 메뉴(내 정보 조회, 포스트 보기/작성, 팔로우 관리)
void showOtherUserMenu(const string& nickname);// 타인 프로필 메뉴(dm 전송, 팔로우 신청, 포스트 보기 및 댓글 작성)
void myinfo(); // 내 정보 조회/수정/삭제
void mypost(const string& nickname); // 내 포스트 관리 메뉴(포스트 작성/조회)
void myfollowing(); // 팔로우 관리(팔로워/팔로일 조회, 팔로우 요청 수락/거절)
void sendingDM(const string& toUser); // dm 전송
void sendingfollow(const string& toUser); // 팔로우 요청
void showpost(const string& nickname); // 포스트 보기, 댓글 작성

bool loginRequest(const string& nickname, const string& password);
bool signupRequest(const string& nickname, const string& name, const string& password,
                   const string& email, int age);


bool loginRequest(const string& nickname, const string& password) {
    json request_body = {
        {"nickname", nickname},
        {"password", password}
    };

    auto res = cli.Post("/login", request_body.dump(), "application/json");

    if (res && res->status == 200) {
        // 쿠키 저장
        if (res->has_header("Set-Cookie")) {
            session_cookie = res->get_header_value("Set-Cookie");
            cli.set_default_headers({{"Cookie", session_cookie}});
        }
        return true;
    }

    return false;
}

bool signupRequest(const string& nickname, const string& name, const string& password,
                   const string& email, int age) {
    json request_body = {
        {"nickname", nickname},
        {"name", name},
        {"password", password},
        {"email", email},
        {"age", age}
    };

    auto res = cli.Post("/signup", request_body.dump(), "application/json");

    return res && res->status == 201;
}



// 메인 함수 (프로그램 시작점)
int main() {
    int choice;

    while (true) {
        // 메인 메뉴 출력
        cout << "\n[메인메뉴]" << endl;
        cout << "1. 로그인/회원가입" << endl;

        // 로그인된 상태에서만 접근 가능한 메뉴
        if (isLoggedIn) {
            cout << "2. 사용자 기능" << endl;
            cout << "3. 소셜 기능" << endl;
            cout << "4. 메시지 기능" << endl;
        }

        cout << "0. 종료" << endl;
        cout << "선택: ";
        cin >> choice;

        // 메뉴 선택 처리
        switch (choice) {
            case 1:
                showLoginMenu();  // 로그인 또는 회원가입
                break;
            case 2:
                if (isLoggedIn) showUserMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 3:
                if (isLoggedIn) showSocialMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 4:
                if (isLoggedIn) showDMMenu();
                else cout << "로그인이 필요합니다.\n";
                break;
            case 0:
                cout << "종료합니다." << endl;
                return 0;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 로그인 및 회원가입 메뉴
void showLoginMenu() {
    int choice;
    string nickname, password;

    cout << "\n[로그인/회원가입]" << endl;
    cout << "1. 로그인" << endl;
    cout << "2. 회원가입" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";
    cin >> choice;

    switch (choice) {
        case 1:
            // 로그인 입력
            cout << "닉네임 입력: ";
            cin >> nickname;
            cout << "비밀번호 입력: ";
            cin >> password;

            // POST /login 호출로 백엔드 인증 요청
            // 로그인 성공 시 아래 처리
            if (loginRequest(nickname, password)) {
                currentUserNickname = nickname;
                isLoggedIn = true;
                cout << nickname << "님 로그인 완료." << endl;
            } else {
                cout << "로그인 실패. 닉네임 또는 비밀번호를 확인하세요." << endl;
            }
            break;

        case 2: {
            string name, email;
            int age;

            cout << "닉네임 입력: ";
            cin >> nickname;
            cout << "이름 입력: ";
            cin >> name;
            cout << "비밀번호 입력: ";
            cin >> password;
            cout << "이메일 입력: ";
            cin >> email;
            cout << "나이 입력: ";
            cin >> age;

            if (signupRequest(nickname, name, password, email, age)) {
                cout << "회원가입 성공! 로그인 해주세요." << endl;
            } else {
                cout << "회원가입 실패. 중복된 닉네임일 수 있습니다." << endl;
            }
            break;
        }
        case 0:
            return;

        default:
            cout << "잘못된 입력입니다." << endl;
    }
}

// 사용자 기능 메뉴 (프로필 확인, 게시글 등)
void showUserMenu() {
    string targetNickname;
    cout << "\n조회할 사용자 닉네임 입력: ";
    cin >> targetNickname;

    if (targetNickname == currentUserNickname) {
        // 본인 프로필
        showMyProfileMenu(currentUserNickname);
    } else {
        // 다른 사람 프로필
        showOtherUserMenu(targetNickname);
    }
}

// 내 프로필 메뉴
void showMyProfileMenu(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[" << nickname << "님의 프로필]" << endl;
        cout << "[닉네임, 이메일, 나이 등 자동 조회됨]" << endl;
        cout << "1. 내 정보 조회/수정/삭제" << endl;
        cout << "2. 내 포스트 보기/작성" << endl;
        cout << "3. 팔로우 관리" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1: myinfo(); break;
            case 2: mypost(nickname); break;
            case 3: myfollowing(); break;
            case 0: return;
            default: cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 타인 프로필 메뉴
void showOtherUserMenu(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[" << nickname << "의 프로필]" << endl;
        cout << "[해당 유저 정보 자동 조회]" << endl;
        cout << "1. 메시지 보내기" << endl;
        cout << "2. 팔로우 신청" << endl;
        cout << "3. 포스트 보기 및 댓글 작성" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1: sendingDM(nickname); break;
            case 2: sendingfollow(nickname); break;
            case 3: showpost(nickname); break;
            case 0: return;
            default: cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 내 정보 조회/수정/삭제
void myinfo() {
    int choice;
    while (true) {
        cout << "\n[내 정보 관리]" << endl;
        cout << "1. 정보 수정" << endl;
        cout << "2. 계정 삭제" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // PUT /users/:id 요청
                cout << "수정할 항목 입력받고 백엔드에 전송" << endl;
                break;
            case 2:
                // DELETE /users/:id 요청
                cout << "계정 삭제됨. 로그아웃 처리됩니다." << endl;
                isLoggedIn = false;
                currentUserNickname = "";
                return;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 내 게시글 관리 메뉴
void mypost(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[내 포스트]" << endl;
        cout << "[내 포스트 목록 자동 조회]" << endl;
        cout << "1. 포스트 작성" << endl;
        cout << "2. 포스트 조회" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // POST /posts 요청
                cout << "포스트 작성" << endl;
                break;
            case 2:
                showpost(nickname);  // 조회
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 게시글 보기 + 댓글 작성
void showpost(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[" << nickname << "의 게시글 목록]" << endl;
        cout << "[백엔드에서 게시글 목록 자동 조회]" << endl;
        cout << "1. 댓글 작성" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        if (choice == 1) {
            // POST /posts/:id/comment
            cout << "댓글 입력 후 백엔드 전송" << endl;
        } else if (choice == 0) {
            return;
        } else {
            cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 팔로우 관리
void myfollowing() {
    int choice;
    while (true) {
        cout << "\n[팔로우 관리]" << endl;
        cout << "1. 팔로워/팔로잉 조회" << endl;
        cout << "2. 팔로우 요청 수락/거절" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // GET /follow/list
                cout << "팔로워 및 팔로잉 목록 조회" << endl;
                break;
            case 2:
                // GET /follow/requests + POST /follow/respond
                cout << "팔로우 요청 목록 조회 및 응답" << endl;
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

// DM 보내기
void sendingDM(const string& toUser) {
    cin.ignore();  // 입력 버퍼 제거
    string message;
    cout << "[DM 보내기]" << endl;
    cout << "메시지 입력: ";
    getline(cin, message);

    // POST /dm
    cout << "[DM 전송 요청] to: " << toUser << ", message: " << message << endl;
}

// 팔로우 요청 보내기
void sendingfollow(const string& toUser) {
    cout << "[팔로우 요청]" << endl;
    cout << toUser << "님에게 팔로우 요청을 보냅니다." << endl;

    // POST /follow/request
}

// 소셜 메뉴
void showSocialMenu() {
    int choice;
    string keyword;

    cout << "\n[소셜 기능]" << endl;
    cout << "1. 사용자 검색" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "닉네임 키워드 입력: ";
            cin >> keyword;

            // GET /users?keyword=xxx
            cout << "[검색 요청 → 결과 목록 출력]" << endl;
            break;
        case 0:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
    }
}

// DM 메뉴
void showDMMenu() {
    int choice;
    string targetUser;

    cout << "\n[DM 기능]" << endl;
    cout << "1. 특정 사용자와 대화 보기" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "상대방 닉네임 입력: ";
            cin >> targetUser;

            // GET /dm/conversation?user1_id=&user2_id=
            cout << "[대화 내용 자동 조회]" << endl;
            break;
        case 0:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
    }
}
