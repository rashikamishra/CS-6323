
#ifndef UTILS_H
#include "Utils.h"
#endif
class Parser {
public:
	Parser();
	~Parser();

	bool Open(const char *file);
	bool Close();

	bool Abort(char *error);	


	char GetChar();
	char CheckChar();
	int GetInt();
	float GetFloat();
	bool GetToken(char *str);
	bool FindToken(const char *tok);
	bool SkipWhitespace();
	bool SkipLine();
	bool Reset();

	
	char *GetFileName()			{return FileName;}
	int GetLineNum()			{return LineNum;}

private:
	void *File;
	char FileName[256];
	int LineNum;
};

