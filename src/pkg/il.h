/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 20010-10-24
	版权：版权归作者所有，本文件可以自由传播
 */


#include <wchar.h>
#include <locale.h>
#include <stdlib.h>


#pragma once


namespace ox
{
namespace il
{

struct locale_const_t
{
	enum il_configure_thread_locale_c
	{
		__retrive_current_thread_locale = 0,
		__enable_per_thread_locale = _ENABLE_PER_THREAD_LOCALE,
		__disable_per_thread_locale = _DISABLE_PER_THREAD_LOCALE,
	};
	enum il_local_categary_c
	{
		__lc_all      =LC_ALL      ,
		__lc_collate  =LC_COLLATE  ,
		__lc_ctype    =LC_CTYPE    ,
		__lc_monetary =LC_MONETARY ,
		__lc_numeric  =LC_NUMERIC  ,
		__lc_time     =LC_TIME     ,
	};
};

struct locale_same_stdc : virtual public locale_const_t
{
	static int configthreadlocale(il_configure_thread_locale_c type) {return _configthreadlocale(type);}
	static _locale_t create_locale(il_local_categary_c category,const char * locale){return _create_locale(category,locale);}
	static _locale_t get_current_locale(void) {return _get_current_locale();}
	/// 对应的函数是_get_current_locale 和 _create_locale这两个函数
	static void free_locale(_locale_t const & locale) {_free_locale(locale);}
};

 /// 这个函数不用free_locale调用
struct locale_stdc : public locale_same_stdc
{
	template<typename character_tn>
	static character_tn * setlocale(il_local_categary_c category,const character_tn *locale);
	template<> static char * setlocale<char>(il_local_categary_c category,const char *locale)
	{return ::setlocale(category,locale);}
	template<> static wchar_t * setlocale<wchar_t>(il_local_categary_c category,const wchar_t *locale)
	{return ::_wsetlocale(category,locale);}
};

/// 目前：不允许拷贝构造和赋值，只能构建自己，要不要这两个，将来再说
class locale_t : protected locale_stdc, virtual public locale_const_t
{
public:
	/// 直接初始化成系统平台的本地设置，不是C系统的当前配置
	locale_t(bool init_by_sys=false) {if(init_by_sys)locale_=get_sys_locale();}
	locale_t(il_local_categary_c category,const char * locale) {create(category,locale);}
	locale_t(locale_t const & refv); //{create(LC_ALL,refv->locinfo->lc_category->locale);}
	locale_t(_locale_t const & _locale_):locale_(_locale_){}
	~locale_t() {free_locale(locale_);}

	bool create(il_local_categary_c category,const char * locale)
	{
		locale_=create_locale(category,locale);
		/// 我不知道，是否创建成功，所以全部返回成功
		return true;
	}
	operator _locale_t () const { return locale_;}
	locale_t & operator = (locale_t const & refv); //{create(refv->locinfo->lc_category,refv->locinfo->locale); return *this;}
	_locale_t refp_locale()const{return locale_;}
	_locale_t operator->()const{ return locale_;}
	_locale_t operator*()const { return locale_;}

