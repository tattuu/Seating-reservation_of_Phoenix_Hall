#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define MARK '/' /* �Q�l�������̈͂ݕ��� */
#define MRLNG 5 //�\����Ȑ��͈̔͏��

typedef struct listelement { //�\���̂̃��X�g
    char     *dataitem; // �f�[�^��
    struct listelement *link; // �A�h���X��(�����N��)
} listelement;

void pro_sel(void); // �\�񂷂�����ƍ��Ȑ��̎w��
void input_res(char kouen[], int zaseki); // �\��ҏ��̓���
void output_res(char kouen[], int zaseki, char name[], char address[], char tel[]); // �\��ҏ�����ʂƃt�@�C���ɏo��
int GetRandom(int min,int max); // ��������
void book_app(char *); // �\����Ȃƒʂ��ԍ��̃t�@�C���o��
listelement *AddItem (listelement *listpointer, char *data); // ������listpointer�ɂ�����̈�����data��ǉ�
int SearchItem( listelement*listpointer, char *data ); // ������listpointer���̃f�[�^�ƁA������̈�����*data���̃f�[�^���r����B
void res_pass_input(void); // �\����̍폜�E�m�F�Ɏg���ʂ��ԍ��A�p�X���[�h�̓��͂ƁA������p���������t�@�C�����̍쐬
void Clear(void); // �ʂ̗\��ҏ��t�@�C���̍폜
void Confirmation(void); // �ʂ̗\��ҏ��̊m�F
void situation(void); // �z�[���̍��Ȕz�u�}
void direct_sel(int); // ���Ȃ̒��ڎw��
void seat_sel(int zaseki); // �\������w��Ɨ\�񂵂悤�Ƃ��Ă�����Ȃ̊m�F

FILE *ifp, *ifp2, *ofp; // �����̊֐��ŗp����̂ŃO���[�o���ϐ���

int judge = 0, judge2 = 0, same, list, int_ref_name=0, toosi_make=1, count=1;
char filename[19], zaseki1[MRLNG]="0", zaseki2[MRLNG]="0", zaseki3[MRLNG]="0", zaseki4[MRLNG]="0", zaseki5[MRLNG]="0", concert_program[50][50];

int main(void){
	char num[2];
	char *e;
	int int_num;

	while(1){
		printf("�@�\����\n�A�\����폜\n�B�\����m�F\n--------------------\n�����ł��I�т��������F");
		fflush(stdin); // ��������
		fgets(num, 2, stdin); // ��������
		int_num=atoi(num);
		if((strtol(num,&e,0)==0) || int_num>3 || int_num<1){
			printf("\n�@�`�B�̒�������͂��Ă��������B\n\n");
			continue;
		}
		else{
			break;
		}
	}
	if(int_num==1){	//�\�������
		pro_sel();
	}
	else if(int_num==2 || int_num==3){

		res_pass_input();// �\����̍폜�E�m�F�Ɏg���ʂ��ԍ��A�p�X���[�h�̓��͂ƁA������p���������t�@�C�����̍쐬

		if(int_num==2){	//�\����폜
			Clear();
		}

		else if(int_num==3){	//�\����m�F
			Confirmation();
		}
	}
	return 0;
}

