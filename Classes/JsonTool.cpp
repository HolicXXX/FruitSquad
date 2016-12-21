#include "JsonTool.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "cocos2d.h"
USING_NS_CC;

static JsonTool* _jsontool = nullptr;

JsonTool::JsonTool()
	:m_filename("")
{
}

JsonTool::~JsonTool()
{
}

JsonTool* JsonTool::getInstance()
{
	if (!_jsontool)
	{
		_jsontool = new JsonTool();
	}
	return _jsontool;
}

void JsonTool::parseJson(const std::string& file)
{
	if (m_filename == "")
	{
		m_filename = "data.json";
		auto path = FileUtils::getInstance()->getWritablePath().append(m_filename);
		auto checkData = FileUtils::getInstance()->getStringFromFile(path);
		if (checkData == "")
		{
			auto data = FileUtils::getInstance()->getStringFromFile(m_filename);
			m_doc.Parse<0>(data.c_str());
			if (!m_doc.IsObject())
			{
				CCLOG("json parse error");
			}
			saveJson();
		}
		else
		{
			m_doc.Parse<0>(checkData.c_str());
			if (!m_doc.IsObject())
			{
				CCLOG("json parse error");
			}
		}
	}
}

void JsonTool::saveJson()//need test
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_doc.Accept(writer);
	auto src = FileUtils::getInstance()->getWritablePath().append(m_filename);
	FILE* fp = fopen(src.c_str(), "w+");
	if (!fp)
	{
		CCLOG("fopen error");
	}
	fwrite(buffer.GetString(), sizeof(char), buffer.GetSize(), fp);
	fclose(fp);
}