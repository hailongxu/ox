
/*
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14
*/

#pragma once

#include <stdarg.h>
#include <time.h>
#include <string.h>

#include <windows.h>

/**
	clock_t : 单位是毫秒
	clock()返回的是从当前进程启动伊始到现在的时间流逝，单位是毫秒

	time_t : 单位是秒
	time ()返回的是秒数，从1970-01-01 00:00:00开始所经过的秒数

	tm

	tm_sec		Seconds after minute (0 – 59).
	tm_min		Minutes after hour (0 – 59).
	tm_hour	Hours after midnight (0 – 23).
	tm_mday	Day of month (1 – 31).
	tm_mon	Month (0 – 11; January = 0).
	tm_year	Year (current year minus 1900).
	tm_wday	Day of week (0 – 6; Sunday = 0).
	tm_yday	Day of year (0 – 365; January 1 = 0).
	tm_isdst	Positive value if daylight saving time is in effect; 0 if daylight saving time is not in effect; negative value if status of daylight saving time is unknown. If the TZ environment variable is set, the C run-time library assumes rules appropriate to the United States for implementing the calculation of daylight-saving time (DST).
*/
namespace ox
{
	namespace pkg
	{
		//namespace ___
		//{
		//	template <typename character_tn> class datetime_trait;

		//	template <> class datetime_trait<char>
		//	{
		//	protected:
		//		typedef char character;
		//		character * to_string(time_t const& _in_time_t) const
		//		{
		//			//tm *ptm = localtime(&_m_sencods_elapsed_from_1977_01_01_00_00_00);
		//			//return asctime(ptm);
		//			return ctime(&_in_time_t);
		//		}
		//		static int vsprintf(char *buffer, const char *format, va_list argptr)
		//		{
		//			return ::vsprintf(buffer,format,argptr);
		//		}
		//	};
		//	template <> class datetime_trait<wchar_t>
		//	{
		//	protected:
		//		typedef wchar_t character;
		//		character * to_string(time_t const& _in_time_t) const
		//		{
		//			//tm *ptm = localtime(&_m_sencods_elapsed_from_1977_01_01_00_00_00);
		//			//return asctime(ptm);
		//			//return wctime(_in_time_t);
		//		}
		//		static int vsprintf(wchar_t *buffer, const wchar_t *format, va_list argptr)
		//		{
		//			return ::vswprintf(buffer,format,argptr);
		//		}
		//	};
		//}

		struct timespan
		{
			timespan(size_t _days=0,size_t _hours=0,size_t _minutes=0,size_t _seconds=0)
				: days(_days), hours(_hours), minutes(_minutes), seconds(_seconds)
			{}
			size_t days;
			size_t hours;
			size_t minutes;
			size_t seconds;
		};

		struct datetime
		{
			typedef struct tm tm;
		public:
			/// 构造初始化为当前时间，默认构造
			datetime() { _m_sencods_elapsed_from_1977_01_01_00_00_00=time(0); }

			/// 拷贝构造函数
			datetime(tm const & _in_tm)
			{
				tm2time_t(_in_tm,_m_sencods_elapsed_from_1977_01_01_00_00_00);
			}
			/// 拷贝构造函数
			datetime(time_t const & _in_time_t)
				: _m_sencods_elapsed_from_1977_01_01_00_00_00(_in_time_t)
			{}

			/// 拷贝构造函数
			datetime(datetime const & _in_datetime)
				: _m_sencods_elapsed_from_1977_01_01_00_00_00(_in_datetime._m_sencods_elapsed_from_1977_01_01_00_00_00)
			{}


			datetime & assign_from_now()
			{
				_m_sencods_elapsed_from_1977_01_01_00_00_00=time(0);
				return *this;
			}

			/// 常用属性
		public:
			bool is_leap_year() const
			{
				tm * _tmp_tm = to_tm();
				return
					( _tmp_tm->tm_year >= 1582)
					? (_tmp_tm->tm_year%4==0 && _tmp_tm->tm_year%100!=0  ||  _tmp_tm->tm_year%400==0 )
					: (_tmp_tm->tm_year%4==0);
			}

			/// 常用操作
		public:

			/// return current milliseconds from process started
			static long clock() { return ::clock(); }
			static tm& clear(tm& atm)
			{
				memset(&atm,0,sizeof(atm));
				return atm;
			}
			static tm maketm(int y,int m,int d,int h=0,int mi=0,int s=0)
			{
				tm atm;
				clear(atm);
				atm.tm_year = y;
				atm.tm_mon = m;
				atm.tm_mday = d;
				atm.tm_hour = h;
				atm.tm_min = mi;
				atm.tm_sec = s;
				atm.tm_isdst = 0;
				atm.tm_wday = 0;
				atm.tm_yday = 0;
				return atm;
			}
			static tm now()
			{
				time_t atime = time(0);
				return *localtime(&atime);
			}

			// 计算两个时间的差，差值为秒
			static double difftime(time_t & t1, time_t & t2)
			{
				return ::difftime(t1,t2);
			}

			double difftime(time_t const & t)
			{
				return ::difftime(_m_sencods_elapsed_from_1977_01_01_00_00_00,t);
			}