void pro_sel(void){
	char data[2], zaseki[2], ch;
	char *e;
	int int_data, int_zaseki, i, j = 0;
	
	FILE *ifp_concert_program; // ���ڂ�ǂݍ��ނ��߂Ɏg���t�@�C��

	while(1){ // ����ɓ��͂����܂ŌJ��Ԃ�
		printf("\n���L�̐����̒�������͂��Ă��������B\n\n");
		if((ifp_concert_program = fopen("configration/concert_program.txt","r"))==NULL){
			printf("���̓t�@�C�����J���܂���ł����B\n");
			return;
		}
		while((ch = fgetc(ifp_concert_program)) != EOF){ // EOF�ɂȂ�܂�ch�ɑ��
			if(ch == MARK){
				for(i = 0; (concert_program[j][i] = fgetc(ifp_concert_program)) != MARK; i++); // �o�b�N�X���b�V���܂�concert_program[i]�ɏo��
				concert_program[j][i] = '\0';
			}
			printf("%d�F%s\n", j + 1, concert_program[j]);
			j++;
		}
		j=1;
		fclose(ifp_concert_program);
		printf("\n---------------------\n�����ł��I�щ������F");
		fflush(stdin); // ���͂̃o�b�t�@�N���A
		fgets(data, 2, stdin); // ��������
		int_data=atoi(data);
		if((strtol(data,&e,0)==0) || (int_data>3 || int_data<1)){
			printf("\n1�`3�̒�������͂��Ă��������B\n");
			continue;
		}
		else{
			break;
		}
	}

	while(1){
		printf("\n�\�񂵂������Ȑ���1�`5�̐����œ��͂��Ă��������F");

		fflush(stdin); // ��������
		fgets(zaseki, 2, stdin); // ��������
		int_zaseki=atoi(zaseki);
		if((strtol(zaseki,&e,0)==0) || (int_zaseki>5 || int_zaseki<1)){
			printf("\n1�`5�̒�������͂��Ă��������B\n");
			continue;
		}
		else{
			break;
		}
	}

	situation();	//���Ȑ}�̕\��
	seat_sel(int_zaseki);	//���Ȃ̗\��
	input_res(data, int_zaseki);	//�\��ҏ�����

	return;
}

void situation(void)
{
	printf("                   |�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�X�e�[�W�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n");
	printf("                   �\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\\n");
	printf("\n\n\n");
	printf("�@A|�@01 02 03 04|�@|05 06 07 08 09 10 11 12 13 14 15 16|�@|17 18 19 20 21|A�@\n");
	printf("�@B|�@01 02 03 04|��|05 06 07 08 09 10 11 12 13 14 15 16|��|17 18 19 20 21|B�@\n");
	printf("�@C|�@01 02 03 04|�@|05 06 07 08 09 10 11 12 13 14 15 16|�@|17 18 19 20 21|C�@\n");
	printf("�@D|�@01 02 03 04|�H|05 06 07 08 09 10 11 12 13 14 15 16|�H|17 18 19 20 21|D�@\n");
	printf("�@E|�@01 02 03 04|�@|05 06 07 08 09 10 11 12 13 14 15 16|�@|17 18 19 20 21|E�@\n");
	printf("�@F|�@01 02 03 04|��|05 06 07 08 09 10 11 12 13 14 15 16|��|17 18 19 20 21|F�@\n");
	printf("�@G|�@ �@�@�@�@�@|�@|05 06 07 08 09 10 11 12 13 14 15 16|�@|�@�@�@�@�@�@�@|G�@\n");
	printf("�@H|�@ �@�@�@�@�@|�H|05 06 07 08 09 10 11 12 13 14 15 16|�H|�@�@�@�@�@�@�@|H�@\n");
	printf("�@I|�@ �@�@�@�@�@|�@|05 06 07 08 09 10 11 12 13 14 15 16|�@|�@�@�@�@�@�@�@|I�@\n");
	printf("\n\n");
	printf("***********************************�z�[��1�K��********************************\n");
	printf("\n\n");

	printf("   01 01 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 44 47 �@\n");
	printf("�@ 02 02 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 43 46 �@\n");
	printf("03 03 03 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 42 45 44\n");
	printf("04 04 04 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 41 44 43\n");
	printf("05 05 05 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 40 43 42\n");
	printf("06 06 06 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 39 42 41\n");
	printf("07 07 07 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 38 41 40\n");
	printf("08 08 08 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 37 40 39\n");
	printf("09 09 09 |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| 36 49 38\n");
	printf("CC BB AA |�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@| AA BB CC\n");
	printf("�@�@�@�@10�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@35�@�@�@�@\n");
	printf("�@�@�@10�@11�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@34�@38�@�@�@\n");
	printf("�@�@10�@11�@12�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@33�@37�@37�@�@\n");
	printf("�@�@�@11�@12�@13�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@32�@36�@36�@�@�@\n");
	printf("�@�@�@�@12�@13�@14�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@31�@35�@35�@�@�@�@\n");
	printf("�@�@�@�@�@13�@14�@15�@�@�@ �@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@30�@34�@34�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@14�@15�@16�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@29�@33�@33�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@15�@16�@17�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@28�@32�@32�@�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@�@16�@17�@18�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@27�@31�@31�@�@�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@�@�@17�@18�@19�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@26�@30�@30�@�@�@�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@�@�@�@18�@19�@ �@AA 20 21 22 23 24 25 AA�@�@�@29�@29�@�@�@�@�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@�@�@�@�@19�@20�@BB 21 22 23 24 25 26 27 BB�@28�@28�@�@�@�@�@�@�@�@�@�@�@\n");
	printf("�@�@�@�@�@�@�@�@�@�@�@�@�@�@CC 21 22 23 24 25 26 27 28 CC�@�@�@�@�@�@�@�@�@�@�@�@�@�@\n");
	printf("\n\n");
	printf("***********************************�z�[��2�K��********************************\n");
	printf("\n\n");

	return;
}

