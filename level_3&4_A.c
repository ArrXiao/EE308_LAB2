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

int if_else_num[2]={0,0};
int if_elsenum=0;
int if_elseif_elsenum=0;
int count[Key_Num];//����ؼ��ֳ��ֵĴ��� 
char input[Len_Max+1];
int Case[100];
//����ջ
typedef struct{
	int data[100];
	int top;
	int bottom;
}stack;

//����ջ
stack *StackCreate(){
	stack *p=(stack*)malloc(sizeof(stack));//�����¿ռ� 
	if(p==NULL)//����ʧ�� 
	return 0;
	p->bottom=p->top=0;//����ɹ� 
	return p;
}

//��ջ
void StackInput(stack *p,int num){
	p->data[p->top]=num;//����ջ�� 
	p->top++;//ջ��ָ���1 
} 

//��ջ 
void StackOutput(stack *p){
	if(p->top!=p->bottom){//ջ�ǿ� 
		p->top--;//ջ����1 
	}
}


void CheckKeyWord(){//�Ƚ��Ƿ�Ϊ�ؼ��� 
	int i,result;
	static int case_num=0;
	for(i=0;i<Key_Num;i++){
		result=strcmp(input,Key[i]);
		if(result==0&i==25&count[i]!=0)case_num++;//level_2�ж�Ϊ�ڼ���switch 
		if(result==0&i==2)Case[case_num]++;//level_2�ж���һ��swich��case���� 
		if(result==0&i==15)if_else_num[0]++;//�ж�if���� 
		if(result==0&i==9)if_else_num[1]++;//�ж�else����
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
void stackfunc(stack *p){//�ж��Ƿ�ջ���ݰ���else if
	int comp=0;
	if(if_else_num[0]==1&&if_else_num[1]==0){//�����if��ѹջ1
		if_else_num[0]=0;
		StackInput(p,1);
	}
	if(if_else_num[0]==1&&if_else_num[1]==1){//�����else if��ѹջ3
		if_else_num[0]=0;
		if_else_num[1]=0;
		StackInput(p,3);
	}
	if(if_else_num[0]==0&&if_else_num[1]==1){//�����else����ջֱ������1
		while (p->data[p->top-1]!=1){
            if (p->data[p->top-1]==3){
                comp=1;
            	StackOutput(p);
        	}
    	}
    	StackOutput(p);
    	if_else_num[1]=0;
    	if(comp==1)if_elseif_elsenum++;
		if(comp==0)if_elsenum++;
	}
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
	stack *p;
	p=StackCreate();
	enum MY_STATE state;
	fi=fopen("a.c","r");//����Ҫʵ����ļ� 
	if(fi==NULL) return 1;
	i=0;
	state=INVAILD;
	memset(count,0,sizeof(count));
	memset(Case,0,sizeof(Case));//��ʼ��
	while(fscanf(fi,"%c",&input[i])==1){// ��ʽ����ʽ��ȡ
		if(input[i]==0x0A&&(if_else_num[0]!=0||if_else_num[1]!=0))stackfunc(p);//����ջ���� 
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
	int total=0;
	for(i=0;i<Key_Num;i++){
		if(count[i]!=0)total=total+count[i];
	}
		printf("%s: %d\n","total num",total);
		printf("%s num: %d\n",Key[25],count[25]);//level_2����swich��case 
		printf("%s num: ",Key[2]); 
	for(i=0;i<count[25];i++){
		if(i!=count[25]-1)printf("%d ",Case[i]);
		if(i==count[25]-1)printf("%d\n",Case[i]);//level_2����swich��case
	}
	printf("if_else num:%d\n",if_elsenum);//level_3ͳ��if_else����
	printf("if_elseif_else num:%d\n",if_elseif_elsenum);//level_4ͳ��if_else_if����

	return 0;
	}
