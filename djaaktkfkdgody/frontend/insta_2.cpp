#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace httplib;
using json = nlohmann::json;

Client cli("52.63.106.255", 5000);
string session_cookie;
bool isLoggedIn = false;
string currentUserNickname;
int currentUserId = -1;

// 함수 선언
void showLoginMenu();
void showUserMenu();
void showMyProfileMenu(const string& nickname);
void showOtherUserMenu(const string& nickname);
void myinfo();
void mypost(const string& nickname);
void showpost(const string& nickname);
void myfollowing();
void sendingDM(const string& toUser);
void sendingfollow(const string& toUser);
void showSocialMenu();
void showDMMenu();
bool loginRequest(const string& nickname, const string& password);
bool signupRequest(const string& nickname, const string& name, const string& password, const string& email, int age);

// 로그인 요청
bool loginRequest(const string& nickname, const string& password) {
    json request_body = {
        {"nickname", nickname},
        {"password", password}
    };
    auto res = cli.Post("/login", request_body.dump(), "application/json");
    if (res && res->status == 200) {
        auto res_json = json::parse(res->body);
        if (res_json["status"] == "authenticated") {
            currentUserId = res_json["user_id"];
            if (res->has_header("Set-Cookie")) {
                session_cookie = res->get_header_value("Set-Cookie");
                cli.set_default_headers({{"Cookie", session_cookie}});
            }
            return true;
        }
    }
    return false;
}

// 회원가입 요청
bool signupRequest(const string& nickname, const string& name, const string& password, const string& email, int age) {
    json request_body = {
        {"nickname", nickname},
        {"name", name},
        {"password", password},
        {"email", email},
        {"age", age}
    };
    auto res = cli.Post("/users", request_body.dump(), "application/json");
    return res && res->status == 200;
}

