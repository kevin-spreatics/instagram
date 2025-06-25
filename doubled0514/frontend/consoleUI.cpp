
#include <iostream>
#include <string>
using namespace std;

bool isLoggedIn = false;
string currentUser;

void showLoginMenu();
void showUserMenu();
void showSocialMenu();
void showDMMenu();
void showProfileMenu();
void showMyProfileMenu(const std::string& userId);
void showOtherUserMenu(const std::string& targetUser);
void myinfo();
void mypost(const std::string& userId);
void myfollowing();
void sendingDM();
void sendingfollow();
void otherpost(const std::string& userId);
void showpost(const std::string& userId);

int main() {
    int choice;
    
    while (true) {
        cout << "\n[메인메뉴]" << endl;
        cout << "1. 로그인/회원가입" << endl;
        if (isLoggedIn) {
            cout << "2. 사용자 기능" << endl;
            cout << "3. 소셜" << endl;
            cout << "4. DM함" << endl;
        }
        cout << "0. 종료" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                showLoginMenu();
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

void showLoginMenu() {
    int choice;
    cout << "\n[로그인/회원가입]" << endl;
    cout << "1. 로그인" << endl;
    cout << "2. 회원가입" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "아이디 입력: ";
            cin >> currentUser;
            isLoggedIn = true;
            cout << currentUser << "님 로그인 완료." << endl;
            break;
        case 2:
            cout << "회원가입" << endl;
            break;
        case 0:
            return;
        default:
            cout << "잘못된 입력입니다." << endl;
    }
}

void showUserMenu() {
    string targetUser;
    cout << "\n조회할 사용자 ID 입력: ";
    cin >> targetUser;

    if (targetUser == currentUser) {
        showMyProfileMenu(currentUser);
    } else {
        showOtherUserMenu(targetUser);
    }
}

void showMyProfileMenu(const string& userid) {
    int choice;
    while (true) {
        cout << "\n[내 프로필]" << endl;
        cout << "[자동 조회된 유저 정보 자동 조회 (닉네임,이메일,나이)]" << endl;
        cout << "1. 내 정보 조회/수정/삭제" << endl;
        cout << "2. 내 포스트 조회/작성" << endl;
        cout << "3. 팔로워/팔로잉, 팔로우 신청 관리" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                myinfo();
                break;
            case 2:
                mypost(userid);
                break;
            case 3:
                myfollowing();
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}

void showOtherUserMenu(const string & targetUser) {
    int choice;
    while (true) {
        cout << "\n[" << targetUser << "의 프로필]" << endl;
        cout << "[자동 조회된 유저 정보 자동 조회 (닉네임,이메일,나이)]" << endl;
        cout << "1. DM 보내기" << endl;
        cout << "2. 팔로우 신청하기" << endl;
        cout << "3. 포스트 보기 및 댓글 작성" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
            case 1:
                sendingDM();
                break;
            case 2:
                sendingfollow();
                break;
            case 3:
                showpost(targetUser);
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
        }
    }
}


void myinfo(){
    int choice;
    while (true) {
        cout << "1. 사용자 정보 수정" << endl;
        cout << "2. 사용자 정보 삭제" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "사용자 정보 수정란" << endl;
                break;
            case 2:
                cout << "사용자 정보 삭제란" << endl;
                break;
            case 0:
                return;
            default:
                cout << "잘못된 입력입니다." << endl;
}}}

void mypost(const string & userid){
    int choice ;
    while (true){
    cout << "사용자의 모든 포스트 목록 자동 조회" <<endl;
    cout << "1.포스트 작성"<<endl;
    cout << "2.특정 포스트 조회" << endl;
    cout << "0.뒤로가기"<<endl;
    cout << "선택:";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "포스트 작성란" << endl;
            break;
        case 2:
            showpost(userid) ;
            break;
        case 0:
            cout << "뒤로가기" << endl;
             return;
        default:
            cout << "잘못된 입력입니다." << endl;}

}}

void otherpost(const string & userid){
    int choice;
    while(true){
        cout << "//해당 사용자의 모든 포스트 목록 자동 조회//" << endl;
        cout << "1.특정 포스트 조회하기"  << endl;
        cout << "0. 뒤로가기"<< endl;
        cout << "선택 : ";
        cin >> choice;
        switch(choice){
            case 1: 
                showpost(userid);
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                return;
            default:
            cout << "잘못된 입력입니다." << endl;    
}}}

void showpost(const string& userid){
    int choice;
    while(true){
        cout << "//특정 포스트 조회 중//" << endl;
        cout << "//조회확인용 유저아이디 출력//" << userid << endl;
        cout << "1.코멘트 작성하기" << endl;
        cout << "0.뒤로가기" << endl;
        cout << "선택:" ;
        cin >> choice ; 
        switch(choice){
            case 1: 
                cout << "코멘트 작성" << endl;
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                return;
            default:
            cout << "잘못된 입력입니다." << endl;  

    }
}}


void myfollowing(){
    int choice;
    while(true){
        cout << "1.팔로워/팔로잉 조회" <<endl;
        cout << "2.팔로우 신청 조회" << endl;
        cout << "0.뒤로가기" << endl;
        cout << "선택: ";
        cin >> choice ;
        switch(choice){
            case 1: 
                cout << "팔로워/팔로잉 목록 조회" << endl;
                break;
            case 2:
                cout << "팔로우 신청 조회" <<endl;
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                break;
            default:
            cout << "잘못된 입력입니다." << endl;
        }
    }
}
void sendingDM(){
    int choice;
    while(true){     
        cout << "DM 보내기 " << endl;
        break;
}}
void sendingfollow(){
    int choice;
    while(true){
        cout << "1.팔로잉/팔로우 조회하기" << endl;
        cout << "2.팔로우 신청하기" << endl;
        cout << "0. 뒤로가기"<< endl;
        cout << "선택 : ";
        cin >> choice;
        switch(choice){
            case 1: 
                cout << "팔로워/팔로잉 목록 조회" << endl;
                break;
            case 2:
                cout << "팔로우 신청 하기" <<endl;
                break;
            case 0 :
                cout <<"뒤로가기"<< endl;
                return;
            default:
            cout << "잘못된 입력입니다." << endl;
    }
}}

void showSocialMenu() {
    cout << "[소셜 기능은 아직 구현되지 않았습니다.]" << endl;
}

void showDMMenu() {
    cout << "[DM 기능은 아직 구현되지 않았습니다.]" << endl;
}