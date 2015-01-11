
#include <stdio.h>
#include "io.h"
#include <algorithm>
#include <windows.h>
#include <string>
#include <vector>


#pragma once


enum matchenum 
{
	__match_from_left,
	__match_from_right,
};


struct intv_t
{
	intv_t(): b(0),e(0xfffffff) {}
	intv_t(int _b,int _e=0xfffffff): b(_b), e(_e) {}
	void set(int _b,int _e) {b=_b,e=_e;}
	int size() const {return b-e;}
	bool is_ordered() const {return b<=e;}
	int b,e;
};
struct rect_t
{
	rect_t():x1(0),x2(0),y1(0),y2(0) {}
	rect_t(int _x1,int _y1,int _x2,int _y2)
		: x1(_x1),x2(_x2),y1(_y1),y2(_y2)
	{}
	int width() const {return x2-x1;}
	int height() const {return y2-y1;}
	void set_x(int x1,int x2) {this->x1=x1;this->x2=x2;}
	void set_y(int y1,int y2) {this->y1=y1;this->y2=y2;}
	int x1,x2;
	int y1,y2;
};

typedef unsigned char uchar_t;
struct color_t
{
	color_t()
	{
		color.v = 0;
	}
	color_t(uchar_t r,uchar_t g,uchar_t b)
	{
		color.argb[0] = r;
		color.argb[1] = g;
		color.argb[2] = b;
	}
	uchar_t r() const {return color.argb[0];}
	uchar_t g() const {return color.argb[1];}
	uchar_t b() const {return color.argb[2];}
	uchar_t& r() {return color.argb[0];}
	uchar_t& g() {return color.argb[1];}
	uchar_t& b() {return color.argb[2];}
	union
	{
		unsigned int v;
		uchar_t argb[4];
	} color;

	bool is_eq_rgb(uchar_t const* rgb) const
	{
		return is_eq_rgb(*rgb,*(rgb+1),*(rgb+2));
	}
	bool is_eq_bgr(uchar_t const* bgr) const
	{
		return is_eq_bgr(*bgr,*(bgr+1),*(bgr+2));
	}
	bool is_eq_rgb(uchar_t _r,uchar_t _g,uchar_t _b) const
	{
		return r()==_r && g()==_g && b()==_b;
	}
	bool is_eq_bgr(uchar_t _b,uchar_t _g,uchar_t _r) const
	{
		return r()==_r && g()==_g && b()==_b;
	}
	bool operator < (color_t const& other) const
	{
		return color.v < other.color.v;
	}
};

