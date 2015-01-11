

/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../met/_if_switch.h"
#include "../met/tslist.h"
#include "../cxx/tags.h"
#include "mixture_inf_decl.h"
#include "mixture_obj_type.h"
#include "mixture_unitize.h"
#include "mixture_decl.h"
#include "mixture_method_decl.h"
#include "mixture_conv.h"
#include "mixture_type_access.h"
#include "mixture_access.h"



#pragma once



___namespace2_begin(ox,mix)




template
<
	template<size_t>class ssl,size_t ii,
	template<size_t>class tsl,size_t i,
	typename uz,
	typename ms,
	typename edi
>
struct tinf <ssl<ii>,tsl<i>,uz,ms,edi>
{
	typedef ssl<ii> jfeature;
	typedef tsl<i> ifeature;
	typedef ssl<ii-1> jfeature_next;
	typedef ssl<0> jfeature0;
	typedef typename tsl<0>::type ifeature0;

	typedef uz unitize;
	typedef ms metas;
	typedef edi ediff;

	typedef typename ifeature::idiff idiff;
	typedef mixture<ifeature0,uz,ms,edi> owner_type;
	typedef typename type_for_instantiate<ifeature,owner_type>::type value_type;
	typedef typename met::add_ref<value_type>::type reference;
	typedef typename met::add_con<reference>::type const_reference;
	typedef typename met::param_type_for_init<value_type>::type param_type;
	typedef typename methodlmd_apply<jfeature0,ifeature,uz,ms,edi>::type unit_type;
	typedef typename methodlmd_apply<jfeature_next,ifeature,uz,ms,edi>::type base;
	typedef typename met::iff<ii==0||ii==1,void,base>::type _t_next;
	typedef typename met::add_ref<unit_type>::type unit_reference;
	typedef typename met::add_con<unit_type>::type unit_const_reference;


	template <typename method>
	static unit_reference unit(method* m)
	{
		return *static_cast<unit_type*>(m);
	}
	template <typename method>
	static unit_const_reference unit(method const* m)
	{
		return *static_cast<unit_type const*>(m);
	}
	template <typename method>
	static reference value(method* m)
	{
		return unit(m).value();
	}
	template <typename method>
	static const_reference value(method const* m)
	{
		return unit(m).value();
	}
	template <typename method_tn>
	static owner_type* owner(method_tn* m)
	{
		return static_cast<owner_type*>(m);
	}
	template <typename method_tn>
	static owner_type const* owner(method_tn const* m)
	{
		return static_cast<owner_type const*>(m);
	}

	template<typename addr_tn,typename mod_tn=int>
	struct data_typeat
	{
		typedef typename ox::mix::data_typeat<owner_type,addr_tn,mod_tn>::type type;
	};
	/// when method is const?, owner should be const too????, this condition has not done
	template <typename sym,typename method>
	static typename data_typeat<ox::mix::saddr<sym>,int&>::type
	data(method* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return data_addrat<ox::mix::saddr<sym>>()(owner(m));
	}
	template <size_t i,typename method>
	static typename data_typeat<ox::mix::index<i>,int&>::type
	data(method* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return data_addrat<ox::mix::index<i>>()(owner(m));
	}


	template <typename addr_tn,typename mod_tn=int>
	struct method_typeat
	{
		typedef typename ox::mix::method_typeat<owner_type,addr_tn,mod_tn>::type type;
	};
	template <typename sym,typename method_tn>
	static typename method_typeat<ox::mix::saddr<sym>,int&>::type
	method(method_tn* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::saddr<sym>>()(owner(m));
	}
	template <size_t i,typename method_tn>
	static typename method_typeat<ox::mix::index<i>,int&>::type
	method(method_tn& m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::index<i>>()(owner(m));
	}
	template <size_t i,size_t j,typename method_tn,int&>
	static typename method_typeat<ox::mix::index<i,j>>::type
	method(method_tn* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::index<i,j>>()(owner(m));
	}
};



