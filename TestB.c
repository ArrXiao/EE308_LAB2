#include <stdio.h>

int main(){
	int i=-1;
    if(i<0){
        if(i<-1) i++;
        else i++;
	}else if(i>0){
        if (i>2){}
        else if (i==2) 
        	if(i!=0) i++;
        	else if(i>2)
        		if(i>5) i++;
        		else i++;
        else i++;
    }
    return 0;
}
