#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <regex>
#include <windows.h>

using namespace std;

// 사용자 정보 구조체
struct User {
    string id;
    string email;
    string password;
};

// 포스트 구조체
struct Post {
    string author; // 작성자 아이디
    string title;
    string content; // 본문 내용
    vector<string> comments; // 댓글 목록
};

// 전역 변수
map<string, User> users; // 아이디 → 사용자 정보(모든 사용자 정보를 아이디 기준으로 저장)
string currentUser; // 현재 로그인한 사용자 아이디

vector<Post> postList; // 게시글 목록

map<string, set<string>> followRequests; // 팔로우 요청 저장
map<string, set<string>> follows; // 실제 팔로우 관계 저장
map<pair<string, string>, vector<string>> dmMessages; // (보낸이, 받는이) 쌍 기준으로 dm 저장

// 유틸리티
    // 이메일 형식 체크
    // user@domain.com, u.ser@domain.co.kr 등의 형식 지원
bool isValidEmail(const string& email) {
    regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); 
    return regex_match(email, pattern);
}

// 메뉴 선언
void mainMenu();
void userMenu();
void postMenu();
void socialMenu();
void dmMenu();

// 사용자 기능
void registerUser();
void loginUser();
void showUserInfo();
void updateUser();
void deleteUser();

// 포스트 기능
void createPost();
void viewPosts();

// 소셜 기능
void searchUser();
void requestFollow();
void viewFollowList();
void viewFollowRequests();
void handleFollowRequest();

// 메시지 기능
void sendDM();
void viewDM();
void deleteDM();

// ---------------- 사용자 기능 ----------------
void registerUser() {
    // 로그인 상태인지 확인
    if (!currentUser.empty()) {
        cout << "이미 로그인 상태입니다.\n";
        return;
    }

    // 아이디 중복 체크
    string id, email, pw, pwCheck;
    cout << "[회원가입]\n아이디: ";
    cin >> id;
    if (users.count(id)) {
        cout << "이미 존재하는 아이디입니다.\n";
        return;
    }

    // 이메일 형식 체크
    cout << "이메일: ";
    cin >> email;
    if (!isValidEmail(email)) {
        cout << "이메일 형식이 올바르지 않습니다.\n";
        return;
    }

    // 비밀번호 일치 확인
    cout << "비밀번호: ";
    cin >> pw;
    cout << "비밀번호 확인: ";
    cin >> pwCheck;
    if (pw != pwCheck) {
        cout << "비밀번호가 일치하지 않습니다.\n";
        return;
    }

    // users[id]에 사용자 등록
    users[id] = {id, email, pw};
    cout << "회원가입이 완료되었습니다.\n";
}

void loginUser() {
    // 로그인 상태인지 확인
    if (!currentUser.empty()) {
        cout << "이미 로그인 상태입니다.\n";
        return;
    }
    string input, pw;
    cout << "[로그인] 아이디 또는 이메일 입력: ";
    cin >> input;
    cout << "비밀번호 입력: ";
    cin >> pw;

    // 입력받은 아이디 또는 이메일과 비밀번호가 일치하면 currentUser에 로그인 처리
    for (map<string, User>::iterator it = users.begin(); it != users.end(); ++it) {
        User& user = it->second;
        if ((user.id == input || user.email == input) && user.password == pw) {
            currentUser = user.id;
            cout << "로그인 성공!\n";
            return;
        }
    }
    cout << "로그인 실패.\n";
}

void showUserInfo() {
    if (currentUser.empty()) {
        cout << "로그인 후 이용하세요.\n";
        return;
    }
    User& user = users[currentUser];
    cout << "[사용자 정보]\n아이디: " << user.id << "\n이메일: " << user.email << "\n";
}

void updateUser() {
    // 로그인 필수
    if (currentUser.empty()) {
        cout << "로그인 후 이용하세요.\n";
        return;
    }

    // 아이디, 이메일, 비밀번호 중 하나 수정 가능
    // 아이디 수정 시 기존 id 키를 삭제하고 새 키로 저장
    User user = users[currentUser];
    cout << "1. 아이디 수정\n2. 이메일 수정\n3. 비밀번호 수정\n선택: ";
    int sel;
    cin >> sel;
    string input;
    switch (sel) {
        case 1:
            cout << "새 아이디: ";
            cin >> input;
            if (users.count(input)) {
                cout << "이미 존재합니다.\n";
            } else {
                user.id = input;
                users.erase(currentUser);
                users[input] = user;
                currentUser = input;
                cout << "아이디 수정 완료.\n";
            }
            break;
        case 2:
            cout << "새 이메일: ";
            cin >> input;
            if (!isValidEmail(input))
                cout << "이메일 형식 오류.\n";
            else {
                user.email = input;
                users[currentUser] = user;
                cout << "이메일 수정 완료.\n";
            }
            break;
        case 3:
            cout << "새 비밀번호: ";
            cin >> input;
            user.password = input;
            users[currentUser] = user;
            cout << "비밀번호 수정 완료.\n";
            break;
        default:
            cout << "잘못된 선택.\n";
    }
}

