#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
}


void rotate(int b[4][4]){
	/* user code */
	
	int a[4][4]={0}; //temp b
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			a[i][j] = b[3-j][i];
		}
	}
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			b[i][j] = a[i][j];
		}
	}

	// user code fin
}

int set_board(int dir, int b[4][4]){

	/* user code init */

	int a[4][4]; // temp b
	int change=0; //변화가 있으면 1, 없으면 0
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			a[i][j] = b[i][j];
		}
	}
	//배열 회전
	for(int i=0; i<dir; i++){
		rotate(b);
	}
	//우측이 0일 때 옆으로 이동 *3
	for(int n=0; n<3; n++){
		for(int i=0; i<4; i++){
			for(int j=0; j<3; j++){
				if(b[i][j] && b[i][j+1]==0){
					b[i][j+1] = b[i][j];
					b[i][j] = 0;
				}
			}
		}
	}
	for(int i=0; i<4; i++){
		//0==1이고 2==3 일때 1,3에 저장
		if(b[i][0]==b[i][1] && b[i][2]==b[i][3]){
			b[i][1] += b[i][0];
			b[i][0] = 0;
			b[i][3] += b[i][2];
			b[i][2] = 0;
		}
		//서로 붙어있는 두 값이 같을 때 더함
		else{
			for(int j=3; j>0; j--){
				if(b[i][j]==b[i][j-1]){
					b[i][j] += b[i][j-1];
					b[i][j-1] = 0;
					break;
				}
			}
		}
	}
	//우측이 0일 때 옆으로 이동*3
	for(int n=0; n<3; n++){
		for(int i=0; i<4; i++){
			for(int j=0; j<3; j++){
				if(b[i][j] && b[i][j+1]==0){
					b[i][j+1] = b[i][j];
					b[i][j] = 0;
				}
			}
		}
	}
	//배열 원래대로 회전
	if(dir){
		for(int i=4-dir; i>0; i--){
			rotate(b);
		}
	}
	//변화 유무 판단
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(a[i][j] != b[i][j]){
				change = 1;
			}
		}
	}
	if(!change){
		return 0;
	}

	// user code fin
	return 1;
}

int is_game_over(){
	/* user code */
	int flag=1; //게임오버 1, 아니면 0
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(b[i][j]==0){
				flag = 0;
				return 0;
			}
		}
	}
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if(b[i][j]==b[i][j+1] || b[i][j]==b[i+1][j]){
				flag = 0;
				return 0;
			}
		}
	}
	for(int i=0; i<3; i++){
		if(b[3][i]==b[3][i+1] || b[i][3]==b[i+1][3]){
			flag = 0;
			return 0;
		}
	}
	if(flag){
		return 1;
	}
	
	// user code fin
	/* if game over return 0, else then return 1 */
	return 0;
}

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */
	
	int sum=0 ;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			sum += b[i][j];
			if(score<b[i][j]){
				score = b[i][j];
			}
		}
	}
	if(sum!=tot){
		printf("tot error!");
	}

	// user code fin
	/* calculate score & check sum of all block equasls variable tot */



	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
}

int make_two_or_four(){
	/* user code */

	int flag_zero=0; // 0 : return 0; 1 : return 2 or 4
	int a[4][4] = {0}; // check 0
	int n = 4; // 2 또는 4 중 어떤걸 저장할 것인지 알려줌
	int row, col;

	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(b[i][j]==0){
				a[i][j] = 1;
				flag_zero = 1;
			}
		}
	}
	if(flag_zero){
		if(rand()%3){ // 2 or 4 결정
			n = 2;
		}
		while(1){
			row = rand()%4;
			col = rand()%4;
			if(a[row][col]){
				break;
			}
		}
		b[row][col] = n;
		return n;
	}

	// user code fin

	/* return 2 or 4 that makes in this times */

	/* if can not make two or four, then return 0 */
	return 0;	
}


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}

int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;

	/* make init board */
	/* user code */

	tot += make_two_or_four();
	tot += make_two_or_four();	


	// user code fin

	draw_board(tot, -1);

	
	do {
		command = GetCommand();

		if (command != -1) {
			if( set_board(command, b) ){

				/* user code */

				tot += make_two_or_four();
				game_over = is_game_over();
				
				// user code fin

				draw_board(tot, command);
			}
		}

	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}