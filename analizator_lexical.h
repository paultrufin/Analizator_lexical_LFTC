#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define SAFEALLOC(var,Type) if((var=(Type*)malloc(sizeof(Type)))==NULL)err("not enough memory");

enum{END,ID,CT_INT,CT_REAL,CT_CHAR,CT_STRING,COMMA,SEMICOLON,
LPAR,RPAR,LBRACKET,RBRACKET,LACC,RACC,ADD,SUB,MUL,DIV,DOT,AND,
OR,NOT,ASSIGN,EQUAL,NOTEQ,LESS,LESSEQ,GREATER,GREATEREQ,SPACE,
LINECOMMENT,COMMENT,BREAK,CHAR,INT,FLOAT,DOUBLE,LONG,SHORT,WHILE,
DO,IF,ELSE,VOID,STRUCT,UNSIGNED,SWITCH,CASE,FOR,RETURN}AL;

void err(const char *fmt,...)
{
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"error: ");
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

void print_AL_by_code(int code){
	switch(code){
		case 0: printf("END");
				break;
		case 1:	printf("ID");
				break;
		case 2: printf("CT_INT");
				break;
		case 3: printf("CT_REAL");
				break;
		case 4: printf("CT_CHAR");
				break;
		case 5: printf("CT_STRING");
				break;
		case 6: printf("COMMA");
				break;
		case 7: printf("SEMICOLON");
				break;
		case 8: printf("LPAR");
				break;
		case 9: printf("RPAR");
				break;
		case 10: printf("LBRACKET");
				break;
		case 11: printf("RBRACKET");
				break;
		case 12: printf("LACC");
				break;
		case 13: printf("RACC");
				break;
		case 14: printf("ADD");
				break;
		case 15: printf("SUB");
				break;
		case 16: printf("MUL");
				break;
		case 17: printf("DIV");
				break;
		case 18: printf("DOT");
				break;
		case 19: printf("AND");
				break;
		case 20: printf("OR");
				break;
		case 21: printf("NOT");
				break;
		case 22: printf("ASSIGN");
				break;
		case 23: printf("EQUAL");
				break;
		case 24: printf("NOTEQ");
				break;
		case 25: printf("LESS");
				break;
		case 26: printf("LESSEQ");
				break;
		case 27: printf("GREATER");
				break;
		case 28: printf("GREATEREQ");
				break;
		case 29: printf("SPACE");
				break;
		case 30: printf("LINECOMMENT");
				break;
		case 31: printf("COMMENT");
				break;
		case 32: printf("BREAK");
				break;
		case 33: printf("CHAR");
				break;
		case 34: printf("INT");
				break;
		case 35: printf("FLOAT");
				break;
		case 36: printf("DOUBLE");
				break;
		case 37: printf("LONG");
				break;
		case 38: printf("SHORT");
				break;
		case 39: printf("WHILE");
				break;
		case 40: printf("DO");
				break;
		case 41: printf("IF");
				break;
		case 42: printf("ELSE");
				break;
		case 43: printf("VOID");
				break;
		case 44: printf("STRUCT");
				break;
		case 45: printf("UNSIGNED");
				break;
		case 46: printf("SWITCH");
				break;
		case 47: printf("CASE");
				break;
		case 48: printf("FOR");
				break;
		case 49: printf("RETURN");
				break;
	}
}

typedef struct _Token{
	int code; // codul (numele)
	union{
		char *text; // folosit pentru ID, CT_STRING (alocat dinamic)
		long int i; // folosit pentru CT_INT, CT_CHAR
		double r; // folosit pentru CT_REAL
	};
	int line; // linia din fisierul de intrare
	struct _Token *next; // inlantuire la urmatorul AL
}Token;

Token *lastToken=NULL,*tokens=NULL;

