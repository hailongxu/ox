
/// define from the interval

#pragma once


/// ֻ��Ϊ�˿������������


//#define ___namespace_begin(ns) \
//	___namespace1_begin(ns)
//
//
//#define ___namespace_end() \
//	___namespace1_end()



#define ___namespace1_begin(ns1) \
namespace ns1 \
{ 


#define ___namespace1_end() \
}


/// namespace 2

#define ___namespace2_begin(ns1,ns2) \
namespace ns1 \
{ \
	namespace ns2 \
	{


#define ___namespace2_end() \
	} \
}


/// ֻ��Ϊ�˿������������

#define ___namespace3_begin(ns1,ns2,ns3) \
namespace ns1 \
{ \
	namespace ns2 \
	{ \
		namespace ns3 \
		{


#define ___namespace3_end() \
		} \
	} \
}


#define ___namespace4_begin(ns1,ns2,ns3,ns4) \
namespace ns1 \
{ \
	namespace ns2 \
	{ \
		namespace ns3 \
		{ \
			namespace ns4 \
			{


#define ___namespace4_end() \
			} \
		} \
	} \
}


#define ___namespace_begin(a,b,c,d) namespace a{ namespace b{ namespace c{ namespace d{
#define ___namespace_end() }}}}


/// �겻��ѭ���ݹ��⣬���ô��Ϊɶ��������ѽ���껹�������أ�������

#define ___namespace_of_met_beign ___namespace2_begin(ox,met)
#define ___namespace_of_met_end ___namespace2_end()

#define ___namespace_of_act_beign ___namespace3_begin(ox,cxx,act)
#define ___namespace_of_act_end ___namespace3_end()

#define ___namespace_of_rng_beign ___namespace3_begin(ox,cxx,rng)
#define ___namespace_of_rng_end ___namespace3_end()

#define ___namespace_of_nod_beign ___namespace3_begin(ox,cxx,nod)
#define ___namespace_of_nod_end ___namespace3_end()