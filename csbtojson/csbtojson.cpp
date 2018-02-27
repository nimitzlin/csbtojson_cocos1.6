// csbtojson.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "csb2json.h"

int _tmain(int argc, _TCHAR* argv[])
{

	Csb2Json* csb2j = Csb2Json::getInstance();

	csb2j->run("login.csb");

	return 0;
}

