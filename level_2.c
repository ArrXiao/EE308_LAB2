#include <stdio.h>
#include <string.h>
#define Key_Num 32 //C���Թ���32���ؼ���
#define Len_Min 2 //��̵ĳ���Ϊ2
#define Len_Max 8 //��ĳ���Ϊ8
const char Key[Key_Num][Len_Max+1]={
"auto","break","case","char","const","continue","default","do","double",
"else","enum","extern","float","for","goto","if","int","long","register",
"return","short","signed","sizeof","static","struct","switch","typedef",
"union","unsigned","void","volatile","while"
};

int count[Key_Num];//����ؼ��ֳ��ֵĴ��� 
char input[Len_Max+1];
int Case[100];

void CheckKeyWord(){//�Ƚ��Ƿ�Ϊ�ؼ��� 
	int i,result;
	static int case_num=0;
	for(i=0;i<Key_Num;i++){
		result=strcmp(input,Key[i]);
		if(result==0&i==25&count[i]!=0)case_num++;//level_2�ж�Ϊ�ڼ���switch 
		if(result==0&i==2)Case[case_num]++;//level_2�ж���һ��swich��case���� 
		if(result==0){
			count[i]++;
			break;
		}
		if(result<0)
			break;
	}
}

int IsKeyLetter(char c){//�ж��Ƿ�ΪСд��ĸ 
	return c>='a' && c<='z';
}
int IsOtherLetter(char c){//�ж��Ƿ�Ϊ��д��ĸ�������� 
	return c>='A' && c<='Z' || c>='0' && c<='9' || c=='_';
}

enum MY_STATE{
	INVAILD,//��ʼʱ
	IN_QUOTE,//������
	IN_QUOTE_TRANS,//�����ڶ���ת���
	IN_WORD,//�п����ǹؼ���
	NOT_KEY,//���ǹؼ���
	COMMENT_1,//��һ��ע�ͷ�(/)
	COMMENT_LINE,//��ע��
	COMMENT_BLOCK,//��ע��
	COMMENT_BLOCK_1//��ע���ڶ���'*'
};

int main(){
	FILE *fi;
	int i;
	enum MY_STATE state;
	fi=fopen("a.c","r");//����Ҫʵ����ļ� 
	if(fi==NULL) return 1;
	i=0;
	state=INVAILD;
	memset(count,0,sizeof(count));
	memset(Case,0,sizeof(Case));//��ʼ�� 
	while(fscanf(fi,"%c",&input[i])==1){// ��ʽ����ʽ��ȡ 
		switch(state){
			case INVAILD:
			if(IsKeyLetter(input[i])){
				state=IN_WORD;
				i++;
			}
			else if(IsOtherLetter(input[i]))//�ж��Ƿ�ΪСд��ĸ 
				state=NOT_KEY;
			else if(input[i]=='\"')//�ж����� 
				state=IN_QUOTE;
			else if(input[i]=='/')//�ж�ע�� 
				state=COMMENT_1;
			break;
		
			case IN_QUOTE:
			if(input[i]=='\\')
				state=IN_QUOTE_TRANS;
			else if(input[i]=='\n')//����©���ţ��������
				state=INVAILD;
			break;
		
			case IN_QUOTE_TRANS:
			if(input[i]=='\n')//����©���ţ��������
				state=INVAILD;
			else
				state=IN_QUOTE;
			break;
			
			case IN_WORD:
			if(IsKeyLetter(input[i])){
				if(i<Len_Max)
					i++;
				else{
				i=0;
				state=NOT_KEY;
				}
			}
			else if(IsOtherLetter(input[i])){
				i=0;
				state=NOT_KEY;
			}
			else{
				input[i]='\0';
				CheckKeyWord();
				i=0;
			if(input[i]=='\"')
				state=IN_QUOTE;
			else if(input[i]=='/')
				state=COMMENT_1;
			else
				state=INVAILD;
			}
			break;
			
			case NOT_KEY:
			if(input[i]=='\"')
				state=IN_QUOTE;
			else if(input[i]=='/')
				state=COMMENT_1;
			else if(!IsKeyLetter(input[i]) && !IsOtherLetter(input[i]))
				state=INVAILD;
			break;
			
			case COMMENT_1:
			if(input[i]=='/')
				state=COMMENT_LINE;
			else if(input[i]=='*')
				state=COMMENT_BLOCK;
			else if(IsKeyLetter(input[i])){
				state=IN_WORD;
				i++;
			}
			else if(IsOtherLetter(input[i]))
				state=NOT_KEY;
			else
				state=INVAILD;
			break;
			
			case COMMENT_LINE:
			if(input[i]=='\n')
				state=INVAILD;
			break;
			
			case COMMENT_BLOCK:
			if(input[i]=='*')
				state=COMMENT_BLOCK_1;
			break;
			
			case COMMENT_BLOCK_1:
			if(input[i]=='/')
				state=INVAILD;
			else
				state=COMMENT_BLOCK;
			break;
		}
	}
	fclose(fi);
	
	fi=fopen("keyword.txt","w");
	if(fi==NULL) return 1;
	int total=0;
	for(i=0;i<Key_Num;i++){
		if(count[i]!=0)total=total+count[i];
	}
		fprintf(fi,"%s: %d\n","total num",total);
		fprintf(fi,"%s num: %d\n",Key[25],count[25])//level_2����swich��case 
		fprintf(fi,"%s num: ",Key[2]); 
	for(i=0;i<count[25];i++){
		fprintf(fi,"%d ",Case[i]);
		if(i==count[25])fprintf(fi,"%d\n",Case[i]);//level_2����swich��case
	}
	fclose(fi);
	return 0;
	}
