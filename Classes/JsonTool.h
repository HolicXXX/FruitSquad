#ifndef _JSONTOOL_H_
#define _JSONTOOL_H_

#include "json/rapidjson.h"
#include "json/document.h"

class JsonTool
{
public:
	static JsonTool* getInstance();

	void parseJson(const std::string& file);
	void saveJson();

	rapidjson::Document& getDoc(){ return m_doc; }
	std::string getCurrentFileName(){ return m_filename; }
private:
	JsonTool();
	~JsonTool();

	std::string m_filename;
	rapidjson::Document m_doc;
};


#endif