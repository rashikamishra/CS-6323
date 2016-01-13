#ifndef PARSER_H
#include "Parser.h"
#endif


Parser::Parser() {
	File=0;
	LineNum=0;
	strcpy_s(FileName,"");
}


Parser::~Parser() {
	if(File) {
		printf("ERROR: Parser::~Parser()- Closing file '%s'\n",FileName);
		fclose((FILE*)File);
	}
}

bool Parser::Open(const char *fname) {
	File=(void*)fopen(fname,"r");
	LineNum=1;
	if(File==0) {
		printf("ERROR: Parser::Open()- Can't open file '%s'\n",fname);
		return false;
	}
	strcpy_s(FileName,fname);
	return true;
}

bool Parser::Close() {
	if(File) fclose((FILE*)File);
	else return false;

	File=0;
	return true;
}

bool Parser::Abort(char *error) {
	printf("ERROR '%s' line %d: %s\n",FileName,LineNum,error);
	Close();
	return false;
}

char Parser::GetChar() {
	char c=char(getc((FILE*)File));
	if(c=='\n') LineNum++;
	return c;
}

char Parser::CheckChar() {
	int c=getc((FILE*)File);
	ungetc(c,(FILE*)File);
	return char(c);
}

int Parser::GetInt() {
	SkipWhitespace();
	int pos=0;
	char temp[256];

	char c=CheckChar();
	if(c=='-') {
		temp[pos++]=GetChar();
		c=CheckChar();
	}
	if(!isdigit(c)) {
		printf("ERROR: Parser::GetInt()- Expecting int on line %d of '%s'\n",LineNum,FileName);
		return 0;
	}
	temp[pos++]=GetChar();

	while(isdigit(c=CheckChar())) temp[pos++]=GetChar();

	temp[pos++]='\0';
	return atoi(temp);
}

float Parser::GetFloat() {
	SkipWhitespace();
	int pos=0;
	char temp[256];

	char c=CheckChar();
	if(c=='-') {
		temp[pos++]=GetChar();
		c=CheckChar();
	}
	if(!isdigit(c)) {
		printf("ERROR: Parser::GetFloat()- Expecting float on line %d of '%s' '%c'\n",LineNum,FileName,c);
		return 0.0f;
	}
	temp[pos++]=GetChar();

	while(isdigit(c=CheckChar())) temp[pos++]=GetChar();

	if(c=='.') {
		temp[pos++]=GetChar();
		while(isdigit(c=CheckChar())) temp[pos++]=GetChar();
	}

	if(c=='e' || c=='E') {
		temp[pos++]=GetChar();
		c=CheckChar();
		if(c=='+' || c=='-') {
			temp[pos++]=GetChar();
			c=CheckChar();
		}
		if(!isdigit(c)) {
			printf("ERROR: Parser::GetFloat()- Poorly formatted float exponent on line %d of '%s'\n",LineNum,FileName);
			return 0.0f;
		}
		while(isdigit(c=CheckChar())) temp[pos++]=GetChar();
	}
	temp[pos++]='\0';
	return float(atof(temp));
}

bool Parser::GetToken(char *str) {
	SkipWhitespace();

	int pos=0;
	char c=CheckChar();
	while(c!=' ' && c!='\n' && c!='\t' && c!='\r' && !feof((FILE*)File)) {
		str[pos++]=GetChar();
		c=CheckChar();
	}
	str[pos]='\0';
	return true;
}

bool Parser::FindToken(const char *tok) {
	int pos=0;
	while(tok[pos]!='\0') {
		if(feof((FILE*)File)) return false;
		char c=GetChar();
		if(c==tok[pos]) pos++;
		else pos=0;
	}
	return true;
}

bool Parser::SkipWhitespace() {
	char c=CheckChar();
	bool white=false;
	while(isspace(c)) {
		GetChar();
		c=CheckChar();
		white=true;
	}
	return white;
}

bool Parser::SkipLine() {
	char c=GetChar();
	while(c!='\n') {
		if(feof((FILE*)File)) return false;
		c=GetChar();
	}
	return true;
}

bool Parser::Reset() {
	if(fseek((FILE*)File,0,SEEK_SET)) return false;
	return true;
}