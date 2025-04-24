// banksystem.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define BANK_MAXLENGTH 30
#define STRING_MAXLENGTH 20

typedef struct
{
	int id;
	char name[STRING_MAXLENGTH];
	int balance;
}Bank;

int main(void)
{
	int num = 0, i = 0, j = 0;
	Bank bank[BANK_MAXLENGTH];			// Bank형 변수를 포인터 선언으로 배열 선언하면 각각의 배열에 접근하여 변경할 수 없으므로 일단 Bank형 구조체 배열을 많이 잡아놈
	while (num != 5)
	{
		printf("-----menu-----\n");
		printf("1. 계좌 개설\n");
		printf("2. 입금\n");
		printf("3. 출금\n");
		printf("4. 계좌정보 전체 출력\n");
		printf("5. 프로그램 종료\n");
		printf("선택 :");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
		{
			if (i >= BANK_MAXLENGTH)
			{
				printf("bank service is closed (user is full)\n");
				break;
			}
			printf("account id :");
			scanf("%d", &((bank + i)->id));

			bool b_IsIdExist = false;
			for (int t = 0; t < i; t++)
			{
				if (bank[t].id == bank[i].name)
				{
					b_IsIdExist = true;
					printf("ID is already exist\n");
				}
			}
			if (b_IsIdExist)
				break;

			printf("name :");
			scanf("%s", bank[i].name);

			bank[i].balance = -1;
			while (bank[i].balance < 0)
			{
				printf("account input :");
				scanf("%d", &bank[i].balance);
				if (bank[i].balance < 0)
					printf("can't input negativie number\n");
			}
			i++;
			break;
		}
		case 2:
		{
			int money = -1;			// 반복문 안의 지역변수
			int ID = 0;
			printf("account your id :");
			scanf("%d", &ID);

			// find ID
			for (j = 0; j < i; j++)
			{
				if (bank[j].id == ID)
				{
					while (money < 0)
					{
						printf("account input :");
						scanf("%d", &money);
						if (money < 0)
							printf("can't input negativie number\n");
					}
					bank[j].balance += money;
					break;
				}
			}
			if (j == i)
				printf("%d: not match any\n", ID);
			break;
		}
		case 3:
		{
			int money = -1;			// 반복문 안의 지역변수
			int ID = 0;
			printf("account your id :");
			scanf("%d", &ID);
			for (j = 0; j < i; j++)
			{
				if (bank[j].id == ID)
				{
					while (money < 0)
					{
						printf("account input :");
						scanf("%d", &money);
						if (money < 0)
							printf("can't input negativie number\n");
					}
					bank[j].balance -= money;
					break;
				}
			}
			if (j == i)
				printf("%d: not match any\n", ID);
			break;

		}
		case 4:
		{
			int ID = 0;
			printf("input ID :");
			scanf("%d", &ID);
			for (j = 0; j < i; j++)
			{
				if (bank[j].id == ID)
				{
					printf("id :%d\n", bank[j].id);
					printf("name :%s\n", bank[j].name);
					printf("balance :%d\n", bank[j].balance);
					break;
				}
			}
			if (j == i)
				printf("%d: not match any\n", ID);
			break;
		}
		default:
			break;
		}
	}
	


}