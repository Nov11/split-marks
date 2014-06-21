#include<stdio.h>
#include<string.h>

void trimtab(char* str)
{
	char* p;
	char* q;
	char* pend;
	
	p = str;
	q = str;
	pend = p + strlen(str);
	while(p < pend){
		while(*p == '\t' && p < pend){
			p++;
		}
		printf("|%c|\n", *p);
		if(p == pend)break;
		while(p < pend){
			*q = *p;
			q++;
			p++;
		}
		*q = '\0';
		if(p == pend)break;
		p = str;
	}
}
int main()
{
char s[]="\tab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
char *delim="o";
char *p;
printf("%s|", strtok(s,delim));
while((p=strtok(NULL,delim)))printf("%s|",p);
printf("\n");


char nn[]="<nodes>";
//trimtab(nn);
printf("new nn :%s\n", nn);
p = strtok(nn, "<");
printf("|%s|\n", p);
p = strtok(NULL, ">");
printf("|%s|\n", p);


return 0;
}