void seat_sel(int zaseki)
{
	char sel[2], check[2]; //sel�͍��Ȏw����@, check�͊ԈႢ�m�F
	char *e;
	int int_sel, int_check;

	while(1){
		while(1){
			printf("���Ȃ̎w����@���ȉ���3�ł��B\n�@���ڎw��\n--------------------\n���p�����ł����͂��������F");

			fflush(stdin); // ��������
			fgets(sel, 2, stdin); // ��������
			int_sel=atoi(sel);
			if((strtol(sel,&e,0)==0) || (int_sel>3 || int_sel<1)){
				printf("\n1�`3�̒�������͂��Ă��������B\n");
				continue;
			}
			else{
				break;
			}
		}

		switch(int_sel){
			case 1:	//���ڎw���I�������ꍇ
				direct_sel(zaseki);
				break;
		}

		printf("�\�񂷂���Ȃ�\n");
		printf("1.%s\n", zaseki1);
		if(zaseki2[0]!='0'){	//2�Ȗڂ̗\��������ꍇ
			printf("2.%s\n", zaseki2);
		}
		if(zaseki3[0]!='0'){	//3�Ȗڂ̗\��������ꍇ
			printf("3.%s\n", zaseki3);
		}
		if(zaseki4[0]!='0'){	//4�Ȗڂ̗\��������ꍇ
			printf("4.%s\n", zaseki4);
		}
		if(zaseki5[0]!='0'){	//5�Ȗڂ̗\��������ꍇ
			printf("5.%s\n", zaseki5);
		}

		while(1){
			printf("�ł��B���̍��Ȃł�낵���ł����H\n���p�����œ��͂��Ă��������B\n--------------------\n�@�͂�\n�A������\n�񓚁F");
			fflush(stdin); // ��������
			fgets(check, 2, stdin); // ��������
			int_check=atoi(check);
			if(strtol(check,&e,0)==0){
				printf("\n�@���A�̒�������͂��Ă��������B\n");
				continue;
			}
			else if(int_check > 2 || int_check < 1){
				printf("\n�@���A�̒�������͂��Ă��������B\n");
				continue;
			}
			else{
				break;
			}
		}
		if(int_check==2){ //�\�񂷂���Ȃ��ԈႢ����̏ꍇ���s����
			printf("\n�ē��͂��肢���܂��B\n");
			continue;
		}
		else{
			break;
		}
	}

	return;
}

void direct_sel(int num)
{
	int i; //���[�v�ϐ�

	printf("�\�񂷂���Ȃ𔼊p�p�����œ��͂��Ă��������B���A�������Ȃ�����ꍇ�͓��͍��Ȃ��Ƃ�Enter�������Ă��������B\n");

	for(i=0;i<num;i++)
	{
		book_app("configration/zaseki.txt"); // �֐��Ăяo��(�����́A���̓t�@�C���Əo�̓t�@�C��)
	}
	toosi_make=0; // ���ȗ\������邽�߂�book_app�����s���Ă��鎞�ɁA�K�v�ȏ��������������邽�߂�toosi_make��+1

	return;
}