struct interset
{
	static void swap(int& a,int& b)
	{
		int t = a;
		a = b;
		b = t;
	}
	static void normal_rect(rect_t& r)
	{
		if (r.x1>r.x2) swap(r.x1,r.x2);
		if (r.y1>r.y2) swap(r.y1,r.y2);
	}
	static bool is_normal(intv_t const& l)
	{
		return l.b<=l.e;
	}
	static bool is_normal(rect_t const& r)
	{
		return r.x1<=r.x2 && r.y1<=r.y2;
	}
	static int minof(int const& a,int const& b)
	{
		return a<b?a:b;
	}
	static int minof(int const& a,int const& b,int const& c,int const& d)
	{
		int r = a;
		if (b<r) r = b;
		if (c<r) r = c;
		if (d<r) r = d;
		return r;
	}
	static int maxof(int const& a,int const& b,int const& c,int const& d)
	{
		int r = a;
		if (b>r) r = b;
		if (c>r) r = c;
		if (d>r) r = d;
		return r;
	}
	static int intersize(intv_t const& l1,intv_t const& l2)
	{
		assert(is_normal(l1) && is_normal(l2));
		int xleft = minof(l1.b,l1.e,l2.b,l2.e);
		int xright = maxof(l1.b,l1.e,l2.b,l2.e);
		int r = (l1.e-l1.b)+(l2.e-l2.b)-(xright-xleft);
		if (r<0) r=0;
		return r;
	}
	static int interarea(rect_t const& r1,rect_t const& r2)
	{
		assert(is_normal(r1) && is_normal(r2));
		int xlen = intersize(intv_t(r1.x1,r1.x2),intv_t(r2.x1,r2.x2));
		int ylen = intersize(intv_t(r1.y1,r1.y2),intv_t(r2.y1,r2.y2));
		return xlen*ylen;
	}
	static intv_t interline(intv_t const& l1,intv_t const& l2,intv_t const& null_intv=intv_t(0,0))
	{
		assert(is_normal(l1) && is_normal(l2));
		intv_t r = null_intv;
		if (l2.b>l1.e) return r;
		if (l2.b>=l1.b)
		{
			r.b = l2.b;
			r.e = minof(l2.e,l1.e);
			return r;
		}
		r.b = l1.b;
		if (l2.e<l1.b) return intv_t(0,0);
		r.e = minof(l2.e,l1.e);
		return r;
	}
	static rect_t interrect(rect_t const& r1,rect_t const& r2)
	{
		assert(is_normal(r1) && is_normal(r2));
		intv_t xline = interline(intv_t(r1.x1,r1.x2),intv_t(r2.x1,r2.x2));
		intv_t yline = interline(intv_t(r1.y1,r1.y2),intv_t(r2.y1,r2.y2));
		rect_t r (xline.b,yline.b,xline.e,yline.e);
		return r;
	}
	static void combine(std::vector<intv_t> const& vec,int mindistance,std::vector<intv_t>& out)
	{
		if (vec.empty())
			return;
		out.push_back(vec.front());
		for (int i=1;i<vec.size();++i)
		{
			intv_t const& v = vec[i];
			if (v.b-out.back().e<=mindistance-1)
				out.back().e = v.e;
			else
				out.push_back(v);
		}
	}
	static void combine_back(std::vector<intv_t>& a,intv_t const& b)
	{
		if (a.empty())
			a.push_back(b);
		else
		{
			intv_t& c = a.back();
			intv_t d = interset::interline(c,b,intv_t(0,-1));
			if (d.is_ordered())
				c = d;
			else
				a.push_back(c);
		}
	}
	static void combine_sorted(std::vector<intv_t> const& a,std::vector<intv_t> const& b,std::vector<intv_t>& c)
	{
		int asize = a.size();
		int bsize = b.size();
		for (int i=0,j=0;i<asize && j<bsize;)
		{
			if (a[i].b<b[j].b)
				combine_back(c,a[i++]);
			else
				combine_back(c,b[j++]);
		}
	}
	static bool is_any_overlapped(std::vector<intv_t> const& a,intv_t const& b)
	{
		for (int i=0;i<a.size();++i)
		{
			intv_t r = interline(a[i],b,intv_t(0,-1));
			if (r.is_ordered())
				return true;
		}
		return false;
	}
	static void intertest()
	{
		intv_t l1(-4,5);
		intv_t l2(4,12);
		int a = intersize(l1,l2);
		rect_t r1 (1,1,5,6);
		rect_t r2(-4,-4,-2,-2);
		int b = interarea(r1,r2);
		intv_t c = interline(l1,l2);
		rect_t d = interrect(r1,r2);
		int z = 0;
	}
};

struct debug_print
{
	static int& count() {static int _m_count = 0;return _m_count;}
	static int& depth() {static int _m_count = 0;return _m_count;}
	static std::string& buffer() {static std::string _m_buffer;return _m_buffer;}

	static bool& is_printf() {static bool _static_printf=false; return _static_printf;}
	bool _m_debug;
	char const* _m_str;
	debug_print(char const* str,bool bdebug=is_printf()) :_m_str(str),_m_debug(bdebug)
	{
		if (!_m_debug) return;
		char intbuffer[32];
		buffer().clear();
		count()++;
		buffer().append(depth(),'\t');
		buffer().append(_m_str);
		buffer().push_back('(');
		buffer().append(itoa(count(),intbuffer,10));
		buffer().append(") {\n");
		OutputDebugStringA(buffer().c_str());
		depth()++;
	}
	~debug_print()
	{
		if (!_m_debug) return;
		char intbuffer[32];
		depth()--;
		buffer().clear();
		//buffer().push_back('\n');
		buffer().append(depth(),'\t');
		buffer().append("}");
		buffer().append("~");
		buffer().append(_m_str);
		buffer().push_back('(');
		buffer().append(itoa(count(),intbuffer,10));
		buffer().append(")\n");
		OutputDebugStringA(buffer().c_str());
	}
};



