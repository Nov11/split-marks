#include "dam.h"
// typedef struct{
	// ELE* node;
	// ELE* next;
// }ELEHEAD;

void asStr(char** name, char* str)
{
	int len;
	char* p;
	
	len = strlen(str);
	if((p = (char*)malloc(len * sizeof(char))) == NULL){
		perror("malloc");
		exit(1);
	}
	strcpy(p, str);
	*name = p;
}

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
		printf("|34%c|\n", *p);
		if(p == str)break;
		while(p < pend){
			*q = *p;
			q++;
			p++;
		}
		*q = '\0';
		pend = q;
		q = str;
		p = str;
	}
}

void trav(ELE* p){
	ELE* nptr;
	ATTNODE* attptr;
	
	while(p != NULL){
		nptr = p;
		p = p->next;
		
		printf("name:%s|", nptr->name);
		attptr = nptr->attnode;
		while(attptr != NULL){
			printf("att name:%s ", attptr->name);
			attptr = attptr->next;
		}
		printf("\n");
	}
}

void loadDTD(ELE** phead, FILE* fp){
	char buff[BSIZE] = {0};
	char* p;
	ELE* nptr;
	ATTNODE* attptr;
	
	while(fgets(buff, BSIZE, fp) != NULL){
		// printf("%s", buff);
		p = strtok(buff, BLANK);
		// printf("1st blank:%p %s\n", p, p);
		if(strcmp(p, ELBEG) != 0){
			printf("not a legal element!\n");
			exit(1);
		}
		// printf("name %s ", );
		p = strtok(NULL, ATTBG);
		if((nptr = (ELE*)malloc(sizeof(ELE))) == NULL){
			perror("malloc");
			exit(1);
		}
		nptr->next = NULL;
		nptr->attnode = NULL;
		asStr(&nptr->name, p); 
		// printf("nptr:%s\n", nptr->name);
		nptr->next = *phead;
		*phead = nptr;
		
		
		p = strtok(NULL, ATTED);//delete the ")"
		p = strtok(p, ",");//get the first *** where ***,***....
		do{// printf("atrri %s\n", p = strtok(NULL, ATTED));
			
			if((attptr = (ATTNODE*)malloc(sizeof(ATTNODE))) == NULL){
				perror("malloc");
				exit(1);
			}
			attptr->next = NULL;
			asStr(&attptr->name, p);
			attptr->next = nptr->attnode;
			nptr->attnode = attptr;
		}while((p = strtok(NULL, ",")));
		
	}
}

int lookUpNodeNameInDTD(ELE* phead, char* str)
{
	ELE* p;
	
	p = phead;
	while(p){
		if((strcmp(p->name, str)) == 0){
			return FOUND;
		}else{
			p = p->next;
		}
	}
	
	return NOTFOUND;
}

int lookUpNodeAttPCDATAInDTD(ELE* phead, char* str){
	ELE* p;
	ATTNODE* attptr;
	
	p = phead;
	while(p){
		if((strcmp(p->name, str)) == 0){
			attptr = p->attnode;
			while(attptr){
				if((strcmp(attptr->name, "#PCDATA")) == 0){
					return FOUND;
				}else{
					attptr = attptr->next;
				}
			}	
		}
		p = p->next;
	}
	
	return NOTFOUND;
}

int lookUpNodeAttInDTD(ELE* phead, char* nodename, char* attname){
	ELE* p;
	ATTNODE* attptr;
	
	p = phead;
	while(p){
		if((strcmp(p->name, nodename)) == 0){
			attptr = p->attnode;
			while(attptr){
				if((strcmp(attptr->name, attname)) == 0){
					return FOUND;
				}else{
					attptr = attptr->next;
				}
			}	
		}
		p = p->next;
	}
	
	return NOTFOUND;
}