			static double difftime_external(tm const& begin_tm,tm const& end_tm)
			{
				tm tmb = begin_tm;
				tm tme = end_tm;
				to_internal(tmb);
				to_internal(tme);
				time_t begin = tm2time_t(tmb);
				time_t end = tm2time_t(tme);
				return difftime(begin,end);
			}
			static double difftime(tm const& begin_tm,tm const& end_tm)
			{
				time_t begin = tm2time_t(begin_tm);
				time_t end = tm2time_t(end_tm);
				return difftime(begin,end);
			}
			static double sub_real(tm const& begin_tm,tm const& end_tm)
			{
				return difftime_external(begin_tm,end_tm);
			}
			static double sub(tm const& begin_tm,tm const& end_tm)
			{
				return difftime(begin_tm,end_tm);
			}
			static tm sub(tm const& atm, timespan const& span)
			{
				time_t tt = tm2time_t(atm);
				tt -= get_seconds(span);
				return *time_t2tm(tt);
			}
			static tm add(tm const& atm, timespan const& span)
			{
				time_t tt = tm2time_t(atm);
				tt += get_seconds(span);
				return *time_t2tm(tt);
			}
			static tm sub_external(tm const& atm, timespan const& span)
			{
				tm itm = atm;
				to_internal(itm);
				time_t tt = tm2time_t(itm);
				tt -= get_seconds(span);
				itm = *time_t2tm(tt);
				to_external(itm);
				return itm;
			}
			static tm add_external(tm const& atm, timespan const& span)
			{
				tm itm = atm;
				to_internal(itm);
				time_t tt = tm2time_t(itm);
				tt += get_seconds(span);
				itm = *time_t2tm(tt);
				to_external(itm);
			}

			static size_t get_seconds(timespan const& span)
			{
				size_t hours = span.days*24 + span.hours;
				size_t minutes = hours*60+span.minutes;
				size_t seconds = minutes*60+span.seconds;
				return seconds;
			}

			//static size_t dayofweek_real(tm const& atm)
			//{
			//	///w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1 
			//	size_t c = (atm.tm_year/100)*100;
			//	size_t y = atm.tm_year - c;
			//	size_t m = atm.tm_mon;
			//	size_t d = atm.tm_mday;
			//	size_t w = y + (y/4) + (c/4) - 2*c + (26*(m+1)/10) + d - 1;
			//	return w%7;
			//}

			//// 格式化日期
			//int sprintf(character* buf,const character* format, ...)
			//{
			//	va_list arglist;
			//	va_start(arglist,format);
			//	return vsprintf(buf,format,arglist);
			//}

			/// 日期类型的比较
			bool operator < (datetime const & _in_right_datetime)
			{
				return _m_sencods_elapsed_from_1977_01_01_00_00_00 < _in_right_datetime._m_sencods_elapsed_from_1977_01_01_00_00_00;
			}

			static char* to_string(tm const& tm,char* buffer)
			{
				sprintf (buffer,"%u%c%u%c%u%c%u%c%u%c%u",
					tm.tm_year,'.',
					tm.tm_mon,'.',
					tm.tm_mday,' ',
					tm.tm_hour,':',
					tm.tm_min,':',
					tm.tm_sec);
				return buffer;
			}
			/// 把类型转化成相关的类型
		public:
			/// 和原有的C兼容
			/// 把当前类型转成tm类型
			tm * to_tm() const
			{
				return localtime(&_m_sencods_elapsed_from_1977_01_01_00_00_00); 
			}
			static tm * time_t2tm(time_t const& _in_time_t)
			{
				return localtime(&_in_time_t); 
			}
			static tm& to_external(tm& atm)
			{
				atm.tm_year += 1900;
				atm.tm_mon += 1;
				//atm.tm_mday += 0;
				return atm;
			}
			static tm internal(tm const& atm)
			{
				tm t1 = atm;
				to_internal(t1);
				return t1;
			}
			static tm external(tm const& atm)
			{
				tm t1 = atm;
				to_external(t1);
				return t1;
			}
			static tm& to_internal(tm& atm)
			{
				atm.tm_year -= 1900;
				atm.tm_mon -= 1;
				//atm.tm_mday -= 0;
				return atm;
			}
			/// 把当前类型转成time_t类型
			time_t & to_time_t()
			{
				return _m_sencods_elapsed_from_1977_01_01_00_00_00;
			}
			time_t const & to_time_t() const
			{
				return _m_sencods_elapsed_from_1977_01_01_00_00_00;
			}
			/// 把 tm 转化成 time_t
			static time_t tm2time_t(tm const & _in_tm)
			{
				return mktime((tm*)&_in_tm);
			}
			/// 把 tm 转化成 time_t
			static time_t tm2time_t(tm const & _in_tm, time_t & _out_time_t)
			{
				return mktime((tm*)&_out_time_t);
			}

			/// 转换成默认的字符串
			//character * to_string() const
			//{
			//	//tm *ptm = localtime(&_m_sencods_elapsed_from_1977_01_01_00_00_00);
			//	//return asctime(ptm);
			//	return base::to_string(_m_sencods_elapsed_from_1977_01_01_00_00_00);
			//}
			//
		private:
			time_t _m_sencods_elapsed_from_1977_01_01_00_00_00;
		};

		struct win_datetime
		{
			//GetSystemTime();
		};
	} // end of pkg
} // end of ox