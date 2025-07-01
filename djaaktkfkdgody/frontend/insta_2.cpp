#include <iostream>
#include <string>
using namespace std;

// 로그인 여부를 저장하는 변수 (true면 로그인 상태)
bool isLoggedIn = false;

// 현재 로그인한 사용자의 닉네임 (user_id는 백엔드에서 관리)
string currentUserNickname;

// 함수 선언부
void showLoginMenu();
void showUserMenu();
void showSocialMenu();
void showDMMenu();
void showMyProfileMenu(const std::string& nickname);
void showOtherUserMenu(const std::string& nickname);
void myinfo();
void mypost(const std::string& nickname);
void myfollowing();
void sendingDM(const std::string& toUser);
void sendingfollow(const std::string& toUser);
void showpost(const std::string& nickname);

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
            cout << "4. DM함" << endl;
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

            // 👉 POST /login 호출로 백엔드 인증 요청
            // 로그인 성공 시 아래 처리
            currentUserNickname = nickname;
            isLoggedIn = true;
            cout << nickname << "님 로그인 완료." << endl;
            break;

        case 2:
            // 👉 회원가입 정보 입력 후 POST /users 요청
            cout << "닉네임, 이름, 비밀번호, 이메일, 나이 입력 받기" << endl;
            break;

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
        cout << "1. DM 보내기" << endl;
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
                // 👉 PUT /users/:id 요청
                cout << "수정할 항목 입력받고 백엔드에 전송" << endl;
                break;
            case 2:
                // 👉 DELETE /users/:id 요청
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
                // 👉 POST /posts 요청
                cout << "제목과 내용 입력 받아 게시글 작성 요청" << endl;
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
            // 👉 POST /posts/:id/comment
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
                // 👉 GET /follow/list
                cout << "팔로워 및 팔로잉 목록 조회" << endl;
                break;
            case 2:
                // 👉 GET /follow/requests + POST /follow/respond
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

    // 👉 POST /dm
    cout << "[DM 전송 요청] to: " << toUser << ", message: " << message << endl;
}

// 팔로우 요청 보내기
void sendingfollow(const string& toUser) {
    cout << "[팔로우 요청]" << endl;
    cout << toUser << "님에게 팔로우 요청을 보냅니다." << endl;

    // 👉 POST /follow/request
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

            // 👉 GET /users?keyword=xxx
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

            // 👉 GET /dm/conversation?user1_id=&user2_id=
            cout << "[대화 내용 자동 조회]" << endl;
            break;
        case 0:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
    }
}
