// json_erase_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../src_json/JSON.h"


struct condi : JSONValue::stringify_interface
{
	bool operator()(JSONValue const* value)
	{
		if (value->IsString())
			return false;
		return true;
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	JSONDocumentA da;
	da.parse("1234567890");
	double aa = da.root().AsNumber();
	/// ����һ��document����
	JSONDocument doc;
	/// ���ɵĶ��󶼲����ͷţ�doc��ͳһ�ͷ�
	JSONNull* jnull = doc.make_json_null();
	JSONArray* jarray = doc.make_json_array();
	JSONObject* jobject = doc.make_json_object();
	JSONObject* jobject2 = doc.make_json_object();
	jobject2->push_back(L"key1",L"value1");
	jobject2->push_back(L"key2",L"value2");
	jarray->push_back(L"stt1");
	jarray->push_back(344.0);
	jobject->push_back(L"k1",L"v1");
	jobject->push_back(L"k2",jarray);
	jobject->push_back(L"k3",jobject2);
	/// ��һ�������ʽ����json��
	std::wstring str2 = jobject->Stringify();
	//jarray->push_back(L"2");
	//jarray->push_back(jobject);
	//std::wstring str = jarray->Stringify(&condi());
	//JSONValue const& v = (*jarray)[0];
	jarray->remove(0);
	jarray->remove(0);
	return 0;
}