void deleteUser() {
    // 로그인 필수
    if (currentUser.empty()) {
        cout << "로그인 후 이용하세요.\n";
        return;
    }

    // 확인 후 users[currentUser] 삭제, 로그아웃 처리
    cout << "정말 탈퇴하시겠습니까? (y/n): ";
    char yn;
    cin >> yn;
    if (yn == 'y' || yn == 'Y') {
        users.erase(currentUser);
        currentUser = "";
        cout << "탈퇴 완료.\n";
    }
}

void userMenu() {
    while (true) {
        cout << "\n[사용자 메뉴]\n";
        cout << "1. 회원가입\n2. 로그인\n3. 정보 조회\n4. 정보 수정\n5. 탈퇴\n0. 뒤로가기\n> ";
        int cmd;
        cin >> cmd;
        if (cmd == 0) return;
        switch (cmd) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: showUserInfo(); break;
            case 4: updateUser(); break;
            case 5: deleteUser(); break;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

// ---------------- 포스트 기능 ----------------
void createPost() {
    // 로그인 사용자만 가능
    if (currentUser.empty()) {
        cout << "로그인 후 작성 가능합니다.\n";
        return;
    }

    // 제목과 내용 입력받아 postList에 Post 구조체 저장
    cin.ignore();
    string title, content;
    cout << "[새 글 작성]\n제목: ";
    getline(cin, title);
    cout << "내용(선택): ";
    getline(cin, content);
    Post post = { currentUser, title, content, vector<string>() };
    postList.push_back(post);
    cout << "등록 완료.\n";
}

void viewPosts() {
    if (postList.empty()) {
        cout << "게시글이 없습니다.\n";
        return;
    }

    // 전체 게시글 목록 번호 출력
    cout << "[게시글 목록]\n";
    for (size_t i = 0; i < postList.size(); ++i) {
        cout << i + 1 << ". " << postList[i].title << " (by " << postList[i].author << ")\n";
    }

    // 선택한 글의 제목/내용/댓글 출력
    cout << "번호 선택 (0: 뒤로가기): ";
    int sel;
    cin >> sel;
    if (sel <= 0 || sel > (int)postList.size()) return;

    Post& post = postList[sel - 1];
    cout << "\n[" << post.title << "] by " << post.author << "\n";
    cout << post.content << "\n";

    // 로그인 시 댓글 작성 가능(post.comments.push_back())
    if (!post.comments.empty()) {
        cout << "[댓글]\n";
        for (size_t i = 0; i < post.comments.size(); ++i)
            cout << "- " << post.comments[i] << "\n";
    }

    cout << "1. 댓글 작성\n0. 뒤로가기\n> ";
    int action;
    cin >> action;
    if (action == 1) {
        if (currentUser.empty()) {
            cout << "로그인 후 댓글 작성 가능.\n";
            return;
        }
        cin.ignore();
        string comment;
        cout << "댓글 입력: ";
        getline(cin, comment);
        post.comments.push_back(currentUser + ": " + comment);
        cout << "댓글 등록 완료.\n";
    }
}

void postMenu() {
    while (true) {
        cout << "\n[포스트 메뉴]\n1. 게시글 작성\n2. 게시글 조회\n0. 뒤로가기\n> ";
        int cmd;
        cin >> cmd;
        if (cmd == 0) return;
        switch (cmd) {
            case 1: createPost(); break;
            case 2: viewPosts(); break;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

// ---------------- 소셜 기능 ----------------
void searchUser() {
    // 아이디를 입력받고 해당 사용자가 존재하는지 확인
    cout << "[사용자 검색] 아이디 입력: ";
    string target;
    cin >> target;
    if (users.count(target))
        cout << "검색 결과: " << target << " (회원)\n";
    else
        cout << "존재하지 않는 사용자입니다.\n";
}

void requestFollow() {
    // 상대방 존재+자기 자신이 아닌지 체크
    // followRequests[상대id]에 currentUser를 추가, 수신자 기준으로 저장됨
    cout << "[팔로우 요청] 대상 아이디 입력: ";
    string target;
    cin >> target;
    if (users.count(target) && target != currentUser) {
        followRequests[target].insert(currentUser);
        cout << "팔로우 요청 완료.\n";
    } else {
        cout << "잘못된 사용자입니다.\n";
    }
}

void viewFollowList() {
    // 내가 팔로우한 사용자 목록 출력
    cout << "[내 팔로우 목록]\n";
    set<string>& list = follows[currentUser];
    for (set<string>::iterator it = list.begin(); it != list.end(); ++it)
        cout << "- " << *it << "\n";
}

void viewFollowRequests() {
    // 내가 받은 팔로우 요청 목록 출력(followRequests[currentUser]기준)
    cout << "[받은 팔로우 요청 목록]\n";
    set<string>& list = followRequests[currentUser];
    for (set<string>::iterator it = list.begin(); it != list.end(); ++it)
        cout << "- " << *it << "\n";
}

void handleFollowRequest() {
    // 요청자 id 입력 후 수락 여부 입력
        // 수락 시 follows[currentsUser].insert(requester)
        // 이후 followRequests에서 제거
    // 수락 시 반대 방향(상대가 나를 팔로우)이 기록됨
    cout << "[팔로우 수락/거절] 요청자 아이디 입력: ";
    string requester;
    cin >> requester;
    set<string>& reqs = followRequests[currentUser];
    if (reqs.count(requester)) {
        cout << "수락하시겠습니까? (y/n): ";
        char yn;
        cin >> yn;
        if (yn == 'y' || yn == 'Y') {
            follows[currentUser].insert(requester);
            cout << "팔로우 수락 완료.\n";
        } else {
            cout << "거절되었습니다.\n";
        }
        reqs.erase(requester);
    } else {
        cout << "요청 목록에 없습니다.\n";
    }
}

void socialMenu() {
    while (true) {
        cout << "\n[소셜 메뉴]\n";
        cout << "1. 사용자 검색\n2. 팔로우 요청\n3. 팔로우 목록\n";
        cout << "4. 받은 요청 목록\n5. 요청 수락/거절\n0. 뒤로가기\n> ";
        int cmd;
        cin >> cmd;
        if (cmd == 0) return;
        switch (cmd) {
            case 1: searchUser(); break;
            case 2: requestFollow(); break;
            case 3: viewFollowList(); break;
            case 4: viewFollowRequests(); break;
            case 5: handleFollowRequest(); break;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

// ---------------- DM 기능 ----------------
void sendDM() {
    // 팔로우한 사용자에게만 전송 가능
    // 메시지는 dmMessages[{보낸이, 받는이}] 형태로 저장
    cout << "[DM 전송] 수신자 아이디 입력: ";
    string to;
    cin >> to;
    if (!follows[currentUser].count(to)) {
        cout << "팔로우한 사용자에게만 DM 가능.\n";
        return;
    }
    cin.ignore();
    string msg;
    cout << "메시지 입력: ";
    getline(cin, msg);
    dmMessages[make_pair(currentUser, to)].push_back(msg);
    cout << "전송 완료.\n";
}

void viewDM() {
    // dmMessages에서 현재 사용자 관련된 대화 상대 목록 출력
    // 입력받은 상대와의 메시지 출력(양방향 모두 확인)
    cout << "[DM 목록]\n";
    for (map<pair<string, string>, vector<string>>::iterator it = dmMessages.begin(); it != dmMessages.end(); ++it) {
        if (it->first.first == currentUser || it->first.second == currentUser) {
            cout << "- 상대: " << (it->first.first == currentUser ? it->first.second : it->first.first) << "\n";
        }
    }
    cout << "상대 아이디 입력: ";
    string target;
    cin >> target;
    pair<string, string> key1 = make_pair(currentUser, target);
    pair<string, string> key2 = make_pair(target, currentUser);

    if (dmMessages.count(key1)) {
        for (size_t i = 0; i < dmMessages[key1].size(); ++i)
            cout << currentUser << ": " << dmMessages[key1][i] << "\n";
    }
    if (dmMessages.count(key2)) {
        for (size_t i = 0; i < dmMessages[key2].size(); ++i)
            cout << target << ": " << dmMessages[key2][i] << "\n";
    }
}

void deleteDM() {
    // 보낸 사람 기준 쌍의 메시지만 삭제(상대가 보낸 건 남아 있음)
    cout << "[DM 삭제] 대상 아이디 입력: ";
    string target;
    cin >> target;
    pair<string, string> key = make_pair(currentUser, target);
    if (dmMessages.count(key)) {
        cout << "정말 삭제할까요? (y/n): ";
        char yn;
        cin >> yn;
        if (yn == 'y' || yn == 'Y') {
            dmMessages.erase(key);
            cout << "삭제 완료.\n";
        }
    } else {
        cout << "해당 사용자와의 메시지가 없습니다.\n";
    }
}

void dmMenu() {
    while (true) {
        cout << "\n[DM 메뉴]\n1. DM 전송\n2. DM 조회\n3. DM 삭제\n0. 뒤로가기\n> ";
        int cmd;
        cin >> cmd;
        if (cmd == 0) return;
        switch (cmd) {
            case 1: sendDM(); break;
            case 2: viewDM(); break;
            case 3: deleteDM(); break;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

// ---------------- 메인 메뉴 ----------------
void mainMenu() {
    while (true) {
        cout << "\n[메인 메뉴]\n1. 사용자 기능\n2. 포스트 기능\n3. 소셜 기능\n4. 메시지 기능\n5. 종료\n> ";
        int cmd;
        cin >> cmd;
        switch (cmd) {
            case 1: userMenu(); break;
            case 2: postMenu(); break;
            case 3: socialMenu(); break;
            case 4: dmMenu(); break;
            case 5: cout << "프로그램을 종료합니다.\n"; return;
            default: cout << "잘못된 입력입니다.\n";
        }
    }
}

// ---------------- 실행 시작 ----------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    mainMenu();
    return 0;
}