struct small_helper
{
	static int erase_zero(int* array, int size)
	{
		int j = 0;
		for (int i=0; i<size; ++i)
		{
			if (array[i]!=0)
				array[j++] = array[i];
		}
		return j;
	}
	static int skip(int* array, int size, int v)
	{
		for (int i=0; i<size; ++i)
		{
			if (array[i]!=v)
				return i;
		}
		return -1;
	}
	static int rskip(int* array, int size, int v)
	{
		for (int i=size-1; i>=0; --i)
		{
			if (array[i]!=v)
				return i;
		}
		return -1;
	}
	static void get_color_distance(int a,int b)
	{
		int d = b-a;
	}
	static void reverse_string(std::string& str)
	{
		size_t size = str.size();
		size_t half = (size+1)/2;
		for (size_t i=0;i<half;++i)
		{
			char c = str[i];
			str[i] = str[size-i-1];
			str[size-i-1] = c;
		}
	}

	static int GetFolderFileList(const char* folder,const char* filter,std::vector<std::string>& filePathArray)
	{
		struct _finddata_t files;
		int fileHandle;
		char fspec[256];
		sprintf(fspec,"%s\\%s",folder,filter);

		fileHandle = _findfirst(fspec,&files);

		if (-1 == fileHandle)
		{
			return 0;
		}

		filePathArray.clear();

		do
		{
			char path[260];
			sprintf(path,"%s\\%s",folder,files.name);

			filePathArray.push_back(std::string(path));

		}while( 0 ==_findnext(fileHandle,&files));

		_findclose(fileHandle);
		return filePathArray.size();
	}
	static int GetFolderOnlyFileList(const char* folder,const char* filter,std::vector<std::string>& filePathArray)
	{
		struct _finddata_t files;
		int fileHandle;
		char fspec[256];
		sprintf(fspec,"%s\\%s",folder,filter);

		fileHandle = _findfirst(fspec,&files);

		if (-1 == fileHandle)
		{
			return 0;
		}

		filePathArray.clear();

		do
		{
			//char path[260];
			//sprintf(path,"%s\\%s",folder,files.name);

			filePathArray.push_back(std::string(files.name));

		}while( 0 ==_findnext(fileHandle,&files));

		_findclose(fileHandle);
		return filePathArray.size();
	}
	static void actiondir(char const* dir,char const* ext,void (*action)(char const*))
	{
		std::vector<std::string> filePathArray;
		GetFolderFileList(dir,ext,filePathArray);
		size_t count = filePathArray.size();
		for (int i = 0;i < count;i++)
		{
			char const* path = filePathArray[i].c_str();
			action(path);
		}
	}
	static void insert_before_ext(char* path,char const* str)
	{
		char* p = strrchr(path,'.');
		if (!p) return;
		char ext[8]={0};
		strcpy(ext,p);
		strcpy(++p,str);
		strcat(p,ext);
	}

	static void replace_ext(char* path,char const* newext)
	{
		char* p = strrchr(path,'.');
		if (!p) return;
		strcpy(++p,newext);
	}

	static char const* get_filepos(char const* path)
	{
		char const* p1 = strrchr(path,'/');
		char const* p2 = strrchr(path,'\\');
		return p1>p2?p1:p2;
	}
	static char const* get_ext(char const* path)
	{
		char const* p1 = strrchr(path,'.');
		return p1;
	}
	static void vector_to_01(std::vector<int> const& v,std::string& str)
	{
		for (int i=0;i<v.size();++i)
		{
			char c = '.';
			int n = v[i];
			if (n==0) c='.';
			else if (n>26+10) c='Z';
			else if (n>=10) c=n+'A'-10;
			else if (n>0) c=n+'0';
			str += c;
		}
	}
	static void success_to_intv(std::vector<int> const& v,std::vector<intv_t>& intv)
	{
		int size = v.size();
		int i = 0;
		while (i<size)
		{
			intv_t line(-1,-1);
			/// find 1
			for (;i<size;++i)
				if (v[i]) {line.b=i;break;}
			for  (++i;i<size;++i)
				if (!v[i]) {line.e=i;break;}
			
			if (line.b!=-1)
			{
				if (line.e==-1) line.e = v.size();
				intv.push_back(line);
			}
		}
	}
	static void set_value(std::vector<uchar>& f,intv_t a,uchar v=1)
	{
		if (a.b<0) a.b = 0;
		if (a.e>f.size()) a.e = f.size();
		if (a.e<a.b) return;
		memset(&f[a.b],v,a.size()*sizeof(uchar));
	}
};




