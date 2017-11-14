#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define MARK '/' /* 参考資料名の囲み文字 */
#define MRLNG 5 //予約座席数の範囲上限

typedef struct listelement { //構造体のリスト
    char     *dataitem; // データ部
    struct listelement *link; // アドレス部(リンク部)
} listelement;

void pro_sel(void); // 予約する公演と座席数の指定
void input_res(char kouen[], int zaseki); // 予約者情報の入力
void output_res(char kouen[], int zaseki, char name[], char address[], char tel[]); // 予約者情報を画面とファイルに出力
int GetRandom(int min,int max); // 乱数生成
void book_app(char *); // 予約座席と通し番号のファイル出力
listelement *AddItem (listelement *listpointer, char *data); // 引数のlistpointerにもう一つの引数のdataを追加
int SearchItem( listelement*listpointer, char *data ); // 引数のlistpointer内のデータと、もう一つの引数の*data内のデータを比較する。
void res_pass_input(void); // 予約情報の削除・確認に使う通し番号、パスワードの入力と、それらを用いた検索ファイル名の作成
void Clear(void); // 個別の予約者情報ファイルの削除
void Confirmation(void); // 個別の予約者情報の確認
void situation(void); // ホールの座席配置図
void direct_sel(int); // 座席の直接指定
void seat_sel(int zaseki); // 予約方式指定と予約しようとしている座席の確認

FILE *ifp, *ifp2, *ofp; // 複数の関数で用いるのでグローバル変数に

int judge = 0, judge2 = 0, same, list, int_ref_name=0, toosi_make=1, count=1;
char filename[19], zaseki1[MRLNG]="0", zaseki2[MRLNG]="0", zaseki3[MRLNG]="0", zaseki4[MRLNG]="0", zaseki5[MRLNG]="0", concert_program[50][50];

int main(void){
	char num[2];
	char *e;
	int int_num;

	while(1){
		printf("①予約情報\n②予約情報削除\n③予約情報確認\n--------------------\n数字でお選びください：");
		fflush(stdin); // 文字入力
		fgets(num, 2, stdin); // 文字入力
		int_num=atoi(num);
		if((strtol(num,&e,0)==0) || int_num>3 || int_num<1){
			printf("\n①～③の中から入力してください。\n\n");
			continue;
		}
		else{
			break;
		}
	}
	if(int_num==1){	//予約情報入力
		pro_sel();
	}
	else if(int_num==2 || int_num==3){

		res_pass_input();// 予約情報の削除・確認に使う通し番号、パスワードの入力と、それらを用いた検索ファイル名の作成

		if(int_num==2){	//予約情報削除
			Clear();
		}

		else if(int_num==3){	//予約情報確認
			Confirmation();
		}
	}
	return 0;
}

void pro_sel(void){
	char data[2], zaseki[2], ch;
	char *e;
	int int_data, int_zaseki, i, j = 0;
	
	FILE *ifp_concert_program; // 演目を読み込むために使うファイル

	while(1){ // 正常に入力されるまで繰り返す
		printf("\n下記の数字の中から入力してください。\n\n");
		if((ifp_concert_program = fopen("configration/concert_program.txt","r"))==NULL){
			printf("入力ファイルを開けませんでした。\n");
			return;
		}
		while((ch = fgetc(ifp_concert_program)) != EOF){ // EOFになるまでchに代入
			if(ch == MARK){
				for(i = 0; (concert_program[j][i] = fgetc(ifp_concert_program)) != MARK; i++); // バックスラッシュまでconcert_program[i]に出力
				concert_program[j][i] = '\0';
			}
			printf("%d：%s\n", j + 1, concert_program[j]);
			j++;
		}
		j=1;
		fclose(ifp_concert_program);
		printf("\n---------------------\n数字でお選び下さい：");
		fflush(stdin); // 入力のバッファクリア
		fgets(data, 2, stdin); // 文字入力
		int_data=atoi(data);
		if((strtol(data,&e,0)==0) || (int_data>3 || int_data<1)){
			printf("\n1～3の中から入力してください。\n");
			continue;
		}
		else{
			break;
		}
	}

	while(1){
		printf("\n予約したい座席数を1～5の数字で入力してください：");

		fflush(stdin); // 文字入力
		fgets(zaseki, 2, stdin); // 文字入力
		int_zaseki=atoi(zaseki);
		if((strtol(zaseki,&e,0)==0) || (int_zaseki>5 || int_zaseki<1)){
			printf("\n1～5の中から入力してください。\n");
			continue;
		}
		else{
			break;
		}
	}

	situation();	//座席図の表示
	seat_sel(int_zaseki);	//座席の予約
	input_res(data, int_zaseki);	//予約者情報入力

	return;
}

