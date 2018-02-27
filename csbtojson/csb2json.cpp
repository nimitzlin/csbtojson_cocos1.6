
#include "stdafx.h"
#include "Csb2Json.h"
#include <direct.h>
using namespace std;
using namespace cocostudio;

Csb2Json::Csb2Json()
{
}

Csb2Json::~Csb2Json()
{
}

char* Csb2Json::getDataFromFile(string path)
{
	ifstream t;  
	int length;
	
	t.open(path,ios::binary);      // open input file 
	if(!t)
	{
		exit(-1);
	}
	t.seekg(0, ios::end);    // go to the end  
	length = t.tellg();           // report location (this is the length)  
	t.seekg(0, ios::beg);    // go back to the beginning  
	csbBuffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
	memset(csbBuffer, 0, length);
	bufferLen = length;
	t.read(csbBuffer, length);       // read the whole file into the buffer  
	t.close();                    // close file handle  
	return csbBuffer;
}

int Csb2Json::run(const string csbName)
{
	char* cur_path;
	cur_path = _getcwd( NULL, 0 );

	std::string fullPath = string(cur_path) + string("\\") + string(csbName);
    auto len = csbName.find(".");
    auto pureName = csbName.substr(0, len);
    auto jsonName = pureName + ".json";
    auto fileDataBytes = getDataFromFile(fullPath);
   
    ofstream oStream(jsonName, ios::out);
    m_pOutStream = &oStream;

    CocoLoader  tCocoLoader;
    if (true == tCocoLoader.ReadCocoBinBuff((char*)fileDataBytes))
    {
        *m_pOutStream << "{" << endl;
        stExpCocoNode*  tpRootCocoNode = tCocoLoader.GetRootCocoNode();
        rapidjson::Type tType = tpRootCocoNode->GetType(&tCocoLoader);
        if (rapidjson::kObjectType == tType || rapidjson::kArrayType == tType)
        {
            getTree(&tCocoLoader, tpRootCocoNode,1);
        }
        *m_pOutStream << "}" << endl;
        m_pOutStream->close();

		bufferLen = 0;
		delete []csbBuffer;
        return 0;
    }
    else {
		bufferLen = 0;
		delete []csbBuffer;
        return 1;
    }
}

void Csb2Json::getTree(CocoLoader *pLoader, stExpCocoNode *pNode, int nTabNum)
{
    stExpCocoNode* pArray = pNode->GetChildArray(pLoader);
    int nChildNum = pNode->GetChildNum();
    //strTab用于控制缩排，按cocos原来的标准，一次缩排2个空格。
    std::string strTab = "";
    for (int i = 0; i < nTabNum; ++i) {
        strTab = strTab + "  ";
    }
    for (int i = 0; i < nChildNum; ++i) {
        stExpCocoNode *pSubNode = &pArray[i];
        string strKey = pSubNode->GetName(pLoader);
        string strVal = pSubNode->GetValue(pLoader);
        rapidjson::Type tType = pSubNode->GetType(pLoader);

        if (strKey != "") {
            strKey = "\"" + strKey + "\": ";
        }

        if (rapidjson::kObjectType == tType) {
            if (strKey == "\"children\": ") {
                strKey = "";
            }
            *m_pOutStream << strTab << strKey << "{" << endl;
            getTree(pLoader, pSubNode, nTabNum + 1);
            *m_pOutStream << strTab << "}";
        }
        else if (rapidjson::kArrayType == tType) {
            *m_pOutStream << strTab << strKey << "[";
            int subNum = pSubNode->GetChildNum();
            if (subNum == 0) {  //避免空数组换行。
                *m_pOutStream << "]";
            }
            else {
                *m_pOutStream << endl;
                getTree(pLoader, pSubNode, nTabNum + 1);
                *m_pOutStream << strTab << "]";
            }
        }
        else {
            if (rapidjson::kNullType == tType) {
                strVal = "null";
            }
            else if (rapidjson::kStringType == tType || 83 == tType) {
                strVal = "\"" + strVal + "\"";
            }
            else if ((rapidjson::kNumberType == tType || -8 == tType) && strVal == "1") {
                strVal = "true";
            }
            else if ((rapidjson::kNumberType == tType || -8 == tType) && strVal == "0") {
                strVal = "false";
            }
            *m_pOutStream << strTab << strKey << strVal;
        }

        //判断并合理使用结尾的逗号。
        if (i == nChildNum - 1) {
            *m_pOutStream << endl;
        }
        else {
            *m_pOutStream << "," << endl;
        }
    }
}