void parseXML(ELE* phead, XMLNODE** tr, char* filename)
{
	FILE* fp;
	char buff[BSIZE] = {0};
	char* p;
	XMLNODE* xptr;
	
	
	if((fp = fopen(filename, "r")) == NULL){
		perror("fopen");
		exit(1);
	}
	
	
	
	//while
	while((fgets(buff, BSIZE, fp)) != NULL){
		// printf("the line : %s\n", buff);
		if((strcmp(buff, XMLHD) == 0)){
			// printf("encouter xml header go over it\n");
			continue;
		}
		
		if((strncmp(buff, DTDHD, strlen(DTDHD)) == 0)){
			// printf("encounter dtd header check filename\n");
			strtok(buff, " ");
			p = strtok(NULL, " ");
			if((strcmp(p, DTDRT)) == 0){			//if it's not the rootname it's not using our dtd file
				// printf("use our dtd file only\n");
				continue;
			}else{
				printf("not our dtd prog exit\n");
				exit(1);
			}
		}

		//trimtab(buff);
		// p = strtok(buff, "<");
		// printf("aaa|%s\n", p);
		// p = strtok(NULL, ">");
		// printf("aaa%s|\n", p);
		p = buff;
		
		if(*p == '<'){ 
			p = strtok(buff, ">");
			if(*p == '<')p++;
		}else{
			strtok(buff, "<");
			p = strtok(NULL, ">");
		}
		//buildNode(p)
		// printf("node name:%s\n", p);//return;
		if(*p == '/'){
			p++;
			XMLNODE* popxptr;
			pop(&popxptr);
			// printf("popxptr:%s\n", popxptr->name);
			if(strcmp(popxptr->name, p) == 0){
				// printf("%s closeed\n", p);
				*tr = popxptr;
				continue;
			}else{
				printf("inlegal nesting\n");
				exit(1);
			}
		}
		if((xptr = (XMLNODE*)malloc(sizeof(XMLNODE))) == NULL){
			perror("malloc");
			exit(1);
		}
		xptr->next = NULL;
		xptr->subnodelist = NULL;
		xptr->value = 0;
		
		if(lookUpNodeNameInDTD(phead, p) == NOTFOUND){
			printf("the name not exits progs termned\n");
			exit(1);
		}
		asStr(&xptr->name, p);
		// printf("xptr->name:%s\n", xptr->name);
		if(lookUpNodeAttPCDATAInDTD(phead, p) == FOUND){
			char* pend;
			p = strtok(NULL, "<");
			pend = strtok(NULL, ">");
			if(*pend != '/'){
				printf("not a </**> stlye\n");
				exit(1);
			}
			pend++;
			// printf("p:%s pend:%s\n", p, pend);
			if(strcmp(xptr->name, pend) != 0){
				printf("not stick to our rule, one line a element pair\n");
				exit(1);
			}
			asStr(&xptr->value, p);
			XMLNODE* popxptr;
			pop(&popxptr);
			// xptr->subnodelist = popxptr->subnodelist;
			// popxptr->subnodelist = xptr;
			xptr->next = popxptr->subnodelist;
			popxptr->subnodelist = xptr;
			push(popxptr);
			continue;
		}
		//push stack
		if(top != 0){
			XMLNODE* ppppp;
			pop(&ppppp);
			xptr->next = ppppp->subnodelist;
			ppppp->subnodelist = xptr;
			push(ppppp);
			// printf("xptr:%s into father:%s\n", xptr->name, ppppp->name);
		}
		push(xptr);
		int ttop = top - 1;
		while(ttop > -1){
			// printf("stack %d, name:%s ", ttop, stack[ttop]->name);
			XMLNODE* xcv = stack[ttop]->subnodelist;
			while(xcv != NULL){
				// printf("subnode:%s ", xcv->name);
				xcv = xcv->next;
			}
			// printf("\n");
			ttop--;
		}
		// printf("push\n");
		
		// printf("pushed %s\n", ppppp->name);
		// printf("name of the node :%s and its attri %s|\n", xptr->name, xptr->value);
		
			
		
	}
	// printf("top:%d\n", top);
	if(fclose(fp) != 0){
		perror("fclose");
		exit(1);
	}
}

void transxml(XMLNODE* tr){
	XMLNODE* p;
	XMLNODE* list;
	int count = 0;
	// int maxcount = 0;
	// int endpos = 0;
	// int level = 0;
	
	push(tr);
	while(top > 0){
		pop(&p);
		printf("level:%d\n", count++);
		printf("trans name:%s |", p->name);
		if(p->value != NULL){
			printf("value:%s\n", p->value);
		}else{
			printf("\n");
		}
		list = p->subnodelist;
		while(list){
			push(list);
			list = list->next;
		}
	}
	// inqu(tr);
	// endpos = tail;
	// while(!quempty()){
		// dequ(&p);
		// printf("trans name:%s |", p->name);
		// if(p->value != NULL){
			// printf("value:%s\n", p->value);
		// }else{
			// printf("\n");
		// }
		// list = p->subnodelist;
		// while(list){
			// inqu(list);
			// count++;
			// list = list->next;
		// }
		// if(endpos == head){
			// if(maxcount < count){
				// maxcount = count;
			// }
			// count = 0;
			// endpos = tail;
			// level++;
		// }
	// }		
	// printf("maxcount:%d level: %d \n", maxcount, level);
}

void printTree(XMLNODE* tr)//, int maxcount, int level)
{
	XMLNODE* p;
	XMLNODE* list;
	int tab = 0;
	int s[20] = {0};
	int level;
	
	push(tr);s[tab++] = 0;
	while(top > 0){
		pop(&p);level = s[--tab];
		int i;
		for(i = 0; i < level; i++){
			int k = 0;
			if(i == level - 1){
				printf("|-------");
				continue;
			}
			int c;
			for(c = 0; c < top; c++){
				if(s[c] == i + 1){
				//if(s[0] - 1 == i)
				printf("|       ");
				k = 1;
				break;//continue;
				}
			}
			if(k == 0)printf("        ");
			
			// }else if(i < c){
				// printf("|       ");
			// } else{
				// printf("aaaaaaaa");
			// }
		}
		
		printf("%s", p->name);
		if(p->value != NULL){
			printf(":%s\n", p->value);
		}else{
			printf("\n");
		}
		list = p->subnodelist;
		while(list){
			push(list);s[tab++] = level + 1;
			list = list->next;
		}
	}
	
}

int main(int argc, char** argv)
{
	// int fd;
	FILE* fp;
	ELE* phead;
	XMLNODE* tr;
	
	if(argc < 2){
		printf("need a file !\n");
		exit(1);
	}
	
	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fopen");
		exit(1);
	}
	
//	printf("the file name:%s\n", argv[1]);
	// if((phead = (ELEHEAD*)malloc(sizeof(ELEHEAD))) == NULL){
		// perror("mallc");
		// exit(1);
	// }
	// phead->next = NULL;
	phead = NULL;
	loadDTD(&phead, fp);
	
	//trav(phead);
	
	parseXML(phead, &tr, "xwow.xml");
	// transxml(tr);
	printTree(tr);
	
	parseXML(phead, &tr, "xwow2.xml");
	// transxml(tr);
	printTree(tr);
	if(fclose(fp) != 0){
		perror("fclose");
		exit(1);
	}
	return 0;
}