void situation(void)
{
	printf("                   |――――――――――――――――――|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　ステージ　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   |　　　　　　　　　　　　　　　　　　|\n");
	printf("                   ―――――――――――――――――――\n");
	printf("\n\n\n");
	printf("　A|　01 02 03 04|　|05 06 07 08 09 10 11 12 13 14 15 16|　|17 18 19 20 21|A　\n");
	printf("　B|　01 02 03 04|通|05 06 07 08 09 10 11 12 13 14 15 16|通|17 18 19 20 21|B　\n");
	printf("　C|　01 02 03 04|　|05 06 07 08 09 10 11 12 13 14 15 16|　|17 18 19 20 21|C　\n");
	printf("　D|　01 02 03 04|路|05 06 07 08 09 10 11 12 13 14 15 16|路|17 18 19 20 21|D　\n");
	printf("　E|　01 02 03 04|　|05 06 07 08 09 10 11 12 13 14 15 16|　|17 18 19 20 21|E　\n");
	printf("　F|　01 02 03 04|通|05 06 07 08 09 10 11 12 13 14 15 16|通|17 18 19 20 21|F　\n");
	printf("　G|　 　　　　　|　|05 06 07 08 09 10 11 12 13 14 15 16|　|　　　　　　　|G　\n");
	printf("　H|　 　　　　　|路|05 06 07 08 09 10 11 12 13 14 15 16|路|　　　　　　　|H　\n");
	printf("　I|　 　　　　　|　|05 06 07 08 09 10 11 12 13 14 15 16|　|　　　　　　　|I　\n");
	printf("\n\n");
	printf("***********************************ホール1階席********************************\n");
	printf("\n\n");

	printf("   01 01 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 44 47 　\n");
	printf("　 02 02 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 43 46 　\n");
	printf("03 03 03 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 42 45 44\n");
	printf("04 04 04 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 41 44 43\n");
	printf("05 05 05 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 40 43 42\n");
	printf("06 06 06 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 39 42 41\n");
	printf("07 07 07 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 38 41 40\n");
	printf("08 08 08 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 37 40 39\n");
	printf("09 09 09 |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| 36 49 38\n");
	printf("CC BB AA |　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　| AA BB CC\n");
	printf("　　　　10　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　35　　　　\n");
	printf("　　　10　11　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　34　38　　　\n");
	printf("　　10　11　12　　　　　　　　　　　　　　　　　　　　　　　　　　　　　33　37　37　　\n");
	printf("　　　11　12　13　　　　　　　　　　　　　　　　　　　　　　　　　　　32　36　36　　　\n");
	printf("　　　　12　13　14　　　　　　　　　　　　　　　　　　　　　　　　　31　35　35　　　　\n");
	printf("　　　　　13　14　15　　　 　 　　　　　　　　　　　　　　　　　　30　34　34　　　　　\n");
	printf("　　　　　　14　15　16　　　　　　　　　　　　　　　　　　　　　29　33　33　　　　　　\n");
	printf("　　　　　　　15　16　17　　　　　　　　　　　　　　　　　　　28　32　32　　　　　　　\n");
	printf("　　　　　　　　16　17　18　　　　　　　　　　　　　　　　　27　31　31　　　　　　　　\n");
	printf("　　　　　　　　　17　18　19　　　　　　　　　　　　　　　26　30　30　　　　　　　　　\n");
	printf("　　　　　　　　　　18　19　 　AA 20 21 22 23 24 25 AA　　　29　29　　　　　　　　　　\n");
	printf("　　　　　　　　　　　19　20　BB 21 22 23 24 25 26 27 BB　28　28　　　　　　　　　　　\n");
	printf("　　　　　　　　　　　　　　CC 21 22 23 24 25 26 27 28 CC　　　　　　　　　　　　　　\n");
	printf("\n\n");
	printf("***********************************ホール2階席********************************\n");
	printf("\n\n");

	return;
}