template
<
	template <typename,size_t>class ssl,typename sl,size_t si,
	template <typename,size_t,size_t,typename>class tsl,typename l,size_t i,size_t in,typename r,
	typename uz,
	typename ms,
	typename edi
>
struct tinf <ssl<sl,si>,tsl<l,i,in,r>,uz,ms,edi>
{
	typedef ssl<sl,si> jfeature;
	typedef tsl<l,i,in,r> ifeature;
	typedef typename feature_next<jfeature>::type jfeature_next;
	typedef tmlist<void> jfeature0;
	typedef typename feature0<ifeature>::type ifeature0; /// ----

	typedef uz unitize;
	typedef ms metas;
	typedef edi ediff;

	typedef typename ifeature::idiff idiff;
	typedef mixture<ifeature0,uz,ms,edi> owner_type;
	typedef typename type_for_instantiate<ifeature,owner_type>::type value_type;
	typedef typename met::add_ref<value_type>::type reference;
	typedef typename met::add_con<reference>::type const_reference;
	typedef typename ox::met::param_type_for_init<value_type>::type param_type;
	typedef typename method2lmd_apply<jfeature0,ifeature,uz,ms,edi>::type unit_type;
	typedef typename method2lmd_apply<jfeature_next,ifeature,uz,ms,edi>::type base;
	typedef typename met::iff<ox::met::is_void<typename jfeature_next::_t_next>::value,void,base>::type _t_next;
	typedef typename met::add_ref<unit_type>::type unit_reference;
	typedef typename met::add_con<unit_type>::type unit_const_reference;


	template <typename method>
	static unit_reference unit(method* m)
	{
		return *static_cast<unit_type*>(m);
	}
	template <typename method>
	static unit_const_reference unit(method const* m)
	{
		return *static_cast<unit_type const*>(m);
	}
	template <typename method>
	static reference value(method* m)
	{
		return unit(m).value();
	}
	template <typename method>
	static const_reference value(method const* m)
	{
		return unit(m).value();
	}
	template <typename method_tn>
	static owner_type* owner(method_tn* m)
	{
		return static_cast<owner_type*>(m);
	}
	template <typename method_tn>
	static owner_type const* owner(method_tn const* m)
	{
		return static_cast<owner_type const*>(m);
	}

	template<typename addr_tn,typename mod_tn=int>
	struct data_typeat
	{
		typedef typename ox::mix::data_typeat<owner_type,addr_tn,mod_tn>::type type;
	};
	/// when method is const?, owner should be const too????, this condition has not done
	template <typename sym,typename method>
	static typename data_typeat<ox::mix::saddr<sym>,int&>::type
	data(method* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return data_addrat<ox::mix::saddr<sym>>()(owner(m));
	}
	template <size_t i,typename method>
	static typename data_typeat<ox::mix::index<i>,int&>::type
	data(method* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return data_addrat<ox::mix::index<i>>()(owner(m));
	}


	template <typename addr_tn,typename mod_tn=int>
	struct method_typeat
	{
		typedef typename ox::mix::method_typeat<owner_type,addr_tn,mod_tn>::type type;
	};
	template <typename sym,typename method_tn>
	static typename method_typeat<ox::mix::saddr<sym>,int&>::type
	method(method_tn* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::saddr<sym>>()(owner(m));
	}
	template <size_t i,typename method_tn>
	static typename method_typeat<ox::mix::index<i>,int&>::type
	method(method_tn& m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::index<i>>()(owner(m));
	}
	template <size_t i,size_t j,typename method_tn,int&>
	static typename method_typeat<ox::mix::index<i,j>>::type
	method(method_tn* m)
	{
		//owner_type* mix = static_cast<owner_type*>(m);
		return method_addrat<ox::mix::index<i,j>>()(owner(m));
	}
};



___namespace2_end()