void book_app(char *in_filename){

	listelement *listpointer;
	listelement *listpointer2;

	char ch,ref_name[5], data[5];
    int i, flag = 0,check1;

	if(toosi_make==1){ // �ʗ\����̒ʂ��ԍ��Ɋւ��鏈������Ȃ���
		ifp = fopen(in_filename, "a+");

		if((ifp2=fopen("configration/seat_list.txt","r"))==NULL){
			printf("���̓t�@�C�����J���܂���ł����B\n");
			return;
		}

		listpointer = NULL;
		listpointer2 = NULL;
	}
	else{ // �ʗ\����̒ʂ��ԍ��Ɋւ��鏈���̎�
		ifp = fopen(in_filename, "a+");
	}

	while(1){
		if(toosi_make==1){
			printf("���ȁF");
			fflush(stdin); // ��������
		    fgets(data, 5, stdin); // ��������

			for(i=1; i<4; i++){ // 4,3,2�����̏ꍇ�A�󂫕�����\n������̂ŁA�����\0�ŏ㏑�����A������̍Ō��\n������Ȃ��悤�ɂ���
				if(data[i]=='\n'){
					data[i]='\0';
				}
			}
		}

		while((ch = fgetc(ifp)) != EOF){ // EOF�ɂȂ�܂�ch�ɑ��
			if(flag==0){ // �������ꕶ���ł����݂����ꍇ�Aflag��1�ɂ���
				flag=1;
			}

			if(ch == MARK){
				for(i = 0; (ref_name[i] = fgetc(ifp)) != MARK; i++); // �o�b�N�X���b�V���܂�ref_name�ɏo��
				ref_name[i] = '\0';
				if(toosi_make==1){
					if(judge == 0){ // ���ڂ̓��͂��ǂ���
						listpointer = AddItem (listpointer, ref_name);
						judge = 1; //�������else if�ɂ��邽�߂�judge��1��
					}
					else if(SearchItem (listpointer, ref_name)!=same && judge == 1){  // listpointer�̒��ɍ����݂�ref_name�̕��͂��Ȃ��A���A���ڈȍ~�̓��͂��ǂ���
						AddItem (listpointer, ref_name);
					}
				}
			}
		}

		if(toosi_make==1){
			while((ch = fgetc(ifp2)) != EOF){ // EOF�ɂȂ�܂�ch�ɑ��
				if(ch == MARK){
					for(i = 0; (ref_name[i] = fgetc(ifp2)) != MARK; i++); // �o�b�N�X���b�V���܂�ref_name�ɏo��
					ref_name[i] = '\0';

					if(judge2 == 0){ // ���ڂ̓��͂��ǂ���
						listpointer2 = AddItem (listpointer2, ref_name);
						judge2 = 1; //�������else if�ɂ��邽�߂�judge��1��
					}
					else if(judge2 == 1){  // listpointer�̒��ɍ����݂�ref_name�̕��͂��Ȃ��A���A���ڈȍ~�̓��͂��ǂ���
						AddItem (listpointer2, ref_name);
					}
				}
			}

			check1=SearchItem (listpointer, data); // ���̓f�[�^�ƃt�@�C�����f�[�^(�\��ςݍ���)�̔�r
			SearchItem (listpointer2,data);

			if(check1==500&&list==1){ // �\�񂵂悤�Ƃ��Ă�����Ȃ��Azaseki.txt�ɑ��݂��Ă��Ȃ��A���Azaseki_list.txt�ɍ��Ȃ����݂����ꍇ
				fprintf (ifp,"/%s/",data); // �\����Ȃ��t�@�C���ɏo��

				if(count==1){ // ��ڂ̍���
					strcpy(zaseki1, data); // �\����Ȃ𑼂̊֐��Ŏg���̂ŁA�O���[�o���ϐ�zaseki1[5]�ɑ��
				}
				else if(count==2){// ��ڂ̍���
					strcpy(zaseki2, data); // �\����Ȃ𑼂̊֐��Ŏg���̂ŁA�O���[�o���ϐ�zaseki2[5]�ɑ��
				}
				else if(count==3){ // �O�ڂ̍���
					strcpy(zaseki3, data); // �\����Ȃ𑼂̊֐��Ŏg���̂ŁA�O���[�o���ϐ�zaseki3[5]�ɑ��
				}
				else if(count==4){ // �l�ڂ̍���
					strcpy(zaseki4, data); // �\����Ȃ𑼂̊֐��Ŏg���̂ŁA�O���[�o���ϐ�zaseki4[5]�ɑ��
				}
				else if(count==5){ // �܂ڂ̍���
					strcpy(zaseki5, data); // �\����Ȃ𑼂̊֐��Ŏg���̂ŁA�O���[�o���ϐ�zaseki5[5]�ɑ��
				}
				count++; // ����ڂ̗\����Ȃ��̃J�E���g

				printf("���Ȃ̗\�񊮗�\n");
				fclose(ifp2);
				break;
			}else{
				printf("���̍��Ȃ͗\�񂷂邱�Ƃ��ł��܂���B\n������x���͂��Ă�������\n");
				continue;
			}
		}
		else{ // �ʗ\����̒ʂ��ԍ��Ɋւ��鏈��
			if(flag==0){ // zaseki_info.txt�ɉ���������Ă��Ȃ����
				ref_name[0]='0';
				ref_name[1]='\0';
			}
			int_ref_name=atoi(ref_name); // ������̒l�������Ă���̂ŁA�����ɂ���
			int_ref_name++; // �ʂ��ԍ���+1����

			fprintf(ifp, "/%d/", int_ref_name); // �t�@�C���o��(��fprintf�̒���++���Ă������ɂȂ�)
			break;
		}
	}

	fclose(ifp);

	return;
}