//double get_normal_variance(int* array, int size)
//{
//	int sum = 0;
//	for (int i=0; i<size; ++i)
//		sum += array[i];
//	double ava = sum*1.0/size;
//	for (int i=0; i<size; ++i)
//	{
//		_g_array_p[i] = array[i]*1.0/sum;
//	}
//	double e = 1.0/size;
//	double d = 0;
//	for (int i=0; i<size; ++i)
//	{
//		double e0 = _g_array_p[i]-e;
//		e0 *= e0;
//		d += e0;
//	}
//	d /= size;
//	d = sqrt(d);
//	return d;
//}
//int DecodeImage(simple_image_t& image,charset_t const& charSet,char codes[4],float *confidence = 0);






struct complex_t
{
	static int const __degree_cycle = 360;
	static double sinusoidal(int degree)
	{
		static double const __pi = 3.1415926;
		degree = (__degree_cycle+degree%__degree_cycle)%__degree_cycle;
		static double _sinn[__degree_cycle];
		static bool inited = false;
		if (!inited)
		{
			for (int i=0;!inited && i<__degree_cycle;i++)
			{
				_sinn[i] = sin(i*2*__pi/__degree_cycle);
				//if (debug_print::is_printf()) printf ("%d,%.8f\n",i,_sinn[i]);
			}
			inited = true;
		}
		return _sinn[degree];
	}
	static double cosine(int degree)
	{
		degree = (__degree_cycle+degree%__degree_cycle)%__degree_cycle;
		static double _cosn[__degree_cycle];
		static double const __pi = 3.1415926;
		static bool inited = false;
		if (!inited)
		{
			for (int i=0;!inited && i<__degree_cycle;i++)
			{
				_cosn[i] = cos(i*2*__pi/__degree_cycle);
				//if (debug_print::is_printf()) printf ("%d,%.8f\n",i,_cosn[i]);
			}
			inited = true;
		}
		return _cosn[degree];
	}
	static double percent(double a,double b)
	{
		complex_t ac = from_angle(a);
		complex_t bc = from_angle(b);
		complex_t cc = ac-bc;
		return cc.angle();
	}

	static int distance(int c1,int c2)
	{
		int c = c2-c1;
		if (c<0) c = -c;
		if (c>90) c = 180-c;
		return c;
	}

	double rc,is;
	complex_t():rc(0),is(0){}
	complex_t(double _c,double _s):rc(_c),is(_s) {}
	complex_t square() const
	{
		complex_t ret;
		ret.rc = rc*rc-is*is;
		ret.is = rc*is*2;
		return ret;
	}
	complex_t operator-(complex_t const& com) const
	{
		return complex_t(rc-com.rc,is-com.is);
	}
	double randian() const
	{
		return atan(rc/is);
	}
	double angle() const
	{
		static double const __pi = 3.1415926;
		double rd = atan(is/rc);
		rd = rd*180/__pi;
		if (rd>0)
			return is>0?rd:rd+180;
		else if (rd<0)
			return is>0?rd+180:rd+360;
		return rd;
	}
	//complex_t operator*(double k)
	//{
	//	return complex_t(c*k,s*k);
	//}
	//complex_t operator+(complex_t const& com)
	//{
	//	return complex_t(c+com.c,s+com.s);
	//}
	complex_t& operator+= (complex_t const& other)
	{
		rc += other.rc;
		is += other.is;
		return *this;
	}
	complex_t& add_angle(int degree)
	{
		rc += cosine(degree);
		is += sinusoidal(degree);
		return *this;
	}
	static complex_t from_angle_square(int degree)
	{
		static complex_t _g_angle_complex_square[__degree_cycle];
		static bool inited = false;
		if (!inited)
		{
			complex_t ret;
			for (int ang=0;ang<__degree_cycle;++ang)
			{
				complex_t ret;
				double c = cosine(ang);
				double s = sinusoidal(ang);
				ret.rc = c*c-s*s;
				ret.is = sinusoidal(ang*2);
				_g_angle_complex_square[ang] = ret;
			}
			inited = true;
		}
		degree = (__degree_cycle+degree%__degree_cycle)%__degree_cycle;
		return _g_angle_complex_square[degree];
	}
	static complex_t from_angle(int degree)
	{
		return complex_t(cosine(degree),sinusoidal(degree));
	}
};

