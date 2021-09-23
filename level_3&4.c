#include <stdio.h>
#include <string.h>
#define Key_Num 32 //C语言共有32个关键字
#define Len_Min 2 //最短的长度为2
#define Len_Max 8 //最长的长度为8
const char Key[Key_Num][Len_Max+1]={
"auto","break","case","char","const","continue","default","do","double",
"else","enum","extern","float","for","goto","if","int","long","register",
"return","short","signed","sizeof","static","struct","switch","typedef",
"union","unsigned","void","volatile","while"
};

int if_else_num[2]={0,0};// 
int if_elsenum=0;
int if_elseif_elsenum=0;
int count[Key_Num];//储存关键字出现的次数 
char input[Len_Max+1];
int Case[100];
//定义栈
typedef struct{
	int data[100];
	int top;
	int bottom;
}stack;

//创建栈
stack *StackCreate(){
	stack *p=(stack*)malloc(sizeof(stack));//分配新空间 
	if(p==NULL)//分配失败 
	return 0;
	p->bottom=p->top=0;//分配成功 
	return p;
}

//入栈
void StackInput(stack *p,int num){
	p->data[p->top]=num;//存入栈中 
	p->top++;//栈顶指针加1 
} 

//出栈 
void StackOutput(stack *p){
	if(p->top!=p->bottom){//栈非空 
		p->top--;//栈顶减1 
	}
}


void CheckKeyWord(){//比较是否为关键字 
	int i,result;
	static int case_num=0;
	for(i=0;i<Key_Num;i++){
		result=strcmp(input,Key[i]);
		if(result==0&i==25&count[i]!=0)case_num++;//level_2判断为第几个switch 
		if(result==0&i==2)Case[case_num]++;//level_2判断在一个swich下case个数 
		if(result==0&i==15)if_else_num[0]++;//判断if个数 
		if(result==0&i==9)if_else_num[1]++;//判断else个数
		if(result==0){
			count[i]++;
			break;
		}
		if(result<0)
			break;
	}
}

int IsKeyLetter(char c){//判断是否为小写字母 
	return c>='a' && c<='z';
}
int IsOtherLetter(char c){//判断是否为大写字母或者数字 
	return c>='A' && c<='Z' || c>='0' && c<='9' || c=='_';
}

enum MY_STATE{
	INVAILD,//初始时
	IN_QUOTE,//引号内
	IN_QUOTE_TRANS,//引号内读到转义符
	IN_WORD,//有可能是关键词
	NOT_KEY,//不是关键词
	COMMENT_1,//第一个注释符(/)
	COMMENT_LINE,//行注释
	COMMENT_BLOCK,//块注释
	COMMENT_BLOCK_1//块注释内读到'*'
};

int main(){
	FILE *fi;
	int i;
	stack *p;
	p=StackCreate();
	enum MY_STATE state;
	fi=fopen("a.c","r");//打开需要实验的文件 
	if(fi==NULL) return 1;
	i=0;
	state=INVAILD;
	memset(count,0,sizeof(count));
	memset(Case,0,sizeof(Case));//初始化
	while(fscanf(fi,"%c",&input[i])==1){// 格式化方式读取
		if(input[i]==0x0A&&(if_else_num[0]!=0||if_else_num[1]!=0)){
			int comp=0;//判断是否弹栈内容包括else if
			if(if_else_num[0]==1&&if_else_num[1]==0){//如果是if则压栈1
				if_else_num[0]=0;
				StackInput(p,1);
			}
			if(if_else_num[0]==1&&if_else_num[1]==1){//如果是else if则压栈3
				if_else_num[0]=0;
				if_else_num[1]=0;
				StackInput(p,3);
			}
			if(if_else_num[0]==0&&if_else_num[1]==1){
			int k;//如果是else，则弹栈直到弹出1
			for(k=0;k<p->top;k++){
			printf("%d",p->data[k]);
		}
		printf("\n");
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
		switch(state){
			case INVAILD:
			if(IsKeyLetter(input[i])){
				state=IN_WORD;
				i++;
			}
			else if(IsOtherLetter(input[i]))//判断是否为小写字母 
				state=NOT_KEY;
			else if(input[i]=='\"')//判断引号 
				state=IN_QUOTE;
			else if(input[i]=='/')//判断注释 
				state=COMMENT_1;
			break;
		
			case IN_QUOTE:
			if(input[i]=='\\')
				state=IN_QUOTE_TRANS;
			else if(input[i]=='\n')//这里漏引号，编译错误。
				state=INVAILD;
			break;
		
			case IN_QUOTE_TRANS:
			if(input[i]=='\n')//这里漏引号，编译错误。
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
		fprintf(fi,"%s num: %d\n",Key[25],count[25]);//level_2输入swich和case 
		fprintf(fi,"%s num: ",Key[2]); 
	for(i=0;i<count[25];i++){
		if(i!=count[25]-1)fprintf(fi,"%d ",Case[i]);
		if(i==count[25]-1)fprintf(fi,"%d\n",Case[i]);//level_2输入swich和case
	}
	fprintf(fi,"if_else num:%d\n",if_elsenum);
	fprintf(fi,"if_elseif_else num:%d\n",if_elseif_elsenum);
	fclose(fi);
	return 0;
	}