void input_res(char kouen[], int zaseki){

	char address[30], name[20], tel[12], select[2]; //�Z���A�����A�d�b�ԍ��A���������l�����͂��ꂽ��������
	char *e;
	int int_select;

	while(1){
		printf("\n�Z���E�����E�d�b�ԍ�����͂��Ă��������B\n");

		printf("�Z���F");
		fflush(stdin); // ��������
		fgets(address, 30, stdin); // ��������
		printf("�����F");
		fflush(stdin); // ��������
		fgets(name, 20, stdin); // ��������
		printf("�d�b�ԍ�(���p�����Ńn�C�t���Ȃ�)�F");
		fflush(stdin); // ��������
		fgets(tel, 12, stdin); // ��������

		printf("--------------------\n");
		while(1){
			printf("�@�ԈႢ�Ȃ�\n�A�ԈႢ����\n���p�����œ��͂��Ă��������F");
			fflush(stdin); // ��������
			fgets(select, 2, stdin); // ��������
			int_select=atoi(select);
			if(strtol(select,&e,0)==0){
				printf("\n�@���A�̒�������͂��Ă��������B\n");
				continue;
			}
			else if(int_select > 2 || int_select < 1){
				printf("\n�@���A�̒�������͂��Ă��������B\n");
				continue;
			}
			else{
				break;
			}
		}
		if(int_select==2){
			printf("\n�ē��͂��肢���܂��B\n");
			continue;
		}
		else{
			break;
		}
	}

	output_res(kouen, zaseki, name, address, tel);

	return;
}