struct probability
{
	static int const __degree_cycle = 180;
	struct citem
	{
		int i,c;
		double ip,cp;
		citem():i(0),c(0),ip(0),cp(0) {}
	};

	static void get_minmax_of(size_t const* array,size_t size,size_t&_min,size_t&_max)
	{
		size_t themin = -1;
		size_t themax = 0;
		for (size_t i=0;i<size;++i)
		{
			if (array[i]>themax) themax = array[i];
			if (array[i]<themin) themin = array[i];
		}
		_min = themin;
		_max = themax;
	}

	static void get_iminmax_of(size_t const* array,size_t size,size_t&_imin,size_t&_imax)
	{
		size_t themin = -1;
		size_t themax = 0;
		size_t imin=0,imax=0;
		for (size_t i=0;i<size;++i)
		{
			if (array[i]>themax) themax = array[i],imax=i;
			if (array[i]<themin) themin = array[i],imin=i;
		}
		_imax = imax;
		_imin = imin;
	}

	static int get_imax_of(citem const* array,size_t size)
	{
		size_t themax = 0;
		size_t imax = 0;
		for (size_t i=0;i<size;++i)
		{
			if (array[i].c > themax)
			{
				themax = array[i].c;
				imax=i;
			}
		}
		return imax;
	}

	struct myless
	{
		bool operator() (citem const& i,citem const& j)
		{
			return i.i >= j.i;
		}
	};

