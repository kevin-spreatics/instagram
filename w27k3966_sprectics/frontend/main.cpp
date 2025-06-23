#include <iostream>
#include <string>
using namespace std;

void m_1_login() {
    string id, password;

    cout << "로그인" << endl;
    cout << "id : " <<endl;
    cin >> id;

    cout << "password : " <<endl;
    cin >> password;

    if (id == "sohee" && password == "1234") {
        cout << "== 로그인 성공 ==" << endl;
    } else {
        cout <<"== 로그인 실패. 아이디 또는 비밀번호를 확인하세요. ==" <<endl;
    }
}

void m_1_signup() {
    string nickname, id, password, email;

    cout << "회원가입" << endl;

    cout << "nickname : " <<endl;
    cin >> nickname;

    cout << "id : " <<endl;
    cin >> id;

    cout << "password : " <<endl;
    cin >> password;

    cout << "password 확인: " <<endl;
    cin >> password;

    cout << " email : " << endl;
    cin >> email;


    cout << "== 회원가입 완료 ==" <<endl;

}

void m_1_userLogin() {
    while (1) {
        cout<< "[1. 로그인]" << endl;
        cout<<"1. 로그인" <<endl;
        cout<<"2. 회원가입"<< endl;
        cout << "3. 뒤로가기" << endl;


        int input;
        cin >>input;
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