void output_res(char kouen[], int zaseki, char name[], char address[], char tel[]){

	int i;
	time_t t;
	struct tm *tp;
	char calender[256], pass[5];
	char pass_moto[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // �p�X���[�h�̊�ƂȂ镶����

	FILE *ofp_info;

	book_app("configration/zaseki_info.txt");

	/* ���P�[������{�ɐݒ� */
	setlocale(LC_TIME, "Japanese");
	/* ���݂̗�����擾 */
	time(&t);
	/* �ڍ׎����ɕϊ� */
	tp = localtime(&t);
	strftime(calender, sizeof(calender),"�\������F%x\n", tp);
	/* ��ʂɕ\�� */
	printf("\n********************\n�\����\n\n");
	printf("%s\n", calender);
	printf("�������F%s\n", concert_program[atoi(kouen) - 1]);
	printf("�\����Ȑ��F%d\n", zaseki);
	printf("�\����Ȕԍ��F%s", zaseki1);

	if(zaseki2[0]!='0'){ // ��ڂ̍��Ȃ����͂���Ă���ꍇ
		printf(",%s", zaseki2);
	}
	if(zaseki3[0]!='0'){ // �O�ڂ̍��Ȃ����͂���Ă���ꍇ
		printf(",%s", zaseki3);
	}
	if(zaseki4[0]!='0'){ // �l�ڂ̍��Ȃ����͂���Ă���ꍇ
		printf(",%s", zaseki4);
	}
	if(zaseki5[0]!='0'){ // �܂ڂ̍��Ȃ����͂���Ă���ꍇ
		printf(",%s", zaseki5);
	}
	printf("\n");
	printf("�\����ʂ��ԍ��F");
	printf("%d\n",int_ref_name);

	printf("�p�X���[�h�F");
	for(i=0; i<4; i++){
		pass[i]=pass_moto[GetRandom(1,52)%(52)]; // �����_���ȕ�����̃p�X���[�h�𐶐�
	}
	pass[i]='\0';

	printf("%s", pass);
	printf("\n");

	printf("\n�\��҂�\n");
	printf("���O�F%s\n", name);
	printf("�Z���F%s\n", address);
	printf("�d�b�ԍ��F%s\n", tel);
	printf("********************\n");

	sprintf(filename, "reservation_infomation/%04d-%s_info.txt", int_ref_name, pass);

	ofp_info=fopen(filename, "w");
	fprintf(ofp_info, "%s\n", calender);
	fprintf(ofp_info, "�������F%s\n", concert_program[atoi(kouen) - 1]);
	fprintf(ofp_info, "�\����Ȑ��F%d\n", zaseki);
	fprintf(ofp_info, "�\����Ȕԍ��F%s", zaseki1);
	if(zaseki2[0]!='0'){ // ��ڂ̍��Ȃ����͂���Ă���ꍇ
		fprintf(ofp_info, ",%s", zaseki2);
	}
	if(zaseki3[0]!='0'){ // �O�ڂ̍��Ȃ����͂���Ă���ꍇ
		fprintf(ofp_info, ",%s", zaseki3);
	}
	if(zaseki4[0]!='0'){ // �l�ڂ̍��Ȃ����͂���Ă���ꍇ
		fprintf(ofp_info, ",%s", zaseki4);
	}
	if(zaseki5[0]!='0'){ // �܂ڂ̍��Ȃ����͂���Ă���ꍇ
		fprintf(ofp_info, ",%s", zaseki5);
	}
	fprintf(ofp_info, "\n");
	fprintf(ofp_info, "�\����ʂ��ԍ��F%d\n", int_ref_name);
	fprintf(ofp_info, "�p�X���[�h�F%s\n", pass);

	fprintf(ofp_info, "\n�\��҂�\n");
	fprintf(ofp_info, "���O�F%s\n", name);
	fprintf(ofp_info, "�Z���F%s\n", address);
	fprintf(ofp_info, "�d�b�ԍ��F%s\n", tel);

	fclose(ofp_info);
	return;
}

int GetRandom(int min,int max){
	static int flag;

	if (flag == 0) {
		srand((unsigned int)time(NULL));
		flag = 1;
	}

	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

listelement *AddItem (listelement *listpointer, char *data) {

    listelement *lp = listpointer; // listpointer��lp�ɑ��
	char *newdata; // additional char*
	newdata = (char *) malloc(strlen(data)+1); // memory for string
	strcpy( newdata, data ); // copy string from data to newdata

	if (listpointer != NULL) { // listpointer�̒��Ƀf�[�^���������ꍇ
	while (listpointer -> link != NULL) // �f�[�^���Ȃ����܂ŉ�
	    listpointer = listpointer -> link; // ���̃��X�g���w��
	listpointer -> link = (struct listelement *) malloc (sizeof (listelement)); // �f�[�^�T�C�Y�m��
	listpointer = listpointer -> link; // �V���ɍ����link��V���ɍ�����Ō�̃��X�g�̐擪���w���悤�ɂ���
	listpointer -> link = NULL; // �Ō�̃��X�g�̃A�h���X����NULL������
	listpointer -> dataitem = newdata; // �f�[�^���ɐV���ȃf�[�^��ǉ�
	return lp; // �V���ȃf�[�^��ǉ�����listpointer��Ԃ�
    }
    else { // ����f�[�^�������Ă��Ȃ�������
	listpointer = (struct listelement  *) malloc (sizeof (listelement)); // listpointer�ɕK�v�ȃf�[�^�T�C�Y�m��
	listpointer -> link = NULL; // �����N����NULL������
	listpointer -> dataitem = newdata; // �f�[�^���ɐV���ȃf�[�^��ǉ�
	return listpointer; // ��̃f�[�^���������Ă��Ȃ�listpointer��Ԃ�
    }
}


int SearchItem( listelement*listpointer, char *data )
{
	int i= 0;

	same=0; // same�����Z�b�g

	while ( listpointer!= NULL){/* �L���[�̍Ō�܂�*/
		i++;/* �J�E���g�A�b�v*/
		if ( strcmp( data, listpointer -> dataitem ) == 0 ) { /* �������r*/
			same = i; // listpointer�̒��ɔ�r�Ώۃf�[�^�����݂������ǂ����̔��f�Ɏg�p(book_app�֐�����if���Ŏg�p)
			list=1;
			return i;
		}

		else {/* ������Ȃ�����*/
			listpointer= listpointer-> link;/* �����N�����ǂ���*/
		}
	}
	return 500;/* �Ō�܂Ō�����Ȃ�������i��Ԃ�*/
}


void res_pass_input(void){
	int i, int_toosi_num;
	char pass_num[5], toosi_num[301];
	char *e;

	while(1){
		printf("\n�ʂ��ԍ�������͂��Ă�������\n");
		fflush(stdin); // ��������
	    fgets(toosi_num, 5, stdin); // ��������
		int_toosi_num=atoi(toosi_num);
		if((strtol(toosi_num,&e,0)==0) || (int_toosi_num>301 || int_toosi_num<1)){
			printf("\n1����301�̒�������͂��Ă��������B\n");
			continue;
		}
		else{
			break;
		}
	}
	for(i=1; i<4; i++){ // 4,3,2�����̏ꍇ�A�󂫕�����\n������̂ŁA�����\0�ŏ㏑�����A������̍Ō��\n������Ȃ��悤�ɂ���
		if(toosi_num[i]=='\n'){
			toosi_num[i]='\0';
		}
	}
	while(1){
		printf("\n�p�X���[�h����͂��Ă�������\n");
		fflush(stdin); // ��������
	    fgets(pass_num, 5, stdin); // 4�����ȓ��̕�������
		for(i=1; i<4; i++){ // 4,3,2�����̏ꍇ�A�󂫕�����\n���������ꍇ
			if(pass_num[i]=='\n'){
				printf("\n4�����̃p�X���[�h����͂��Ă�������\n");
				continue;
			}
		}
		if(strtol(pass_num,&e,0)!=0){
			printf("\n�����݂̂̃p�X���[�h����͂��Ă�������\n");
			continue;
		}
		break;
	}
	sprintf(filename, "%04s-%s_info.txt", toosi_num, pass_num); // �ʂ̗\����t�@�C�������쐬

	return;
}

void Clear(void){
  	if( remove( filename ) == 0 ){ // �t�@�C�������݂��A�폜�ł���ꍇ

    	printf( "�\������폜���܂���\n");
	}
  	else{
    	printf( "�\����폜�Ɏ��s���܂���\n���͓��e���m�F���A�ēx�����͂�������\n");
  	}
	return;
}

void Confirmation(void){
	char itigyou_output[30];

	if((ifp = fopen(filename, "r"))==NULL){
		printf("���͂��ꂽ�\����͑��݂��܂���ł����B\n���͓��e���m�F���A�ēx�����͂�������\n");
		return;
	}
	while(fgets(itigyou_output, 29, ifp)!=NULL){ //��s����ʂɕ\��
		printf("%s", itigyou_output);
	}

	fclose(ifp);

	return;
}