	static double purity180(int const* array,int size)
	{
		static citem _l_array[256];
		if (size>256)
			size = 256;
		int jj=0;
		for (int i=0,j=0;i<size;i++)
		{
			if (array[i]!=0)
			{
				_l_array[jj].i = i;
				_l_array[jj].c = array[i];
				++jj;
			}
		}
		size = jj;

		int sum=0;
		double sinall=0, cosall=0;
		for (int i=0; i<size; ++i)
		{
			sum += _l_array[i].c;
			sinall += (complex_t::sinusoidal(_l_array[i].i*2)*_l_array[i].c);
			cosall += (complex_t::cosine(_l_array[i].i*2)*_l_array[i].c);
		}
		sinall *= sinall;
		cosall *= cosall;

		double r = sum==0?0:sqrt(sinall+cosall)/sum;

		return r;
	}
	static double purity2(int const* array,int size)
	{
		static citem _l_array[256];
		int jj=0;
		for (int i=0,j=0;i<size;i++)
		{
			if (array[i]!=0)
			{
				_l_array[jj].i = i;
				_l_array[jj].c = array[i];
				++jj;
			}
		}
		size = jj;

		int imax = 0;
		int sum = 0;
		int themax = 0;
		for (int i=0; i<size; ++i)
		{
			sum += _l_array[i].c;
			if (_l_array[i].c>themax) themax=_l_array[i].c,imax = i;
		}
		for (int i=0; i<size; ++i)
		{
			_l_array[i].cp = _l_array[i].c*1.0/sum;
			_l_array[i].i -= imax;
			if (_l_array[i].i<0) _l_array[i].i = -_l_array[i].i;
			else if (_l_array[i].i>90) _l_array[i].i = 180-_l_array[i].i;
			_l_array[i].ip = (180-_l_array[i].i)*1.0/180;
		}

		std::sort(&_l_array[0],(&_l_array[size-1])+1,myless());

		double purity_coeff = _l_array[0].ip*_l_array[0].cp;
		for (int i=1;i<size;++i)
		{
			purity_coeff *= _l_array[i].ip*(1-_l_array[i].cp);
		}
		return purity_coeff;
	}
	static int angle_of_expected_of_colorarray(char const* array,int const size,int const step=1)
	{
		complex_t coall;
		int const end = size*step;
		for (int i=0; i<end; i+=step)
			coall.add_angle(array[i]);
		return coall.angle();
	}
	static double expected(int const* array,int size)
	{
		static citem _l_array[__degree_cycle];
		int jj=0;
		for (int i=0,j=0;i<size;i++)
		{
			if (array[i]!=0)
			{
				_l_array[jj].i = i;
				_l_array[jj].c = array[i];
				++jj;
			}
		}
		size = jj;

		complex_t sum;
		for (int i=0; i<size; ++i)
		{
			sum.add_angle(_l_array[i].i);
		}

		return sum.angle();
	}
	static double expected180(unsigned char const* array,int size)
	{
		static citem _l_array[256];
		int jj=0;
		for (int i=0,j=0;i<size;i++)
		{
			if (array[i]!=0)
			{
				_l_array[jj].i = i;
				_l_array[jj].c = array[i];
				++jj;
			}
		}
		size = jj;

		complex_t sum;
		for (int i=0; i<size; ++i)
		{
			sum.add_angle(_l_array[i].i*2);
		}

		return sum.angle()/2;
	}
	static double variance(int* array, int size)
	{
		static double _g_array_p[256] = {0};
		static citem _l_array[256];
		int jj=0;
		for (int i=0,j=0;i<size;i++)
		{
			if (array[i]!=0)
			{
				_l_array[jj].i = i;
				_l_array[jj].c = array[i];
				++jj;
			}
		}
		size = jj;
		int sum = 0;
		double re=0,im=0;
		complex_t comsum,comsum_square;
		for (int i=0; i<size; ++i)
		{
			sum += _l_array[i].c;
		}
		for (int i=0; i<size; ++i)
		{
			_g_array_p[i] = _l_array[i].c*1.0/sum;
		}
		for (int i=0; i<size; ++i)
		{
			complex_t ims = complex_t::from_angle_square(i);
			complex_t im = complex_t::from_angle(i);
			ims.rc *= _g_array_p[i];
			ims.is *= _g_array_p[i];
			comsum_square.rc += ims.rc;
			comsum_square.is += ims.is;
			comsum.is += im.is;
			comsum.rc += im.rc;
		}
		complex_t imas = comsum.square();
		complex_t ret = comsum_square-imas;
		double radian = ret.is/ret.rc;
		double r = atan(radian);
		return r;
	}
	unsigned char rgbtogray(unsigned char r,unsigned char g,unsigned char b)
	{
		return (r*4898 + r*9618 + r*1868) >> 14;
	}
	void rgbtogray(unsigned char* rgbarray,int size,int channel=3)
	{
		for (int i=0; i<size; i+=channel)
			rgbarray[i] = rgbtogray(rgbarray[0],rgbarray[1],rgbarray[2]);
	}
	unsigned char bgrtogray(unsigned char b,unsigned char g,unsigned char r)
	{
		return (r*4898 + r*9618 + r*1868) >> 14;
	}
	void bgrtogray(unsigned char* rgbarray,int size,int channel=3)
	{
		for (int i=0; i<size; i+=channel)
			rgbarray[i] = bgrtogray(rgbarray[0],rgbarray[1],rgbarray[2]);
	}
};














//static Matcher _g_matcher;

///************************************************************************/
///*                           C Wrappers                                 */
///************************************************************************/
//int MatcherInit(const char* fontData,charfont_t const* charfont_Array,size_t size)
//{
//	return _g_matcher.Init(fontData,charfont_Array,size);
//}
//
//int MatcherInitImage(simple_image_t& image,charfont_t const* charfont_Array,size_t size)
//{
//	return _g_matcher.Init(image,charfont_Array,size);
//}
//
//int MatcherDecode(simple_image_t& image,IplImage* imageHSV,char code[4])
//{
//	//SimpleImage* gray = siCreateImage(image.width,image.height,1);
//	//if (image.channels != 1)
//	//{
//	//	siRgbToGray(image,gray);
//	//}
//	////siThreshold(gray,gray,150,255);
//	//siThreshold(gray,gray,115,255);
//	//siSaveImage("d:/aaa.png",gray);
//
//	//int ret = thiz.Decode(gray,code);
//	//siReleaseImage(&gray);
//	//return ret;
//
//
//	int ret = _g_matcher.Decode(image,imageHSV,code);
//	return ret;
//}
