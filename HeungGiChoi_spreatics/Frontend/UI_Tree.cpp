#include <iostream>

using namespace std;

void func_1(){
    while(1){
        cout << " 로그인 / 회원가입 화면" << endl;
        cout << "(1) 로그인" << endl;
        cout << "(2) 회원가입(사용자 생성)" << endl;
        cout << "(3) 돌아가기" << endl;

        int input;
        cin >> input;

        switch(input) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }

    }
}

void func_2(){
    while(1){
        cout << "사용자 정보 func" << endl;
        cout << "(1) 사용자 정보 조회" << endl;
        cout << "(2) 사용자 정보 수정" << endl;
        cout << "(3) 사용자 정보 삭제" << endl;
        cout << "(4) 돌아가기" << endl;
        
        int input;
        cin >> input;

        switch(input){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }
    }
}

void func_3_x(){
    while(1){
        cout << "올라온 포스트 조회하기 하위 기능" << endl;
        cout << "(1) 포스트 조회하기" << endl;
        cout << "(2) 포스트에 커맨트 달기" << endl;
        cout << "(3) 포스트에 달린 커맨트 조회하기" << endl;
        cout << "(4) 돌아가기" << endl;
        int input;
        cin >> input;

        switch(input){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }
    }
}

void func_3(){
        while(1){
        cout << "포스팅 func" << endl;
        cout << "(1) 포스트 올리기" << endl;
        cout << "(2) 올라온 포스트 조회하기" << endl;
        cout << "(3) 돌아가기" << endl;
        
        int input;
        cin >> input;

        switch(input){
            case 1:
                break;
            case 2:
                func_3_x();
                break;
            case 3:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }
    }
}

void main_menu(){
    int input = 0;

    while(1){
        cout << "[Instagram 메인메뉴]" << endl;
        cout << "메뉴를 선택하세요." << endl;
        cout << "(1) 로그인 / 회원가입" << endl;
        cout << "(2) 사용자 정보" << endl;
        cout << "(3) 포스팅" << endl;
        cout << "(4) 프로그램 종료" << endl;

        cin >> input;

        switch(input){
            case 1:
                func_1();
                break;
            case 2:
                func_2();
                break;
            case 3:
                func_3();
                break;
            case 4:
                return;
                break;
            default:
                cout << "잘못 입력하셨습니다. 다시 입력해주세요." << endl;
                break;
        }
    }
}

int main() {
    main_menu();

    return 0;
}