	/// 当前的描述是“C”模式
	static locale_t get_current() {return locale_t(get_current_locale());}
	static locale_t get_sys(il_local_categary_c category=__lc_all) { return locale_t(category,"");}
	static _locale_t const & get_current_locale() { return get_current_locale();}
	/// 因为这个系统的默认值是不会变的，因为重新设置后，系统需要重新启动才可以，所以
	/// 我就用局部临时变量代替了，就免得每次都创建一次
	static _locale_t get_sys_locale(il_local_categary_c category=__lc_all)
	{
		return create_locale(category,"");
	}
	template<typename character_tn>
	static character_tn * setlocale(il_local_categary_c category,const character_tn *locale){return locale_stdc::setlocale(category,locale);}
private:
	_locale_t locale_;
	//int categary_;
};

/**
LC_ALL	All categories, as listed below.
LC_COLLATE	The strcoll, _stricoll, wcscoll, _wcsicoll, strxfrm, _strncoll, _strnicoll, _wcsncoll, _wcsnicoll, and wcsxfrm functions.
LC_CTYPE	The character-handling functions (except isdigit, isxdigit, mbstowcs, and mbtowc, which are unaffected).
LC_MONETARY	Monetary-formatting information returned by the localeconv function.
LC_NUMERIC	Decimal-point character for the formatted output routines (such as printf), for the data-conversion routines, and for the non-monetary formatting information returned by localeconv. In addition to the decimal-point character, LC_NUMERIC sets the thousands separator and the grouping control string returned by localeconv.
LC_TIME	The strftime and wcsftime functions.

locale  "lang[_country_region[.code_page]]"
| ".code_page"
| ""
| NULL

========================================
Primary language  Sublanguage  Language string  
Chinese		Chinese		"chinese"
Chinese 	Chinese (simplified)	"chinese-simplified" or "chs"
Chinese		Chinese (traditional)	"chinese-traditional" or "cht"
Czech		Czech				"csy" or "czech"
Danish		Danish				"dan" or "danish"
Dutch		Dutch (default)		"dutch" or "nld"
Dutch		Dutch (Belgium)		"belgian", "dutch-belgian", or "nlb"
English		English (default)	"english"
English		English (Australia)	"australian", "ena", or "english-aus"
English		English (Canada)	"canadian", "enc", or "english-can"
English		English (New Zealand)	"english-nz" or "enz"
English		English (United Kingdom)	"eng", "english-uk", or "uk"
English		English (United States)		"american", "american english", "american-english", "english-american", "english-us", "english-usa", "enu", "us", or "usa"
Finnish		Finnish		"fin" or "finnish"
French		French (default)	"fra" or "french"
French		French (Belgium)	"frb" or "french-belgian"
French		French (Canada)		"frc" or "french-canadian"
French		French (Switzerland)	"french-swiss" or "frs"
German		German (default)		"deu" or "german"
German		German (Austria)		"dea" or "german-austrian"
German		German (Switzerland)	"des", "german-swiss", or "swiss"
Greek		Greek		"ell" or "greek"
Hungarian	Hungarian	"hun" or "hungarian"
Icelandic	Icelandic	"icelandic" or "isl"
Italian		Italian (default)	"ita" or "italian"
Italian		Italian (Switzerland)	"italian-swiss" or "its"
Japanese	Japanese	"japanese" or "jpn"
Korean		Korean	"kor" or "korean"
Norwegian	Norwegian (default)		"norwegian"
Norwegian	Norwegian (Bokmal)		"nor" or "norwegian-bokmal"
Norwegian	Norwegian (Nynorsk)		"non" or "norwegian-nynorsk"
Polish		Polish		"plk" or "polish"
Portuguese	Portuguese (default)		"portuguese" or "ptg"
Portuguese	Portuguese (Brazil)		"portuguese-brazilian" or "ptb"
Russian		Russian (default)		"rus" or "russian"
Slovak		Slovak		"sky" or "slovak"
Spanish		Spanish (default)	"esp" or "spanish"
Spanish		Spanish (Mexico)	"esm" or "spanish-mexican"
Spanish		Spanish (Modern)	"esn" or "spanish-modern"
Swedish		Swedish		"sve" or "swedish"
Turkish		Turkish		"trk" or "turkish"

========================================
Country/Region  Country/Region string  
Australia	"aus" or "australia"
Austria	"aut" or "austria"
Belgium	"bel" or "belgium"
Brazil	"bra" or "brazil"
Canada	"can" or "canada"
China	"china", "chn", "pr china", or "pr-china"
Czech Republic	"cze" or "czech"
Denmark	"dnk" or "denmark"
Finland	"fin" or "finland"
France	"fra" or "france"
Germany	"deu" or "germany"
Greece	"grc" or "greece"
Hong Kong SAR	"hkg", "hong kong", or "hong-kong"
Hungary	"hun" or "hungary"
Iceland	"iceland" or "isl"
Ireland	"irl" or "ireland"
Italy	"ita" or "italy"
Japan	"jpn" or "japan"
Korea	"kor" or "korea"
Mexico	"mex" or "mexico"
The Netherlands	"nld", "holland", or "netherlands"
New Zealand	"nzl", "new zealand", "new-zealand", or "nz"
Norway	"nor" or "norway"
Poland	"pol" or "poland"
Portugal	"prt" or "portugal"
Russia	"rus" or "russia"
Singapore	"sgp" or "singapore"
Slovakia	"svk" or "slovak"
Spain	"esp" or "spain"
Sweden	"swe" or "sweden"
Switzerland	"che" or "switzerland"
Taiwan	"twn" or "taiwan"
Turkey	"tur" or "turkey"
United Kingdom	"gbr", "britain", "england", "great britain", "uk", "united kingdom", or "united-kingdom"
United States	"usa", "america", "united states", "united-states", or "us" 

===
"C"
*/

}
}