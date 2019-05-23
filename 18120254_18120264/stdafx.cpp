﻿#include "stdafx.h"

// di chuyển đến vị trí (x,y)
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
// kiểm tra ngày hợp lệ
int CheckDate(DATES BirthDate)
{
	int MaxDay; // ngày trong tháng
	if (BirthDate.Year <= LIMIT_YEAR || BirthDate.Year >= THIS_YEAR || BirthDate.Month < 0 || BirthDate.Month > 12 || BirthDate.Day < 0 || BirthDate.Day > 31)
	{
		return 1;
	}
	else
	{
		switch (BirthDate.Month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			MaxDay = 31;
			break;
		case 2:
			if (BirthDate.Year % 4 == 0 && BirthDate.Year % 100 != 0)
				MaxDay = 29;
			else
				MaxDay = 28;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			MaxDay = 30;
			break;
		}
		if (BirthDate.Day <= MaxDay)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
// xuất thông tin 1 tài khoản
void OutputInfo(ACCOUNT login)
{
	printf("\n\t\tUsername: %s", login.Username);
	printf("\n\n\t\tHo va Ten: %s", login.FullName);
	printf("\n\t\tCMND: %s", login.CMND);
	printf("\n\t\tDia chi: %s", login.Address);
	if (login.Genre == 1)
		printf("\n\t\tGioi tinh: Nam\n");
	else
		printf("\n\t\tGioi tinh: Nu\n");
	printf("\n\t\tNgay Sinh: %d/%d/%d     ", login.BirthDate.Day, login.BirthDate.Month, login.BirthDate.Year);
}
// thay đổi mật khẩu
void RePass(ACCOUNT &login)
{
	ACCOUNT checkpass;
	do {
		// nhập pass mới
		gotoxy(20, 6); printf("Password moi: ");
		// mã hóa mật khẩu
		gotoxy(34, 6);
		int p = 0;
		do {
			login.Password[p] = _getch();
			if (login.Password[p] != '\r')
			{
				printf("*");
			}
			p++;
		} while (login.Password[p - 1] != '\r');
		login.Password[p - 1] = '\0';
		// nhập lại pass
		gotoxy(20, 8); printf("Nhap lai password: ");
		// mã hóa mật khẩu
		gotoxy(39, 8);
		p = 0;
		do {
			checkpass.Password[p] = _getch();
			if (checkpass.Password[p] != '\r')
			{
				printf("*");
			}
			p++;
		} while (checkpass.Password[p - 1] != '\r');
		checkpass.Password[p - 1] = '\0';
		if (strcmp(login.Password, checkpass.Password) != 0)
		{
			gotoxy(20, 10); printf("Khong trung khop. Vui long nhap lai.  ");
			gotoxy(30, 6); printf("                               ");
			gotoxy(39, 8); printf("                               ");
		}
	} while (strcmp(login.Password, checkpass.Password) != 0);
	FILE *fAcc = fopen("Account.bin", "rb+");
	if (fAcc == NULL)
	{
		printf("Khong mo duoc file");
	}
	rewind(fAcc);
	ACCOUNT temp;
	while (fread(&temp, sizeof(ACCOUNT), 1, fAcc))
	{
		if (strcmp(login.Username, temp.Username) == 0)
		{
			// đi đến vị trí info để ghi
			fseek(fAcc, ftell(fAcc) - sizeof(ACCOUNT), 0);
			fwrite(&login, sizeof(ACCOUNT), 1, fAcc);
			fclose(fAcc);	 // quan trọng: không được để ngoài vòng if
			gotoxy(20, 10); printf("Cap nhat password thanh cong.  ");
			system("pause");
		}
	}
}
// cập nhật thông tin cá nhân 
void UpdateInfo(ACCOUNT &login)
{
	gotoxy(10, 4); printf("::::::::::::::::::::Cap nhat thong tin ca nhan::::::::::::::::::::");
	FILE *fAcc = fopen("Account.bin", "rb+");
	if (fAcc == NULL)
	{
		printf("Khong mo duoc file");
	}
	rewind(fAcc);
	ACCOUNT temp;
	while (fread(&temp, sizeof(ACCOUNT), 1, fAcc))
	{
		if (strcmp(login.Username, temp.Username) == 0)
		{
			// nhập info mới
			while (getchar() != '\n');
			flushall();
			gotoxy(20, 6); printf("Username moi: ");
			fgets(login.Username, sizeof(login.Username), stdin);
			strtok(login.Username, "\n"); // loại bỏ \n do fgets tự thêm
			gotoxy(20, 8); printf("Ho Va Ten moi: ");
			fgets(login.FullName, sizeof(login.FullName), stdin);
			gotoxy(20, 10); printf("CMND moi: ");
			fgets(login.CMND, sizeof(login.CMND), stdin);
			gotoxy(20, 12); printf("Dia chi moi: ");
			fgets(login.Address, sizeof(login.Address), stdin);
			do {
				gotoxy(20, 14); printf("Gioi tinh moi (1 la Nam, 2 la Nu): ");
				scanf("%d", &login.Genre);
				if (login.Genre != 1 || login.Genre != 2)
				{
					gotoxy(20, 16); printf("Vui long chi nhap so 1 hoac 2!");
				}
			} while (login.Genre != 1 && login.Genre != 2);
			int check = 0;
			do {
				gotoxy(20, 16); printf("                              ");
				gotoxy(20, 16); printf("Ngay sinh moi: ");
				scanf("%d", &login.BirthDate.Day);
				gotoxy(20, 18); printf("Thang sinh moi: ");
				scanf("%d", &login.BirthDate.Month);
				gotoxy(20, 20); printf("Nam sinh moi: ");
				scanf("%d", &login.BirthDate.Year);
				check = CheckDate(login.BirthDate);
				if (check != 0)
				{
					gotoxy(20, 22); printf("Ngay thang khong hop le. Vui long nhap lai\n");
				}
			} while (check != 0);
			// đi đến vị trí info để ghi
			fseek(fAcc, ftell(fAcc) - sizeof(ACCOUNT), 0);
			fwrite(&login, sizeof(ACCOUNT), 1, fAcc);
			fclose(fAcc);	 // quan trọng: không được để ngoài vòng if
		}
	}
}
// nhập thông tin 1 tài khoản
void InputInfo(ACCOUNT &acc)
{
	while (getchar() != '\n');
	flushall();
	gotoxy(20, 6); printf("Username: ");
	fgets(acc.Username, sizeof(acc.Username), stdin);
	strtok(acc.Username, "\n"); // loại bỏ \n do fgets tự thêm
	ACCOUNT checkpass;
	do {
		// nhập pass 
		gotoxy(20, 8); printf("Password: ");
		// mã hóa mật khẩu
		gotoxy(30, 8);
		int p = 0;
		do {
			acc.Password[p] = _getch();
			if (acc.Password[p] != '\r')
			{
				printf("*");
			}
			p++;
		} while (acc.Password[p - 1] != '\r');
		acc.Password[p - 1] = '\0';
		// nhập lại pass
		gotoxy(20, 10); printf("Nhap lai password: ");
		// mã hóa mật khẩu
		gotoxy(39, 10);
		p = 0;
		do {
			checkpass.Password[p] = _getch();
			if (checkpass.Password[p] != '\r')
			{
				printf("*");
			}
			p++;
		} while (checkpass.Password[p - 1] != '\r');
		checkpass.Password[p - 1] = '\0';
		if (strcmp(acc.Password, checkpass.Password) != 0)
		{
			gotoxy(20, 12); printf("Khong trung khop. Vui long nhap lai.  ");
			gotoxy(30, 8); printf("                               ");
			gotoxy(39, 10); printf("                               ");
		}
	} while (strcmp(acc.Password, checkpass.Password) != 0);
	gotoxy(20, 12); printf("                                      ");
	gotoxy(20, 12); printf("Ho Va Ten: ");
	fgets(acc.FullName, sizeof(acc.FullName), stdin);
	gotoxy(20, 14); printf("CMND: ");
	fgets(acc.CMND, sizeof(acc.CMND), stdin);
	gotoxy(20, 16); printf("Dia chi: ");
	fgets(acc.Address, sizeof(acc.Address), stdin);
	do {
		gotoxy(20, 18); printf("Gioi tinh (1 la Nam, 2 la Nu): ");
		scanf("%d", &acc.Genre);
		if (acc.Genre != 1 || acc.Genre != 2)
		{
			gotoxy(20, 20); printf("Vui long chi nhap so 1 hoac 2!");
		}
	} while (acc.Genre != 1 && acc.Genre != 2);
	int check = 0;
	do {
		gotoxy(20, 20); printf("                              ");
		gotoxy(20, 20); printf("Ngay sinh: ");
		scanf("%d", &acc.BirthDate.Day);
		gotoxy(20, 22); printf("Thang sinh: ");
		scanf("%d", &acc.BirthDate.Month);
		gotoxy(20, 24); printf("Nam sinh: ");
		scanf("%d", &acc.BirthDate.Year);
		check = CheckDate(acc.BirthDate);
		if (check != 0)
		{
			gotoxy(20, 26); printf("Ngay thang khong hop le. Vui long nhap lai\n");
		}
	} while (check != 0);
}
//kiểm tra tài khoản đã tồn tại, return true neu co ton tai
int ExistAcc(FILE *f, ACCOUNT acc)
{
	rewind(f);
	ACCOUNT temp;
	while (fread(&temp, sizeof(ACCOUNT), 1, f))
	{
		if (strcmp(temp.Username, acc.Username) == 0)
			return true;
	}
	return false;
}
// thêm tài khoản
void CreateAcc()
{
	FILE *fAcc = fopen("Account.bin", "ab+");
	if (fAcc == NULL)
	{
		printf("Khong mo duoc file");
	}
	ACCOUNT acc;
	int choice;
	do {
		gotoxy(20, 6); printf("Chon loai tai khoan");
		gotoxy(23, 8); printf("1. Chuyen Vien");
		gotoxy(23, 10); printf("2. Quan Ly");
		gotoxy(20, 12); printf("Lua chon cua ban: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
		{
			bool check = false;
			do {
				system("cls");
				gotoxy(10, 4); printf("::::::::::::::::::::Tao tai khoan Chuyen Vien::::::::::::::::::::");
				InputInfo(acc);
				acc.Permission = 2;
				acc.Status = 2; // blocked
				if (ExistAcc(fAcc, acc) == false)
				{
					check = true;
					fseek(fAcc, 0, SEEK_END);
					fwrite(&acc, sizeof(ACCOUNT), 1, fAcc);
					gotoxy(16, 26); printf("Them tai khoan thanh cong.    ");
					fclose(fAcc);
				}
				else
				{
					gotoxy(16, 26); printf("Username tai khoan da ton tai.");
					int choice;
					do {
						gotoxy(16, 28); printf("Thoat ra menu?");
						gotoxy(20, 30); printf("1. Co");
						gotoxy(20, 32); printf("2. Khong, nhap lai.");
						gotoxy(16, 34); printf("Lua chon cua ban: ");
						scanf("%d", &choice);
						if (choice != 1 && choice != 2)
						{
							gotoxy(16, 36); printf("Vui long chi nhap 1 hoac 2");
						}
					} while (choice != 1 && choice != 2);
					if (choice == 1)
						break;
					system("cls");
				}
			} while (check == false);
			system("pause");
			break;
		}
		case 2:
		{
			bool check = false;
			do {
				system("cls");
				gotoxy(10, 4); printf("::::::::::::::::::::Tao tai khoan Quan Ly::::::::::::::::::::");
				InputInfo(acc);
				acc.Permission = 3;
				acc.Status = 2; // blocked
				if (ExistAcc(fAcc, acc) == false)
				{
					check = true;
					fseek(fAcc, 0, SEEK_END);
					fwrite(&acc, sizeof(ACCOUNT), 1, fAcc);
					gotoxy(16, 26); printf("Them tai khoan thanh cong.    ");
					fclose(fAcc);
				}
				else
				{
					gotoxy(16, 26); printf("Username tai khoan da ton tai.");
					int choice;
					do {
						gotoxy(16, 28); printf("Thoat ra menu?");
						gotoxy(20, 30); printf("1. Co");
						gotoxy(20, 32); printf("2. Khong, nhap lai.");
						gotoxy(16, 34); printf("Lua chon cua ban: ");
						scanf("%d", &choice);
						if (choice != 1 && choice != 2)
						{
							gotoxy(16, 36); printf("Vui long chi nhap 1 hoac 2");
						}
					} while (choice != 1 && choice != 2);
					if (choice == 1)
						break;
					system("cls");
				}
			} while (check == false);
			system("pause");
			break;
		}
		default:
		{
			gotoxy(20, 14); printf("\aLua chon khong hop le. Vui long chon lai!      ");
			system("pause");
		}
		}
	} while (choice != 1 && choice != 2);
}
// kiểm tra tài khoản đăng nhập  
void CheckLogin(FILE *f, char username[MAX_PASS], char pass[MAX_USERNAME], ACCOUNT &login, int &checkacc)
{
	// checkacc = 1: admin, 2: chuyên viên, 3: quản lý, 4: chưa activate
	rewind(f);
	ACCOUNT temp;
	while (fread(&temp, sizeof(ACCOUNT), 1, f))
	{
		if (strcmp(username, temp.Username) == 0 && strcmp(pass, temp.Password) == 0 && temp.Permission == 1)
		{
			checkacc = temp.Permission;
			login = temp;
		}
		else if (strcmp(username, temp.Username) == 0 && strcmp(pass, temp.Password) == 0 && temp.Permission == 2)
		{
			if (temp.Status == 1)
			{
				checkacc = temp.Permission;
				login = temp;
			}
			else
			{
				checkacc = 4;
			}
		}
		else if (strcmp(username, temp.Username) == 0 && strcmp(pass, temp.Password) == 0 && temp.Permission == 3)
		{
			if (temp.Status == 1)
			{
				checkacc = temp.Permission;
				login = temp;
			}
			else
			{
				checkacc = 4;
			}
		}
	}
}
//menu đăng nhập
void menu()
{
	int luachon = 2; // nếu người dùng muốn quay lại: 1 = có, 2 = ko
	char d[26] = "He Thong Quan Ly Thu Vien";
	system("cls");
	// in chuyển động 
	gotoxy(10, 4);
	for (int j = 0; j < 20; j++)
	{
		Sleep(50);
		printf(":");
	}
	for (int j = 0; j < 25; j++)
	{
		Sleep(50);
		printf("%c", d[j]);
	}
	for (int j = 0; j < 20; j++)
	{
		Sleep(50);
		printf(":");
	}
	do {
		gotoxy(20, 7); printf("\t1.Dang nhap");
		gotoxy(20, 9); printf("\t2.Thoat chuong trinh");
		gotoxy(20, 11); printf("Lua chon cua ban:");
		switch (_getch())
		{
		case '1':
			Login(luachon);
			break;
		case '2':
		{
			system("cls");
			gotoxy(20, 11); printf("Thoat trong 3 giay nua");
			for (int j = 0; j < 60; j++)
			{
				Sleep(50);
				printf(".");
			}
			system("cls");
			exit(0);
		}
		default:
		{
			gotoxy(20, 13); printf("\aLua chon khong hop le. Vui long chon lai!     ");
			if (_getch())
			{
				menu();
			}
		}
		}
	} while (luachon == 1);
}
// đăng nhập
void Login(int &luachon)
{
	system("cls");
	// chứa thông tin tài khoản đang login
	ACCOUNT login;
	// user nhập info
	char pass[MAX_PASS];
	char username[MAX_USERNAME];
	gotoxy(20, 4); printf("Username: ");
	scanf("%s", &username);
	gotoxy(20, 6); printf("Password: ");
	// mã hóa mật khẩu
	gotoxy(30, 6);
	int p = 0;
	do {
		pass[p] = _getch();
		if (pass[p] != '\r')
		{
			printf("*");
		}
		p++;
	} while (pass[p - 1] != '\r');
	pass[p - 1] = '\0';
	// mở file tài khoản
	FILE *fAcc = fopen("Account.bin", "rb");
	if (fAcc == NULL)
	{
		printf("Khong mo duoc tap tin");
	}
	// kiểm tra quyền
	int checkacc;
	CheckLogin(fAcc, username, pass, login, checkacc);
	fclose(fAcc);
	if (checkacc == 1)
	{
		gotoxy(20, 9); printf("Ban da dang nhap duoi quyen Admin.\n");
		_getch();
		MenuAdmin(login);
	}
	else if (checkacc == 2)
	{
		gotoxy(20, 9); printf("Ban da dang nhap duoi quyen Chuyen Vien.\n");
		_getch();
		MenuChuyenVien(login);
	}
	else if (checkacc == 3)
	{
		gotoxy(20, 9); printf("Ban da dang nhap duoi quyen Quan Ly.\n");
		_getch();
		MenuQuanLy(login);
	}
	else if (checkacc == 4)
	{
		gotoxy(20, 9); printf("Tai khoan dang bi khoa!  ");
		system("pause"); system("cls");
		gotoxy(20, 9); printf("Lua chon hanh dong.");
		gotoxy(27, 11); printf("1. Quay lai menu chinh");
		gotoxy(27, 13); printf("2. Dang nhap lai");
		gotoxy(20, 15); printf("Lua chon cua ban: ");
		scanf("%d", &luachon);
		if (luachon == 1)
		{
			system("cls");
			// in lại nhưng không có hiệu ứng
			char d[26] = "He Thong Quan Ly Thu Vien";
			gotoxy(10, 4);
			for (int j = 0; j < 20; j++)
			{
				printf(":");
			}
			for (int j = 0; j < 25; j++)
			{
				printf("%c", d[j]);
			}
			for (int j = 0; j < 20; j++)
			{
				printf(":");
			}
		}
		else
		{
			Login(luachon);
		}
	}
	else
	{
		gotoxy(20, 8); printf("\aDang nhap khong thanh cong!  ");
		system("pause"); system("cls");
		gotoxy(20, 9); printf("Quay lai menu chinh?");
		gotoxy(27, 11); printf("1. Co");
		gotoxy(27, 13); printf("2. Khong. Dang nhap lai");
		gotoxy(20, 15); printf("Lua chon cua ban: ");
		scanf("%d", &luachon);
		if (luachon == 1)
		{
			system("cls");
			// in lại nhưng không có hiệu ứng
			char d[26] = "He Thong Quan Ly Thu Vien";
			gotoxy(10, 4);
			for (int j = 0; j < 20; j++)
			{
				printf(":");
			}
			for (int j = 0; j < 25; j++)
			{
				printf("%c", d[j]);
			}
			for (int j = 0; j < 20; j++)
			{
				printf(":");
			}
		}
		else
		{
			Login(luachon);
		}
	}
}
// phân quyền tài khoản
void PhanQuyen()
{
	FILE *f = fopen("Account.bin", "rb+");
	if (f == NULL)
	{
		printf("Khong mo duoc file");
	}
	rewind(f);
	gotoxy(10, 4); printf("::::::::::::::::::::Phan quyen Tai khoan::::::::::::::::::::");
	char usertemp[MAX_USERNAME];
	gotoxy(20, 6); printf("Nhap username: ");
	scanf("%s", &usertemp);
	ACCOUNT temp;
	while (fread(&temp, sizeof(ACCOUNT), 1, f))
	{
		if (strcmp(usertemp, temp.Username) == 0)
		{
			do {
				gotoxy(20, 8); printf("Loai tai khoan");
				gotoxy(23, 10); printf("2. Chuyen Vien");
				gotoxy(23, 12); printf("3. Quan Ly");
				gotoxy(20, 14); printf("Lua chon cua ban: ");
				scanf("%d", &temp.Permission);
				if (temp.Permission != 2 && temp.Permission != 3)
				{
					gotoxy(20, 16); printf("Vui long chi nhap 2 hoac 3");
				}
			} while (temp.Permission != 2 && temp.Permission != 3);
			do {
				gotoxy(20, 16); printf("Tinh trang tai khoan      ");
				gotoxy(23, 18); printf("1. Kich hoat");
				gotoxy(23, 20); printf("2. Khoa");
				gotoxy(20, 22); printf("Lua chon cua ban: ");
				scanf("%d", &temp.Status);
				gotoxy(20, 24); printf("                          ");
				if (temp.Status != 1 && temp.Status != 2)
				{
					gotoxy(20, 24); printf("Vui long chi nhap 1 hoac 2");
				}
			} while (temp.Status != 1 && temp.Status != 2);
			fseek(f, ftell(f) - sizeof(ACCOUNT), 0);
			fwrite(&temp, sizeof(ACCOUNT), 1, f);
			fclose(f);
		}
		else
		{
			gotoxy(20, 8); printf("Khong tim thay tai khoan   ");
		}
	}
}


//menu admin
void MenuAdmin(ACCOUNT &login)
{
	int luachon;
	do {
		system("cls");
		gotoxy(20, 3); printf("\xB2\xB2\xB2\xB2\xB2\xB2 HE THONG QUAN LY THU VIEN - ADMIN \xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(20, 4); printf("\xB2"); gotoxy(66, 4); printf("\xB2");
		gotoxy(20, 5); printf("\xB2"); gotoxy(66, 5); printf("\xB2");
		gotoxy(20, 6); printf("\xB2"); gotoxy(66, 6); printf("\xB2");
		gotoxy(20, 7); printf("\xB2"); gotoxy(66, 7); printf("\xB2");
		gotoxy(20, 8); printf("\xB2"); gotoxy(66, 8); printf("\xB2");
		gotoxy(20, 9); printf("\xB2"); gotoxy(66, 9); printf("\xB2");
		gotoxy(20, 10); printf("\xB2"); gotoxy(66, 10); printf("\xB2");
		gotoxy(20, 11); printf("\xB2"); gotoxy(66, 11); printf("\xB2");
		gotoxy(20, 12); printf("\xB2"); gotoxy(66, 12); printf("\xB2");
		gotoxy(20, 13); printf("\xB2"); gotoxy(66, 13); printf("\xB2");
		gotoxy(20, 14); printf("\xB2"); gotoxy(66, 14); printf("\xB2");
		gotoxy(20, 15); printf("\xB2"); gotoxy(66, 15); printf("\xB2");
		gotoxy(20, 16); printf("\xB2"); gotoxy(66, 16); printf("\xB2");
		gotoxy(20, 17); printf("\xB2"); gotoxy(66, 17); printf("\xB2");
		gotoxy(20, 18); printf("\xB2"); gotoxy(66, 18); printf("\xB2");
		gotoxy(20, 19); printf("\xB2"); gotoxy(66, 19); printf("\xB2");
		gotoxy(20, 20); printf("\xB2"); gotoxy(66, 20); printf("\xB2");
		gotoxy(20, 21); printf("\xB2"); gotoxy(66, 21); printf("\xB2");
		gotoxy(20, 22); printf("\xB2"); gotoxy(66, 22); printf("\xB2");
		gotoxy(20, 23); printf("\xB2"); gotoxy(66, 23); printf("\xB2");
		gotoxy(20, 24); printf("\xB2"); gotoxy(66, 24); printf("\xB2");
		gotoxy(20, 25); printf("\xB2"); gotoxy(66, 25); printf("\xB2");
		gotoxy(20, 26); printf("\xB2"); gotoxy(66, 26); printf("\xB2");
		gotoxy(20, 27); printf("\xB2"); gotoxy(66, 23); printf("\xB2");
		gotoxy(20, 27); printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(21, 5); printf("\t 1. Dang xuat");
		gotoxy(21, 7); printf("\t 2. Thay doi mat khau");
		gotoxy(21, 9); printf("\t 3. Cap nhat thong tin ca nhan");
		gotoxy(21, 11); printf("\t 4. Tao tai khoan moi");
		gotoxy(21, 13); printf("\t 5. Phan quyen tai khoan");
		gotoxy(21, 15); printf("\t 6. Quan ly doc gia");
		gotoxy(21, 17); printf("\t 7. Quan ly sach");
		gotoxy(21, 19); printf("\t 9. Lap phieu muon sach");
		gotoxy(21, 21); printf("\t 10. Lap phieu tra sach");
		gotoxy(21, 23); printf("\t 11. Thong ke");
		gotoxy(21, 25); printf("\t 12. Thoat chuong trinh");
		gotoxy(20, 29); printf("Lua chon cua ban:");
		scanf("%d", &luachon);
		switch (luachon)
		{
		case 1:
		{
			menu();
			break; 
		}
		case 2:
		{
			system("cls");
			char passtemp[MAX_PASS];
			do {
				// nhập pass 
				gotoxy(20, 8); printf("Nhap lai Password: ");
				// mã hóa mật khẩu
				gotoxy(39, 8);
				int p = 0;
				do {
					passtemp[p] = _getch();
					if (passtemp[p] != '\r')
					{
						printf("*");
					}
					p++;
				} while (passtemp[p - 1] != '\r');
				passtemp[p - 1] = '\0';
				if (strcmp(passtemp, login.Password) == 0)
				{
					system("cls");
					RePass(login);
				}
				else if (strcmp(passtemp, login.Password) != 0)
				{
					gotoxy(20, 10); printf("Ban da nhap sai Password");
					int choice;
					do {
						gotoxy(20, 12); printf("Thoat ra menu?");
						gotoxy(23, 14); printf("1. Co");
						gotoxy(23, 16); printf("2. Khong, nhap lai.");
						gotoxy(20, 18); printf("Lua chon cua ban: ");
						scanf("%d", &choice);
						if (choice != 1 && choice != 2)
							gotoxy(20, 20); printf("Vui long chi nhap 1 hoac 2 ");
					} while (choice != 1 && choice != 2);
					if (choice == 1)
						break;
					system("cls");
				}
			} while (strcmp(passtemp, login.Password) != 0);
			break;
		}
		case 3:
		{
			// hiển thị thông tin ban đầu trước khi chỉnh sửa
			system("cls");
			gotoxy(10, 4); printf("::::::::::::::::::::Cap nhat thong tin ca nhan::::::::::::::::::::");
			printf("\n\n\t   Thong tin tai khoan\n");
			OutputInfo(login);
			system("pause");
			system("cls");
			//chỉnh sửa thông tin
			UpdateInfo(login);
			break;
		}
		case 4:
		{
			system("cls");
			CreateAcc();
			break;
		}
		case 5:
		{
			system("cls");
			PhanQuyen();
			system("pause");
			break;
		}
		//case 6:
		//	system("cls");
		//	break;
		//case 7:
		//	system("cls");
		//	break;
		//case 8:
		//	system("cls");
		//	break;
		//case 9:
		//	system("cls");
		//	break;
		//case 10:
		//	system("cls");
		//	system("pause");
		//	break;
		//case 11:
		//	system("cls");
		//	system("pause");
		//	break;
		case 12:
		{
			system("cls");
			gotoxy(20, 11); printf("Thoat trong 3 giay nua");
			for (int j = 0; j < 60; j++)
			{
				Sleep(50);
				printf(".");
			}
			system("cls");
			exit(0);
		}
		default:
		{
			gotoxy(20, 31); printf("\aLua chon khong hop le. Vui long chon lai!      ");
			system("pause");
		}
		}
	} while (luachon != 12);
}
// menu chuyên viên
void MenuChuyenVien(ACCOUNT &login)
{
	int luachon;
	do {
		system("cls");
		gotoxy(20, 3); printf("\xB2\xB2\xB2\xB2\xB2\xB2 HE THONG QUAN LY THU VIEN - CHUYEN VIEN \xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(20, 4); printf("\xB2"); gotoxy(72, 4); printf("\xB2");
		gotoxy(20, 5); printf("\xB2"); gotoxy(72, 5); printf("\xB2");
		gotoxy(20, 6); printf("\xB2"); gotoxy(72, 6); printf("\xB2");
		gotoxy(20, 7); printf("\xB2"); gotoxy(72, 7); printf("\xB2");
		gotoxy(20, 8); printf("\xB2"); gotoxy(72, 8); printf("\xB2");
		gotoxy(20, 9); printf("\xB2"); gotoxy(72, 9); printf("\xB2");
		gotoxy(20, 10); printf("\xB2"); gotoxy(72, 10); printf("\xB2");
		gotoxy(20, 11); printf("\xB2"); gotoxy(72, 11); printf("\xB2");
		gotoxy(20, 12); printf("\xB2"); gotoxy(72, 12); printf("\xB2");
		gotoxy(20, 13); printf("\xB2"); gotoxy(72, 13); printf("\xB2");
		gotoxy(20, 14); printf("\xB2"); gotoxy(72, 14); printf("\xB2");
		gotoxy(20, 15); printf("\xB2"); gotoxy(72, 15); printf("\xB2");
		gotoxy(20, 16); printf("\xB2"); gotoxy(72, 16); printf("\xB2");
		gotoxy(20, 17); printf("\xB2"); gotoxy(72, 17); printf("\xB2");
		gotoxy(20, 18); printf("\xB2"); gotoxy(72, 18); printf("\xB2");
		gotoxy(20, 19); printf("\xB2"); gotoxy(72, 19); printf("\xB2");
		gotoxy(20, 20); printf("\xB2"); gotoxy(72, 20); printf("\xB2");
		gotoxy(20, 21); printf("\xB2"); gotoxy(72, 21); printf("\xB2");
		gotoxy(20, 22); printf("\xB2"); gotoxy(72, 22); printf("\xB2");
		gotoxy(20, 23); printf("\xB2"); gotoxy(72, 23); printf("\xB2");
		gotoxy(20, 23); printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(21, 5); printf("\t 1. Dang xuat");
		gotoxy(21, 7); printf("\t 2. Thay doi mat khau");
		gotoxy(21, 9); printf("\t 3. Cap nhat thong tin ca nhan");
		gotoxy(21, 11); printf("\t 4. Quan ly doc gia");
		gotoxy(21, 13); printf("\t 5. Quan ly sach");
		gotoxy(21, 15); printf("\t 6. Lap phieu muon sach");
		gotoxy(21, 17); printf("\t 7. Lap phieu tra sach");
		gotoxy(21, 19); printf("\t 8. Thong ke");
		gotoxy(21, 21); printf("\t 9. Thoat chuong trinh");
		gotoxy(21, 25); printf("Lua chon cua ban:");
		scanf("%d", &luachon);
		switch (luachon)
		{
		case 1:
		{
			menu();
			break;
		}
		case 2:
		{
			system("cls");
			char passtemp[MAX_PASS];
			do {
				// nhập pass 
				gotoxy(20, 8); printf("Nhap lai Password: ");
				// mã hóa mật khẩu
				gotoxy(39, 8);
				int p = 0;
				do {
					passtemp[p] = _getch();
					if (passtemp[p] != '\r')
					{
						printf("*");
					}
					p++;
				} while (passtemp[p - 1] != '\r');
				passtemp[p - 1] = '\0';
				if (strcmp(passtemp, login.Password) == 0)
				{
					system("cls");
					RePass(login);
				}
				else if (strcmp(passtemp, login.Password) != 0)
				{
					gotoxy(20, 10); printf("Ban da nhap sai Password");
					int choice;
					do {
						gotoxy(20, 12); printf("Thoat ra menu?");
						gotoxy(23, 14); printf("1. Co");
						gotoxy(23, 16); printf("2. Khong, nhap lai.");
						gotoxy(20, 18); printf("Lua chon cua ban: ");
						scanf("%d", &choice);
						if (choice != 1 && choice != 2)
							gotoxy(20, 20); printf("Vui long chi nhap 1 hoac 2 ");
					} while (choice != 1 && choice != 2);
					if (choice == 1)
						break;
					system("cls");
				}
			} while (strcmp(passtemp, login.Password) != 0);
			break;
		}
		case 3:
		{
			// hiển thị thông tin ban đầu trước khi chỉnh sửa
			system("cls");
			gotoxy(10, 4); printf("::::::::::::::::::::Cap nhat thong tin ca nhan::::::::::::::::::::");
			printf("\n\n\t   Thong tin tai khoan\n");
			OutputInfo(login);
			system("pause");
			system("cls");
			//chỉnh sửa thông tin
			UpdateInfo(login);
			break;
		}
		case 9:
		{
			system("cls");
			gotoxy(20, 11); printf("Thoat trong 3 giay nua");
			for (int j = 0; j < 60; j++)
			{
				Sleep(50);
				printf(".");
			}
			system("cls");
			exit(0);
		}
		default:
		{
			gotoxy(21, 27); printf("\aChon sai, vui long chon lai! "); system("pause");
		}
		}
	} while (luachon != 9);
}
// menu quản lý
void MenuQuanLy(ACCOUNT &login)
{
	int luachon;
	do {
		system("cls");
		gotoxy(20, 3); printf("\xB2\xB2\xB2\xB2\xB2\xB2 HE THONG QUAN LY THU VIEN - QUAN LY \xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(20, 4); printf("\xB2"); gotoxy(68, 4); printf("\xB2");
		gotoxy(20, 5); printf("\xB2"); gotoxy(68, 5); printf("\xB2");
		gotoxy(20, 6); printf("\xB2"); gotoxy(68, 6); printf("\xB2");
		gotoxy(20, 7); printf("\xB2"); gotoxy(68, 7); printf("\xB2");
		gotoxy(20, 8); printf("\xB2"); gotoxy(68, 8); printf("\xB2");
		gotoxy(20, 9); printf("\xB2"); gotoxy(68, 9); printf("\xB2");
		gotoxy(20, 10); printf("\xB2"); gotoxy(68, 10); printf("\xB2");
		gotoxy(20, 11); printf("\xB2"); gotoxy(68, 11); printf("\xB2");
		gotoxy(20, 12); printf("\xB2"); gotoxy(68, 12); printf("\xB2");
		gotoxy(20, 13); printf("\xB2"); gotoxy(68, 13); printf("\xB2");
		gotoxy(20, 14); printf("\xB2"); gotoxy(68, 14); printf("\xB2");
		gotoxy(20, 15); printf("\xB2"); gotoxy(68, 15); printf("\xB2");
		gotoxy(20, 16); printf("\xB2"); gotoxy(68, 16); printf("\xB2");
		gotoxy(20, 17); printf("\xB2"); gotoxy(68, 17); printf("\xB2");
		gotoxy(20, 18); printf("\xB2"); gotoxy(68, 18); printf("\xB2");
		gotoxy(20, 19); printf("\xB2"); gotoxy(68, 19); printf("\xB2");
		gotoxy(20, 20); printf("\xB2"); gotoxy(68, 20); printf("\xB2");
		gotoxy(20, 21); printf("\xB2"); gotoxy(68, 21); printf("\xB2");
		gotoxy(20, 22); printf("\xB2"); gotoxy(68, 22); printf("\xB2");
		gotoxy(20, 23); printf("\xB2"); gotoxy(68, 23); printf("\xB2");
		gotoxy(20, 23); printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		gotoxy(21, 5); printf("\t 1. Dang xuat");
		gotoxy(21, 7); printf("\t 2. Thay doi mat khau");
		gotoxy(21, 9); printf("\t 3. Cap nhat thong tin ca nhan");
		gotoxy(21, 11); printf("\t 4. Quan ly doc gia");
		gotoxy(21, 13); printf("\t 5. Quan ly sach");
		gotoxy(21, 15); printf("\t 6. Lap phieu muon sach");
		gotoxy(21, 17); printf("\t 7. Lap phieu tra sach");
		gotoxy(21, 19); printf("\t 8. Thong ke");
		gotoxy(21, 21); printf("\t 9. Thoat chuong trinh");
		gotoxy(21, 25); printf("Lua chon cua ban:");
		scanf("%d", &luachon);
		switch (luachon)
		{
		case 1:
		{
			menu();
			break;
		}
		case 2:
		{
			system("cls");
			char passtemp[MAX_PASS];
			do {
				// nhập pass 
				gotoxy(20, 8); printf("Nhap lai Password: ");
				// mã hóa mật khẩu
				gotoxy(39, 8);
				int p = 0;
				do {
					passtemp[p] = _getch();
					if (passtemp[p] != '\r')
					{
						printf("*");
					}
					p++;
				} while (passtemp[p - 1] != '\r');
				passtemp[p - 1] = '\0';
				if (strcmp(passtemp, login.Password) == 0)
				{
					system("cls");
					RePass(login);
				}
				else if (strcmp(passtemp, login.Password) != 0)
				{
					gotoxy(20, 10); printf("Ban da nhap sai Password");
					int choice;
					do {
						gotoxy(20, 12); printf("Thoat ra menu?");
						gotoxy(23, 14); printf("1. Co");
						gotoxy(23, 16); printf("2. Khong, nhap lai.");
						gotoxy(20, 18); printf("Lua chon cua ban: ");
						scanf("%d", &choice);
						if (choice != 1 && choice != 2)
							gotoxy(20, 20); printf("Vui long chi nhap 1 hoac 2 ");
					} while (choice != 1 && choice != 2);
					if (choice == 1)
						break;
					system("cls");
				}
			} while (strcmp(passtemp, login.Password) != 0);
			break;
		}
		case 3:
		{
			// hiển thị thông tin ban đầu trước khi chỉnh sửa
			system("cls");
			gotoxy(10, 4); printf("::::::::::::::::::::Cap nhat thong tin ca nhan::::::::::::::::::::");
			printf("\n\n\t   Thong tin tai khoan\n");
			OutputInfo(login);
			system("pause");
			system("cls");
			//chỉnh sửa thông tin
			UpdateInfo(login);
			break;
		}
		case 9:
		{
			system("cls");
			gotoxy(20, 11); printf("Thoat trong 3 giay nua");
			for (int j = 0; j < 60; j++)
			{
				Sleep(50);
				printf(".");
			}
			system("cls");
			exit(0);
		}
		default:
		{
			gotoxy(21, 27); printf("\aChon sai, vui long chon lai! "); system("pause");
		}
		}
	} while (luachon != 9);
}





