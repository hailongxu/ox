

#pragma once


class ox_tmp_file_util
{
public:
	static bool get_unique_temp_name (char * strTempName)
	{
		//Get the temporary files directory.
		char szTempPath [MAX_PATH];
		unsigned long dwResult=::GetTempPath (MAX_PATH, szTempPath);
		if (dwResult==0) 
			return FALSE;

		//Create a unique temporary file.
		char szTempFile[MAX_PATH];
		unsigned int nResult=GetTempFileName (szTempPath, _T ("~ex"),0,szTempFile);
		if (dwResult==0) 
			return FALSE;

		_tcscpy(strTempName,szTempFile);

		return TRUE;
	}
};

#endif// _____TEMPFILE_H_____