Token *addTk(int code,int line){
	Token *tk;
	SAFEALLOC(tk,Token)
	tk->code=code;
	tk->line=line;
	tk->next=NULL;
	if(lastToken){
	lastToken->next=tk;
	}else{
	tokens=tk;
	}
	lastToken=tk;
	return tk;
}

void showAtoms(){
	Token *p=NULL;
	for(p=tokens;p!=NULL;p=p->next)
	{
		//printf("#%p %p\n",p,p->text);
		printf("Line %d:",p->line);
		print_AL_by_code(p->code);
		//printf(" - Code:%d ",p->code);
		if(p->code==ID || p->code==CT_STRING || p->code==CT_CHAR) printf("-> %s ",p->text);
		else if((p->code) == 2) printf("-> %ld ",p->i);
		else if((p->code) == 3) printf("-> %f ",p->r);
		printf("\n");

	}

}


void tkerr(const Token *tk,const char *fmt,...)
{
	va_list va;
	va_start(va,fmt);
	fprintf(stderr,"error in line %d: ",tk->line);
	vfprintf(stderr,fmt,va);
	fputc('\n',stderr);
	va_end(va);
	exit(-1);
}

char * createString(const char *s1, const char *s2)
{

	int len1=strlen(s1);
	int len2=strlen(s2);
	printf("\nlen1:%d\n",len1);
	printf("len2:%d\n",len2);
//printf("In createString : s1: %s -- s2: %s",s1,s2);

	char *string = (char*)malloc(sizeof(char)*(abs(len1-len2))+10000);err("insuficienta memory");

	int i=0;
	for(i=0;i<abs(len1-len2);i++){ //len2-len1 merge dar nu exact
		string[i]=s1[i];//s1[i]+1 merge dar nu corect
	}
	for(i=0;i<strlen(string);i++){
		if(string[i] == '\\') {
			if(string[i+1] == 't') {
				string[i]= '\t';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='\\'){
				string[i]='\\';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='a'){
				string[i]='\a';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='b'){
				string[i]='\b';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='f'){
				string[i]='\f';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='n'){
				string[i]='\n';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='r'){
				string[i]='\r';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='v'){
				string[i]='\v';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i]+1=='\''){
				string[i]='\'';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='?'){
				string[i]='\?';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='"'){
				string[i]='\"';
				strcpy(string+i+1, string+i+2);
			}
			else if(string[i+1]=='0'){
				//tring[i]='\0';
				strcpy(string+i+1, string+i+2);
			}
		}
	}
	//strcpy(string,s1);
	//strcat(string,s2);

	//printf("In createString. Returned value: %s\n",string);
	return string;
	free(string);
}