int main() {
    int choice;
    while (true) {
        cout << "\n[메인메뉴]\n1. 로그인/회원가입" << endl;
        if (isLoggedIn) {
            cout << "2. 사용자 기능\n3. 소셜 기능\n4. 메시지 기능" << endl;
        }
        cout << "0. 종료\n선택: ";
        cin >> choice;
        switch (choice) {
            case 1: showLoginMenu(); break;
            case 2: showUserMenu(); break;
            case 3: showSocialMenu(); break;
            case 4: showDMMenu(); break;
            case 0: cout << "프로그램을 종료합니다.\n"; return 0;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

void showLoginMenu() {
    int choice;
    string nickname, password;
    cout << "\n[로그인/회원가입]\n1. 로그인\n2. 회원가입\n0. 뒤로가기\n선택: ";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "닉네임: "; cin >> nickname;
            cout << "비밀번호: "; cin >> password;
            if (loginRequest(nickname, password)) {
                currentUserNickname = nickname;
                isLoggedIn = true;
                cout << nickname << "님 로그인 성공!\n";
            } else {
                cout << "로그인 실패.\n";
            }
            break;
        case 2: {
            string name, email;
            int age;
            cout << "닉네임: "; cin >> nickname;
            cout << "이름: "; cin >> name;
            cout << "비밀번호: "; cin >> password;
            cout << "이메일: "; cin >> email;
            cout << "나이: "; cin >> age;
            if (signupRequest(nickname, name, password, email, age))
                cout << "회원가입 성공!\n";
            else
                cout << "회원가입 실패.\n";
            break;
        }
        case 0: return;
        default: cout << "잘못된 입력입니다.\n";
    }
}

void showUserMenu() {
    if (!isLoggedIn) {
        cout << "로그인이 필요합니다.\n";
        return;
    }
    string targetNickname;
    cout << "\n조회할 사용자 닉네임: ";
    cin >> targetNickname;
    if (targetNickname == currentUserNickname)
        showMyProfileMenu(currentUserNickname);
    else
        showOtherUserMenu(targetNickname);
}

void showMyProfileMenu(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[" << nickname << "님의 프로필]\n1. 내 정보 관리\n2. 내 포스트\n3. 팔로우 관리\n0. 뒤로가기\n선택: ";
        cin >> choice;
        switch (choice) {
            case 1: myinfo(); break;
            case 2: mypost(nickname); break;
            case 3: myfollowing(); break;
            case 0: return;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

void myinfo() {
    int choice;
    while (true) {
        cout << "\n[내 정보 관리]\n1. 정보 조회\n2. 정보 수정\n3. 계정 삭제\n0. 뒤로가기\n선택: ";
        cin >> choice;
        if (choice == 1) {
            string path = "/users/" + to_string(currentUserId);
            auto res = cli.Get(path.c_str());
            if (res && res->status == 200) {
                auto user = json::parse(res->body);
                cout << "\n닉네임: " << user["nickname"]
                     << "\n이름: " << user["name"]
                     << "\n이메일: " << user["email"]
                     << "\n나이: " << user["age"] << endl;
            } else cout << "정보 조회 실패\n";
        } else if (choice == 2) {
            json update_data;
            string name, email, password;
            int age;
            cin.ignore();
            cout << "새 이름: "; getline(cin, name);
            cout << "새 이메일: "; getline(cin, email);
            cout << "새 비밀번호: "; getline(cin, password);
            cout << "새 나이: "; cin >> age;
            if (!name.empty()) update_data["name"] = name;
            if (!email.empty()) update_data["email"] = email;
            if (!password.empty()) update_data["password"] = password;
            if (age > 0) update_data["age"] = age;
            auto res = cli.Put( ("/users/" + to_string(currentUserId)).c_str(), update_data.dump(), "application/json");
            cout << (res && res->status == 200 ? "수정 완료\n" : "수정 실패\n");
        } else if (choice == 3) {
            auto res = cli.Delete( ("/users/" + to_string(currentUserId)).c_str() );
            if (res && res->status == 200) {
                cout << "계정이 삭제되었습니다. 로그아웃합니다.\n";
                isLoggedIn = false;
                currentUserId = -1;
                currentUserNickname = "";
                return;
            } else cout << "삭제 실패\n";
        } else if (choice == 0) return;
        else cout << "잘못된 입력입니다.\n";
    }
}

void mypost(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[내 포스트 관리]\n1. 포스트 작성\n2. 포스트 조회\n0. 뒤로가기\n선택: ";
        cin >> choice;
        if (choice == 1) {
            string title, text;
            cin.ignore();
            cout << "제목: "; getline(cin, title);
            cout << "내용: "; getline(cin, text);
            json data = {
                {"user_id", currentUserId},
                {"title", title},
                {"text", text}
            };
            auto res = cli.Post("/posts", data.dump(), "application/json");
            cout << (res && res->status == 200 ? "작성 완료\n" : "작성 실패\n");
        } else if (choice == 2) {
            showpost(nickname);
        } else if (choice == 0) return;
        else cout << "잘못된 입력입니다.\n";
    }
}

void showpost(const string& nickname) {
    auto res = cli.Get(("/users?keyword=" + nickname).c_str());
    if (!res || res->status != 200) {
        cout << "사용자 검색 실패\n";
        return;
    }
    auto users = json::parse(res->body);
    if (users.empty()) {
        cout << "사용자를 찾을 수 없습니다.\n";
        return;
    }
    int uid = users[0]["user_id"];
    auto postRes = cli.Get(("/posts?user_id=" + to_string(uid)).c_str());
    if (!postRes || postRes->status != 200) {
        cout << "게시글 조회 실패\n";
        return;
    }
    auto posts = json::parse(postRes->body);
    for (auto& post : posts) {
        int postId = post["post_id"];
        cout << "\n[ID: " << postId << "] " << post["title"] << "\n내용: " << post["text"] << endl;

        // 댓글 목록 출력 추가
        auto commentsRes = cli.Get(("/posts/" + to_string(postId) + "/comments").c_str());
        if (commentsRes && commentsRes->status == 200) {
            auto comments = json::parse(commentsRes->body);
            if (!comments.empty()) {
                cout << "댓글들:\n";
                for (auto& c : comments) {
                    cout << "  - (" << c["user_id"] << ") " << c["text"] << endl;
                }
            } else {
                cout << "댓글 없음\n";
            }
        } else {
            cout << "댓글 불러오기 실패\n";
        }
    }

    cout << "\n댓글을 작성할 게시글 ID (0: 취소): ";
    int postId; cin >> postId;
    if (postId == 0) return;
    cin.ignore();
    string comment;
    cout << "댓글 내용: "; getline(cin, comment);
    json commentData = {
        {"user_id", currentUserId},
        {"comment", comment}
    };
    auto comRes = cli.Post( ("/posts/" + to_string(postId) + "/comment").c_str(), commentData.dump(), "application/json");
    cout << (comRes && comRes->status == 200 ? "댓글 작성 완료\n" : "댓글 작성 실패\n");
}

void myfollowing() {
    auto res = cli.Get( ("/follow/list?user_id=" + to_string(currentUserId)).c_str() );
    if (res && res->status == 200) {
        auto list = json::parse(res->body);
        cout << "\n[팔로잉 목록]\n";
        for (auto& user : list) {
            cout << "- " << user["nickname"] << endl;
        }
    } else {
        cout << "팔로잉 목록 조회 실패\n";
    }
}

void sendingDM(const string& toUser) {
    auto res = cli.Get(("/users?keyword=" + toUser).c_str());
    if (!res || res->status != 200) return;
    auto users = json::parse(res->body);
    if (users.empty()) return;
    int toId = users[0]["user_id"];
    string msg;
    cin.ignore();
    cout << "메시지 내용: "; getline(cin, msg);
    json data = {
        {"from_user_id", currentUserId},
        {"to_user_id", toId},
        {"message", msg}
    };
    auto dmRes = cli.Post("/dm", data.dump(), "application/json");
    cout << (dmRes && dmRes->status == 200 ? "DM 전송 완료\n" : "DM 전송 실패\n");
}

void sendingfollow(const string& toUser) {
    auto res = cli.Get(("/users?keyword=" + toUser).c_str());
    if (!res || res->status != 200) return;
    auto users = json::parse(res->body);
    if (users.empty()) return;
    int toId = users[0]["user_id"];
    json data = {
        {"from_user_id", currentUserId},
        {"to_user_id", toId}
    };
    auto followRes = cli.Post("/follow/request", data.dump(), "application/json");
    cout << (followRes && followRes->status == 200 ? "팔로우 요청 완료\n" : "팔로우 요청 실패\n");
}

void showOtherUserMenu(const string& nickname) {
    int choice;
    while (true) {
        cout << "\n[" << nickname << "님의 프로필]\n1. DM 보내기\n2. 팔로우 신청\n3. 게시글 보기 및 댓글\n0. 뒤로가기\n선택: ";
        cin >> choice;
        switch (choice) {
            case 1: sendingDM(nickname); break;
            case 2: sendingfollow(nickname); break;
            case 3: showpost(nickname); break;
            case 0: return;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

void showSocialMenu() {
    string keyword;
    cout << "\n검색할 사용자 닉네임 키워드: ";
    cin >> keyword;
    auto res = cli.Get(("/users?keyword=" + keyword).c_str());
    if (res && res->status == 200) {
        auto users = json::parse(res->body);
        for (auto& user : users) {
            cout << "- 닉네임: " << user["nickname"] << endl;
        }
    } else {
        cout << "검색 실패\n";
    }
}

void showDMMenu() {
    string nickname;
    cout << "상대방 닉네임 입력: "; cin >> nickname;
    auto res = cli.Get(("/users?keyword=" + nickname).c_str());
    if (!res || res->status != 200) return;
    auto users = json::parse(res->body);
    if (users.empty()) return;
    int uid = users[0]["user_id"];
    auto convRes = cli.Get(("/dm/conversation?user1_id=" + to_string(currentUserId) + "&user2_id=" + to_string(uid)).c_str());
    if (convRes && convRes->status == 200) {
        auto msgs = json::parse(convRes->body);
        for (auto& m : msgs) {
            cout << m["sender_id"] << ": " << m["text"] << endl;
        }
    } else cout << "대화 내역 조회 실패\n";
}