void seat_sel(int zaseki)
{
	char sel[2], check[2]; //selは座席指定方法, checkは間違い確認
	char *e;
	int int_sel, int_check;

	while(1){
		while(1){
			printf("座席の指定方法を以下の3つです。\n①直接指定\n--------------------\n半角数字でご入力ください：");

			fflush(stdin); // 文字入力
			fgets(sel, 2, stdin); // 文字入力
			int_sel=atoi(sel);
			if((strtol(sel,&e,0)==0) || (int_sel>3 || int_sel<1)){
				printf("\n1～3の中から入力してください。\n");
				continue;
			}
			else{
				break;
			}
		}

		switch(int_sel){
			case 1:	//直接指定を選択した場合
				direct_sel(zaseki);
				break;
		}

		printf("予約する座席は\n");
		printf("1.%s\n", zaseki1);
		if(zaseki2[0]!='0'){	//2席目の予約をした場合
			printf("2.%s\n", zaseki2);
		}
		if(zaseki3[0]!='0'){	//3席目の予約をした場合
			printf("3.%s\n", zaseki3);
		}
		if(zaseki4[0]!='0'){	//4席目の予約をした場合
			printf("4.%s\n", zaseki4);
		}
		if(zaseki5[0]!='0'){	//5席目の予約をした場合
			printf("5.%s\n", zaseki5);
		}

		while(1){
			printf("です。この座席でよろしいですか？\n半角数字で入力してください。\n--------------------\n①はい\n②いいえ\n回答：");
			fflush(stdin); // 文字入力
			fgets(check, 2, stdin); // 文字入力
			int_check=atoi(check);
			if(strtol(check,&e,0)==0){
				printf("\n①か②の中から入力してください。\n");
				continue;
			}
			else if(int_check > 2 || int_check < 1){
				printf("\n①か②の中から入力してください。\n");
				continue;
			}
			else{
				break;
			}
		}
		if(int_check==2){ //予約する座席が間違いありの場合実行する
			printf("\n再入力お願いします。\n");
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
	int i; //ループ変数

	printf("予約する座席を半角英数字で入力してください。尚、複数座席がある場合は入力座席ごとにEnterを押してください。\n");

	for(i=0;i<num;i++)
	{
		book_app("configration/zaseki.txt"); // 関数呼び出し(引数は、入力ファイルと出力ファイル)
	}
	toosi_make=0; // 座席予約をするためにbook_appを実行している時に、必要な処理だけをさせるためにtoosi_makeを+1

	return;
}

void book_app(char *in_filename){

	listelement *listpointer;
	listelement *listpointer2;

	char ch,ref_name[5], data[5];
    int i, flag = 0,check1;

	if(toosi_make==1){ // 個別予約情報の通し番号に関する処理じゃない時
		ifp = fopen(in_filename, "a+");

		if((ifp2=fopen("configration/seat_list.txt","r"))==NULL){
			printf("入力ファイルを開けませんでした。\n");
			return;
		}

		listpointer = NULL;
		listpointer2 = NULL;
	}
	else{ // 個別予約情報の通し番号に関する処理の時
		ifp = fopen(in_filename, "a+");
	}

	while(1){
		if(toosi_make==1){
			printf("座席：");
			fflush(stdin); // 文字入力
		    fgets(data, 5, stdin); // 文字入力

			for(i=1; i<4; i++){ // 4,3,2文字の場合、空き部分に\nが入るので、それを\0で上書きし、文字列の最後に\nが入らないようにする
				if(data[i]=='\n'){
					data[i]='\0';
				}
			}
		}

		while((ch = fgetc(ifp)) != EOF){ // EOFになるまでchに代入
			if(flag==0){ // 文字が一文字でも存在した場合、flagを1にする
				flag=1;
			}

			if(ch == MARK){
				for(i = 0; (ref_name[i] = fgetc(ifp)) != MARK; i++); // バックスラッシュまでref_nameに出力
				ref_name[i] = '\0';
				if(toosi_make==1){
					if(judge == 0){ // 一回目の入力かどうか
						listpointer = AddItem (listpointer, ref_name);
						judge = 1; //次からはelse ifにするためにjudgeを1に
					}
					else if(SearchItem (listpointer, ref_name)!=same && judge == 1){  // listpointerの中に今現在のref_nameの文章がない、かつ、二回目以降の入力かどうか
						AddItem (listpointer, ref_name);
					}
				}
			}
		}

		if(toosi_make==1){
			while((ch = fgetc(ifp2)) != EOF){ // EOFになるまでchに代入
				if(ch == MARK){
					for(i = 0; (ref_name[i] = fgetc(ifp2)) != MARK; i++); // バックスラッシュまでref_nameに出力
					ref_name[i] = '\0';

					if(judge2 == 0){ // 一回目の入力かどうか
						listpointer2 = AddItem (listpointer2, ref_name);
						judge2 = 1; //次からはelse ifにするためにjudgeを1に
					}
					else if(judge2 == 1){  // listpointerの中に今現在のref_nameの文章がない、かつ、二回目以降の入力かどうか
						AddItem (listpointer2, ref_name);
					}
				}
			}

			check1=SearchItem (listpointer, data); // 入力データとファイル内データ(予約済み座席)の比較
			SearchItem (listpointer2,data);

			if(check1==500&&list==1){ // 予約しようとしている座席が、zaseki.txtに存在していない、かつ、zaseki_list.txtに座席が存在した場合
				fprintf (ifp,"/%s/",data); // 予約座席をファイルに出力

				if(count==1){ // 一つ目の座席
					strcpy(zaseki1, data); // 予約座席を他の関数で使うので、グローバル変数zaseki1[5]に代入
				}
				else if(count==2){// 二つ目の座席
					strcpy(zaseki2, data); // 予約座席を他の関数で使うので、グローバル変数zaseki2[5]に代入
				}
				else if(count==3){ // 三つ目の座席
					strcpy(zaseki3, data); // 予約座席を他の関数で使うので、グローバル変数zaseki3[5]に代入
				}
				else if(count==4){ // 四つ目の座席
					strcpy(zaseki4, data); // 予約座席を他の関数で使うので、グローバル変数zaseki4[5]に代入
				}
				else if(count==5){ // 五つ目の座席
					strcpy(zaseki5, data); // 予約座席を他の関数で使うので、グローバル変数zaseki5[5]に代入
				}
				count++; // 何回目の予約座席かのカウント

				printf("座席の予約完了\n");
				fclose(ifp2);
				break;
			}else{
				printf("この座席は予約することができません。\nもう一度入力してください\n");
				continue;
			}
		}
		else{ // 個別予約情報の通し番号に関する処理
			if(flag==0){ // zaseki_info.txtに何も書かれていなければ
				ref_name[0]='0';
				ref_name[1]='\0';
			}
			int_ref_name=atoi(ref_name); // 文字列の値が入っているので、整数にする
			int_ref_name++; // 通し番号を+1する

			fprintf(ifp, "/%d/", int_ref_name); // ファイル出力(※fprintfの中で++しても無効になる)
			break;
		}
	}

	fclose(ifp);

	return;
}

void input_res(char kouen[], int zaseki){

	char address[30], name[20], tel[12], select[2]; //住所、氏名、電話番号、正しい数値が入力されたかを示す
	char *e;
	int int_select;

	while(1){
		printf("\n住所・氏名・電話番号を入力してください。\n");

		printf("住所：");
		fflush(stdin); // 文字入力
		fgets(address, 30, stdin); // 文字入力
		printf("氏名：");
		fflush(stdin); // 文字入力
		fgets(name, 20, stdin); // 文字入力
		printf("電話番号(半角数字でハイフンなし)：");
		fflush(stdin); // 文字入力
		fgets(tel, 12, stdin); // 文字入力

		printf("--------------------\n");
		while(1){
			printf("①間違いなし\n②間違いあり\n半角数字で入力してください：");
			fflush(stdin); // 文字入力
			fgets(select, 2, stdin); // 文字入力
			int_select=atoi(select);
			if(strtol(select,&e,0)==0){
				printf("\n①か②の中から入力してください。\n");
				continue;
			}
			else if(int_select > 2 || int_select < 1){
				printf("\n①か②の中から入力してください。\n");
				continue;
			}
			else{
				break;
			}
		}
		if(int_select==2){
			printf("\n再入力お願いします。\n");
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
	char pass_moto[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // パスワードの基となる文字列

	FILE *ofp_info;

	book_app("configration/zaseki_info.txt");

	/* ロケールを日本に設定 */
	setlocale(LC_TIME, "Japanese");
	/* 現在の暦時刻を取得 */
	time(&t);
	/* 詳細時刻に変換 */
	tp = localtime(&t);
	strftime(calender, sizeof(calender),"予約日時：%x\n", tp);
	/* 画面に表示 */
	printf("\n********************\n予約情報\n\n");
	printf("%s\n", calender);
	printf("公演名：%s\n", concert_program[atoi(kouen) - 1]);
	printf("予約座席数：%d\n", zaseki);
	printf("予約座席番号：%s", zaseki1);

	if(zaseki2[0]!='0'){ // 二つ目の座席が入力されている場合
		printf(",%s", zaseki2);
	}
	if(zaseki3[0]!='0'){ // 三つ目の座席が入力されている場合
		printf(",%s", zaseki3);
	}
	if(zaseki4[0]!='0'){ // 四つ目の座席が入力されている場合
		printf(",%s", zaseki4);
	}
	if(zaseki5[0]!='0'){ // 五つ目の座席が入力されている場合
		printf(",%s", zaseki5);
	}
	printf("\n");
	printf("予約情報通し番号：");
	printf("%d\n",int_ref_name);

	printf("パスワード：");
	for(i=0; i<4; i++){
		pass[i]=pass_moto[GetRandom(1,52)%(52)]; // ランダムな文字列のパスワードを生成
	}
	pass[i]='\0';

	printf("%s", pass);
	printf("\n");

	printf("\n予約者の\n");
	printf("名前：%s\n", name);
	printf("住所：%s\n", address);
	printf("電話番号：%s\n", tel);
	printf("********************\n");

	sprintf(filename, "reservation_infomation/%04d-%s_info.txt", int_ref_name, pass);

	ofp_info=fopen(filename, "w");
	fprintf(ofp_info, "%s\n", calender);
	fprintf(ofp_info, "公演名：%s\n", concert_program[atoi(kouen) - 1]);
	fprintf(ofp_info, "予約座席数：%d\n", zaseki);
	fprintf(ofp_info, "予約座席番号：%s", zaseki1);
	if(zaseki2[0]!='0'){ // 二つ目の座席が入力されている場合
		fprintf(ofp_info, ",%s", zaseki2);
	}
	if(zaseki3[0]!='0'){ // 三つ目の座席が入力されている場合
		fprintf(ofp_info, ",%s", zaseki3);
	}
	if(zaseki4[0]!='0'){ // 四つ目の座席が入力されている場合
		fprintf(ofp_info, ",%s", zaseki4);
	}
	if(zaseki5[0]!='0'){ // 五つ目の座席が入力されている場合
		fprintf(ofp_info, ",%s", zaseki5);
	}
	fprintf(ofp_info, "\n");
	fprintf(ofp_info, "予約情報通し番号：%d\n", int_ref_name);
	fprintf(ofp_info, "パスワード：%s\n", pass);

	fprintf(ofp_info, "\n予約者の\n");
	fprintf(ofp_info, "名前：%s\n", name);
	fprintf(ofp_info, "住所：%s\n", address);
	fprintf(ofp_info, "電話番号：%s\n", tel);

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

    listelement *lp = listpointer; // listpointerをlpに代入
	char *newdata; // additional char*
	newdata = (char *) malloc(strlen(data)+1); // memory for string
	strcpy( newdata, data ); // copy string from data to newdata

	if (listpointer != NULL) { // listpointerの中にデータがあった場合
	while (listpointer -> link != NULL) // データがない所まで回す
	    listpointer = listpointer -> link; // 次のリストを指定
	listpointer -> link = (struct listelement *) malloc (sizeof (listelement)); // データサイズ確保
	listpointer = listpointer -> link; // 新たに作ったlinkを新たに作った最後のリストの先頭を指すようにする
	listpointer -> link = NULL; // 最後のリストのアドレス部にNULLを入れる
	listpointer -> dataitem = newdata; // データ部に新たなデータを追加
	return lp; // 新たなデータを追加したlistpointerを返す
    }
    else { // 一つもデータが入っていなかったら
	listpointer = (struct listelement  *) malloc (sizeof (listelement)); // listpointerに必要なデータサイズ確保
	listpointer -> link = NULL; // リンク部にNULLを入れる
	listpointer -> dataitem = newdata; // データ部に新たなデータを追加
	return listpointer; // 一個のデータしか入っていないlistpointerを返す
    }
}


int SearchItem( listelement*listpointer, char *data )
{
	int i= 0;

	same=0; // sameをリセット

	while ( listpointer!= NULL){/* キューの最後まで*/
		i++;/* カウントアップ*/
		if ( strcmp( data, listpointer -> dataitem ) == 0 ) { /* 文字列比較*/
			same = i; // listpointerの中に比較対象データが存在したかどうかの判断に使用(book_app関数内のif文で使用)
			list=1;
			return i;
		}

		else {/* 見つからない時は*/
			listpointer= listpointer-> link;/* リンクをたどって*/
		}
	}
	return 500;/* 最後まで見つからなかったらiを返す*/
}


void res_pass_input(void){
	int i, int_toosi_num;
	char pass_num[5], toosi_num[301];
	char *e;

	while(1){
		printf("\n通し番号をを入力してください\n");
		fflush(stdin); // 文字入力
	    fgets(toosi_num, 5, stdin); // 文字入力
		int_toosi_num=atoi(toosi_num);
		if((strtol(toosi_num,&e,0)==0) || (int_toosi_num>301 || int_toosi_num<1)){
			printf("\n1から301の中から入力してください。\n");
			continue;
		}
		else{
			break;
		}
	}
	for(i=1; i<4; i++){ // 4,3,2文字の場合、空き部分に\nが入るので、それを\0で上書きし、文字列の最後に\nが入らないようにする
		if(toosi_num[i]=='\n'){
			toosi_num[i]='\0';
		}
	}
	while(1){
		printf("\nパスワードを入力してください\n");
		fflush(stdin); // 文字入力
	    fgets(pass_num, 5, stdin); // 4文字以内の文字入力
		for(i=1; i<4; i++){ // 4,3,2文字の場合、空き部分に\nがあった場合
			if(pass_num[i]=='\n'){
				printf("\n4文字のパスワードを入力してください\n");
				continue;
			}
		}
		if(strtol(pass_num,&e,0)!=0){
			printf("\n文字のみのパスワードを入力してください\n");
			continue;
		}
		break;
	}
	sprintf(filename, "%04s-%s_info.txt", toosi_num, pass_num); // 個別の予約情報ファイル名を作成

	return;
}

void Clear(void){
  	if( remove( filename ) == 0 ){ // ファイルが存在し、削除できる場合

    	printf( "予約情報を削除しました\n");
	}
  	else{
    	printf( "予約情報削除に失敗しました\n入力内容を確認し、再度ご入力ください\n");
  	}
	return;
}

void Confirmation(void){
	char itigyou_output[30];

	if((ifp = fopen(filename, "r"))==NULL){
		printf("入力された予約情報は存在しませんでした。\n入力内容を確認し、再度ご入力ください\n");
		return;
	}
	while(fgets(itigyou_output, 29, ifp)!=NULL){ //一行ずつ画面に表示
		printf("%s", itigyou_output);
	}

	fclose(ifp);

	return;
}