const char *pCrtCh;
int line=1; 
int getNextToken()
{
	int state=0,nCh;
	char ch;
	const char *pStartCh;
	Token *tk;

	int base=10;
	//const char *pCrtCh;

	while(1){ // bucla infinita
	ch=*pCrtCh;
	//printf("(Stare,caracter)=(%d,%c)\n",state,ch);
	switch(state){
	case 0: // testare tranzitii posibile din starea 0
		if(isalpha(ch)||ch=='_'){
		pStartCh=pCrtCh; // memoreaza inceputul ID-ului
		pCrtCh++; // consuma caracterul
		state=15; // trece la noua stare
		}
		else if(ch=='='){
		pCrtCh++;
		state=43;
		}
		else if(ch==' '||ch=='\r'||ch=='\t'){
		pCrtCh++; // consuma caracterul si ramane in starea 0
		}
		else if(ch=='\n'){ // tratat separat pentru a actualiza linia curenta
		line++;
		pCrtCh++;
		}
		else if(ch=='+'){
		pCrtCh++;
		state=33;
		}
		else if(ch=='-'){
		pCrtCh++;
		state=34;
		}
		else if(ch=='*'){
			pCrtCh++;
			state=35;
		}
		else if(ch=='/'){
			pCrtCh++;
			state=36;
		}
		else if(ch=='.'){
			pCrtCh++;
			state=37;
		}
		else if(ch=='&'){
			pCrtCh++;
			state=38;
		}
		else if(ch=='|'){
			pCrtCh++;
			state=40;
		}
		else if(ch=='!'){
			pCrtCh++;
			state=42;
		}
		else if(ch=='<'){
			pCrtCh++;
			state=48;
		}
		else if(ch=='>'){
			pCrtCh++;
			state=51;
		}
		else if(ch=='0'){pStartCh=pCrtCh; //verificare numar baza 8
			pCrtCh++;
			state=3;
		}
		else if(ch>='1' && ch<='9'){pStartCh=pCrtCh;  //numar in baza 10
			pCrtCh++;
			state=1;
		}
		else if(ch=='\''){ pStartCh=pCrtCh;
			pCrtCh++;
			state=17;
		}
		else if(ch=='"'){pStartCh=pCrtCh;
			pCrtCh++;
			state=21;
		}
		else if(ch==','){
			pCrtCh++;
			state=25;
		}
		else if(ch==';'){
			pCrtCh++;
			state=26;
		}
		else if(ch=='('){
			pCrtCh++;
			state=27;
		}
		else if(ch==')'){
			pCrtCh++;
			state=28;
		}
		else if(ch=='['){
			pCrtCh++;
			state=29;
		}
		else if(ch==']'){
			pCrtCh++;
			state=30;
		}
		else if(ch=='{'){
			pCrtCh++;
			state=31;
		}
		else if(ch=='}'){
			pCrtCh++;
			state=32;
		}
		else if(ch==0){ // sfarsit de sir
		addTk(END,line);
		return END;
		}
		else tkerr(addTk(END,line),"caracter invalid %c",ch);
		break;
	case 1:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
		}
		else if(ch=='.'){
			pCrtCh++;
			state=10;
		}
		else if(ch=='E'||ch=='e'){
			pCrtCh++;
			state=13;
		}
		else {state=2;base=10;}
		break;
	case 2:
		tk=addTk(CT_INT,line);
		//printf("**%ld**",atol(createString(pStartCh,pCrtCh)));
		tk->i=strtol(createString(pStartCh,pCrtCh),NULL,base);
		return CT_INT;
	case 3:
		if(ch>='0'&& ch <='7'){
			pCrtCh++;
			state=4;
		}
		else if(ch=='E' || ch=='e'){
			pCrtCh++;
			state=13;
		}
		else if(ch=='.'){
			pCrtCh++;
			state=10;
		}
		else if(ch=='8'||ch=='9'){
			pCrtCh++;
			state=5;
		}
		else if(ch=='x'){    //verificare '0x' pentru hexa
			pCrtCh++;
			state=6;
			base=16;
		}
		else {state=2;base=10;}  //avem un singur caracter'0' 
		break;
	case 4:
		if(ch >='0' && ch <='7'){
			pCrtCh++;
		}
		else if(ch=='8' || ch=='9'){
			pCrtCh++;
			state=9;
		}
		else if(ch=='.'){
			pCrtCh++;
			state=10;
		}
		else if(ch=='e' || ch =='E'){
			pCrtCh++;
			state=13;
		}
		else {state=2;base=8;}
		break;
	case 5:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
		}
		else state=8;
		break;
	case 6:
		if((ch>='0' && ch<='9')||(ch>='a' && ch<='f') || (ch>='A' && ch <='F')){
			pCrtCh++;
			state=7;
		}
		break;
	case 7 :
		if((ch>='0' && ch<='9')||(ch>='a' && ch<='f') || (ch>='A' && ch <='F')){
			pCrtCh++;
		}
		else {state=2;base=16;}
		break;
	case 8:
		if(ch=='.'){
			pCrtCh++;
			state=10;
		}
		break;
	case 9:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
		}
		else if(ch == '.'){
			pCrtCh++;
			state=8;
		}
		break;
	case 10:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
			state=11;
		}
		break;
	case 11:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
		}
		else if(ch=='e' || ch=='E'){
			pCrtCh++;
			state=13;
		}
		else state=12;
		break;
	case 12:
		tk=addTk(CT_REAL,line);
		//printf("**%s** - base %d",createString(pStartCh,pCrtCh),base);
		tk->r=strtod(createString(pStartCh,pCrtCh),NULL);
		return CT_REAL;
	case 13:
		if(ch=='-' || ch=='+'){
			pCrtCh++;
			state=14;
		}
		else if(ch>='0' && ch<='9'){
			pCrtCh++;
			state=14;
		}
		break;
	case 14:
		if(ch>='0' && ch<='9'){
			pCrtCh++;
		}
		else state=12;
		break;
	case 15:
		if(isalpha(ch) || (ch>='0' && ch<='9') || ch=='_'){
			pCrtCh++;
		}
		else state=16;
		break;
	case 16:
		nCh=pCrtCh-pStartCh; // lungimea cuvantului gasit
		// teste cuvinte cheie
		if(nCh==5&&!memcmp(pStartCh,"break",5))tk=addTk(BREAK,line);
		else if(nCh==4&&!memcmp(pStartCh,"char",4))tk=addTk(CHAR,line);
		else if(nCh==3&&!memcmp(pStartCh,"int",3))tk=addTk(INT,line);
		else if(nCh==5&&!memcmp(pStartCh,"float",5))tk=addTk(FLOAT,line);
		else if(nCh==6&&!memcmp(pStartCh,"double",6))tk=addTk(DOUBLE,line);
		else if(nCh==5&&!memcmp(pStartCh,"short",5))tk=addTk(SHORT,line);
		else if(nCh==4&&!memcmp(pStartCh,"long",4))tk=addTk(LONG,line);
		else if(nCh==8&&!memcmp(pStartCh,"unsigned",8))tk=addTk(UNSIGNED,line);
		else if(nCh==4&&!memcmp(pStartCh,"void",4))tk=addTk(VOID,line);
		else if(nCh==6&&!memcmp(pStartCh,"return",6))tk=addTk(RETURN,line);
		else if(nCh==2&&!memcmp(pStartCh,"if",2))tk=addTk(IF,line);
		else if(nCh==5&&!memcmp(pStartCh,"while",5))tk=addTk(WHILE,line);
		else if(nCh==3&&!memcmp(pStartCh,"for",3))tk=addTk(FOR,line);
		else if(nCh==6&&!memcmp(pStartCh,"switch",6))tk=addTk(SWITCH,line);
		else if(nCh==4&&!memcmp(pStartCh,"case",4))tk=addTk(CASE,line);
		else if(nCh==4&&!memcmp(pStartCh,"else",4))tk=addTk(ELSE,line);
		else if(nCh==2&&!memcmp(pStartCh,"do",2))tk=addTk(WHILE,line);
		else if(nCh==6&&!memcmp(pStartCh,"struct",6))tk=addTk(STRUCT,line);
		// … toate cuvintele cheie …
		else{ // daca nu este un cuvant cheie, atunci e un ID
		tk=addTk(ID,line);
		tk->text=createString(pStartCh,pCrtCh);
		}
		return tk->code;
	case 17:
		if(ch=='\\'){
			pCrtCh++;
			state=18;
		}
		else if(ch!='\'' && ch!='\\'){
			pCrtCh++;
			state=19;
		}
		break;
	case 18:
		if(ch=='a' || ch=='b' || ch=='f' || ch=='n' || ch=='r' || ch=='t' || ch=='v' || ch =='\'' || ch=='?' || ch=='"' || ch=='0' || ch=='\\'){
			pCrtCh++;
			state=19;
		}
		break;
	case 19:
		if(ch=='\''){
			pCrtCh++;
			state=20;
		}
		break;
	case 20:
		tk=addTk(CT_CHAR,line);
		tk->text=createString(pStartCh,pCrtCh);
		return CT_CHAR;
	case 21:
		if(ch=='"'){
			pCrtCh++;
			state=24;
		}
		else if(ch=='\\'){
			pCrtCh++;
			state=22;
		}
		else if(ch!='"' && ch!='\\'){
			pCrtCh++;
			state=23;
		}
		break;
	case 22:
		if(ch=='a' || ch=='b' || ch=='f' || ch=='n' || ch=='r' || ch=='t' || ch=='v' || ch =='\'' || ch=='?' || ch=='"' || ch=='0' || ch=='\\'){
			pCrtCh++;
			state=23;
		}
		break;
	case 23:
		state=21;
		break;
	case 24:
		tk=addTk(CT_STRING,line);
		tk->text=createString(pStartCh,pCrtCh);
		return CT_STRING;
	case 25:
		addTk(COMMA,line);
		return COMMA;
	case 26:
		addTk(SEMICOLON,line);
		return SEMICOLON;
	case 27:
		addTk(LPAR,line);
		return LPAR;
	case 28:
		addTk(RPAR,line);
		return RPAR;
	case 29:
		addTk(LBRACKET,line);
		return LBRACKET;
	case 30:
		addTk(RBRACKET,line);
		return RBRACKET;
	case 31:
		addTk(LACC,line);
		return LACC;
	case 32:
		addTk(RACC,line);
		return RACC;
	case 33:
		addTk(ADD,line);
		return ADD;
	case 34:
		addTk(SUB,line);
		return SUB;
	case 35:
		addTk(MUL,line);
		return MUL;
	case 36:
		if(ch=='/'){
			pCrtCh++;
			state=55;
		}
		else if(ch=='*'){
			pCrtCh++;
			state=56;
		}
		else state=54;
		break;
	case 37:
		addTk(DOT,line);
		return DOT;
	case 38:
		if(ch=='&'){
			pCrtCh++;
			state=39;
		}
		break;
	case 39:
		addTk(AND,line);
		return AND;
	case 40:
		if(ch=='|'){
			pCrtCh++;
			state=41;
		}
		break;
	case 41:
		addTk(OR,line);
		return OR;
	case 42:
		if(ch=='='){
			pCrtCh++;
			state=47;
		}
		else state=46;
		break;
	case 43:
		if(ch=='='){
			pCrtCh++;
			state=45;
		}
		else state=44;
		break;
	case 44:
		addTk(ASSIGN,line);
		return ASSIGN;
	case 45:
		addTk(EQUAL,line);
		return EQUAL;
	case 46:
		addTk(NOT,line);
		return NOT;
	case 47:
		addTk(NOTEQ,line);
		return NOTEQ;
	case 48:
		if(ch=='='){
			pCrtCh++;
			state=50;
		}
		else state=49;
		break;
	case 49:
		addTk(LESS,line);
		return LESS;
	case 50:
		addTk(LESSEQ,line);
		return LESSEQ;
	case 51:
		if(ch=='='){
			pCrtCh++;
			state=53;
		}
		else state=52;
		break;
	case 52:
		addTk(GREATER,line);
		return GREATER;
	case 53:
		addTk(GREATEREQ,line);
		return GREATEREQ;
	case 54:
		addTk(DIV,line);
		return DIV;
	case 55:
		if(ch!='\n' && ch!='\r' && ch!='\0'){
			pCrtCh++;
		}
		else state=0;
		break;
	case 56:
		if(ch=='\n'){
		line++;
		pCrtCh++;
		}
		else if(ch!='*'){
			pCrtCh++;
		}
		else if(ch=='*'){
			pCrtCh++;
			state=57;
		}
		break;
	case 57:
		if(ch=='*'){
			pCrtCh++;
		}
		else if(ch!='*' && ch!='/'){
			pCrtCh++; 
			state=56;
		}
		else if(ch=='/'){
			pCrtCh++;
			state=0;
		}
		break;
	}
	}
}

