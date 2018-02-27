#pragma once

#include <fstream>
#include "CocoLoader.h"
#include "stdafx.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <cstdio>
#include <string>
#include <fstream>  
#include <streambuf>
#include <stdint.h>
#include "zlib/include/zlib.h"
#include "zlib/include/zconf.h"
#include "CocoLoader.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;
using namespace cocostudio;

class Csb2Json
{
public:
    Csb2Json();
    ~Csb2Json();
    static Csb2Json * getInstance() {
        static Csb2Json m_instance;
        return &m_instance;
    };

	char* getDataFromFile(string path);

    int run(const string csbName);

private:
    void getTree(CocoLoader *pLoader, stExpCocoNode *pNode, int nTabNum);
    ofstream * m_pOutStream;
	
	char* csbBuffer;
	int bufferLen;

};

