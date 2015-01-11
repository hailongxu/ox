
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../met/tslist.h"
#include "../met/mod.h"
#include "../met/static_assert.h"
#include "../cxx/tags.h"
#include "./mixture_type_access.h"
#include "./mixture_decl.h"
#include "./mixture_is.h"


#pragma once


___namespace2_begin(ox,mix)



/*
	we use the same struct to get type and instance
*/


template <typename addr_tn>
struct owner_addrat
{
	/// in theory(本来), return_type_at just receive the non-const type,
	/// but in the process of compilling, the inputed mixture obj will match
	/// all the functions, after this, the complling can catch the matched function
	/// so the return_type_at will be probablily instantiated.


	/// mixture_tn* <==> mixture_tn*const
	/// mixture_tn const* <==> mixture_tn const*const
	/// so we do not deal with the condition of *const 


	typedef typename met::rmv_ref<addr_tn>::type addr0;
	static bool const __is_ptr = met::is_sptr<addr0>::value;
	typedef typename met::rmv<int const*const>::from<addr0>::type addr;
	typedef typename met::add<addr_tn>::to<int>::type mods;
	/// ensure we donot return value, just pointer or reference
	typedef typename met::iff<__is_ptr,met::rmv_ref<mods>,met::add_ref<mods>>::type::type modifier;


	template <bool isptr> struct ret
	{
		template <typename t> t& operator()(t* obj) { return *obj; }
	};
	template <> struct ret <true>
	{
		template <typename t> t* operator()(t* obj) { return obj; }
	};

	template <typename mixture_tn,typename mod=modifier>
	struct typeat
	{
		typedef typename owner_typeat<mixture_tn,addr,mod>::type type;
	};

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename owner_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>& obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename owner_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const& obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ptr
	template <typename sl,typename uz,typename ms,typename e>
	typename owner_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>* obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	template <typename sl,typename uz,typename ms,typename e>
	typename owner_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const* obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	/// any type
	template <typename mixture_tn>
	typename owner_typeat<mixture_tn,addr,modifier>::type
	operator()(mixture_tn* obj) const
	{
		typedef typename owner_typeat<mixture_tn,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}
};

template <size_t index>
struct owner_indexat
	: owner_addrat<ox::met::index<index,index>>
{};

template <typename id, size_t offset=0>
struct owner_labelat
	: owner_addrat<ox::met::saddr<id,offset>>
{};

template <size_t offset=0>
struct owner_unlabelat
	: owner_addrat<ox::met::saddr<void,offset>>
{};

//// function
template <typename sym,typename mixture>
typename owner_typeat<mixture,ox::mix::saddr<sym>>::type
owner(mixture& mix)
{
	return owner_addrat<ox::mix::saddr<sym>>()(mix);
}
template <typename sym,size_t i,typename mixture>
typename owner_typeat<mixture,ox::mix::saddr<sym,i>>::type
owner(mixture& mix)
{
	return owner_addrat<ox::mix::saddr<sym,i>>()(mix);
}
template <size_t i,typename mixture>
typename owner_typeat<mixture,ox::mix::index<i>>::type
owner(mixture& mix)
{
	return owner_addrat<ox::mix::index<i>>()(mix);
}
template <size_t i,size_t j,typename mixture>
typename owner_typeat<mixture,ox::mix::index<i,j>>::type
owner(mixture& mix)
{
	return owner_addrat<ox::mix::index<i,j>>()(mix);
}

/// access the xth element, and return the xth element reference

template <typename addr_tn>
struct data_addrat
{
	typedef typename met::rmv_ref<addr_tn>::type addr0;
	static bool const __is_ptr = met::is_sptr<addr0>::value;
	typedef typename met::rmv<int const*const>::from<addr0>::type addr;
	typedef typename met::add<addr_tn>::to<int>::type mods;
	/// ensure we donot return value, just pointer or reference
	typedef typename met::iff<__is_ptr,met::rmv_ref<mods>,met::add_ref<mods>>::type::type modifier;


	template <bool isptr> struct ret
	{
		template <typename t> t& operator()(t& obj) { return obj; }
	};
	template <> struct ret <true>
	{
		template <typename t> t* operator()(t& obj) { return &obj; }
	};

	template <typename mixture_tn,typename mod=modifier>
	struct typeat
	{
		typedef typename data_typeat<mixture_tn,addr,mod>::type type;
	};

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename data_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>& obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj)->unit().value());
	}

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename data_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const& obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj)->unit().value());
	}

	/// mixture type ptr
	template <typename sl,typename uz,typename ms,typename e>
	typename data_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>* obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj)->unit().value());
	}

	template <typename sl,typename uz,typename ms,typename e>
	typename data_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const* obj) const
	{
		typedef typename owner_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj)->unit().value());
	}

	///// any type
	//template <typename mixture_tn>
	//typename data_typeat<mixture_tn,addr,int*>::type
	//operator()(mixture_tn* obj) const
	//{
	//	typedef typename owner_typeat<mixture_tn,addr,modifier>::type mixture_pointer;
	//	return ret<__is_ptr>()(static_cast<mixture_pointer>(obj)->value());
	//}
};

template <size_t index>
struct data_indexat
	: data_addrat<ox::met::index<index>>
{};

template <typename id, size_t offset=0>
struct data_labelat
	: data_addrat<ox::met::saddr<id,offset>>
{};

template <size_t offset=0>
struct data_unlabelat
	: data_addrat<ox::met::saddr<void,offset>>
{};

//// the useful working of function method

template <typename sym,typename mixture>
typename data_typeat<mixture,ox::mix::saddr<sym>,int&>::type
data(mixture& mix)
{
	return data_addrat<ox::mix::saddr<sym>>()(mix);
}
template <size_t i,typename mixture>
typename data_typeat<mixture,ox::mix::index<i>,int&>::type
data(mixture& mix)
{
	return data_addrat<ox::mix::index<i>>()(mix);
}
/// for the data just has first dimention
//template <size_t i,size_t j,typename mixture>
//typename data_typeat<mixture,ox::mix::index<i,j>>::type
//data(mixture& mix)
//{
//	return data_addrat<ox::mix::index<i,j>>()(mix);
//}


///
/// access the xth unit, and return the xth unit reference

template <typename addr_tn>
struct unit_addrat
{
	typedef typename met::rmv_ref<addr_tn>::type addr0;
	static bool const __is_ptr = met::is_sptr<addr0>::value;
	typedef typename met::rmv<int const*const>::from<addr0>::type addr;
	typedef typename met::add<addr_tn>::to<int>::type mods;
	/// ensure we donot return value, just pointer or reference
	typedef typename met::iff<__is_ptr,met::rmv_ref<mods>,met::add_ref<mods>>::type::type modifier;


	template <bool isptr> struct ret
	{
		template <typename t> t& operator()(t* obj) { return *obj; }
	};
	template <> struct ret <true>
	{
		template <typename t> t* operator()(t* obj) { return obj; }
	};

	template <typename mixture_tn,typename mod=modifier>
	struct typeat
	{
		typedef typename unit_typeat<mixture_tn,addr,mod>::type type;
	};

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename unit_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>& obj) const
	{
		typedef typename unit_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename unit_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const& obj) const
	{
		typedef typename unit_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ptr
	template <typename sl,typename uz,typename ms,typename e>
	typename unit_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>* obj) const
	{
		typedef typename unit_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	template <typename sl,typename uz,typename ms,typename e>
	typename unit_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const* obj) const
	{
		typedef typename unit_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	/// any type
	template <typename mixture_tn>
	typename unit_typeat<mixture_tn,addr,modifier>::type
	operator()(mixture_tn* obj) const
	{
		typedef typename unit_typeat<mixture_tn,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}
};
template <size_t index>
struct unit_indexat
	: unit_addrat<ox::met::index<index>>
{};

template <typename id, size_t offset=0>
struct unit_labelat
	: unit_addrat<ox::met::saddr<id,offset>>
{};

template <size_t offset=0>
struct unit_unlabelat
	: unit_addrat<ox::met::saddr<void,offset>>
{};


//// function
template <typename sym,typename mixture>
typename unit_typeat<mixture,ox::mix::saddr<sym>>::type
unit(mixture& mix)
{
	return unit_addrat<ox::mix::saddr<sym>>()(mix);
}
template <typename sym,size_t i,typename mixture>
typename unit_typeat<mixture,ox::mix::saddr<sym,i>>::type
unit(mixture& mix)
{
	return unit_addrat<ox::mix::saddr<sym,i>>()(mix);
}
template <size_t i,typename mixture>
typename unit_typeat<mixture,ox::mix::index<i>>::type
unit(mixture& mix)
{
	return unit_addrat<ox::mix::index<i>>()(mix);
}
template <size_t i,size_t j,typename mixture>
typename unit_typeat<mixture,ox::mix::index<i,j>>::type
unit(mixture& mix)
{
	return unit_addrat<ox::mix::index<i,j>>()(mix);
}


___namespace2_end()






/*
	you can get relavant member reference from part type instantiation
*/

#include "mixture_conv.h"




___namespace2_begin(ox,mix)



//
//template <typename addr_tn>
//struct addrat_from_unit
//{
//	template <typename unit_tn>
//	typename unit_typefrom_unit<unit_tn,addr_tn,int&>::type
//	operator()(unit_tn& obj) const
//	{
//		//typedef typename tul_derivedest<unit_tn>::type derivedest;
//		typedef typename unit_tn::inf::owner derivedest;
//		derivedest* _derivedest = static_cast<derivedest*const>(&obj);
//		return addrat<addr_tn>()(*_derivedest);
//	}
//};
//
//template <typename addr_tn>
//struct addrat_from_unit
//{
//	typedef typename met::rmv<int const*const&>::to<addr_tn>::type addr;
//	typedef typename met::add<addr_tn>::to<int>::type mods;
//	/// ensure we donot return value, just pointer or reference
//	typedef typename
//		met::iff
//		<
//			met::is_same<addr,addr_tn>::value,
//			met::add_ref<mods>,
//			ox::identity<mods>
//		>
//		::type::type
//	modifier;
//
//
//	template <typename mixture_tn,typename mod=modifier>
//	struct typeat
//	{
//		typedef typename unit_typefrom_unit<mixture_tn,addr_tn,mod>::type type;
//	};
//
//	/// mixture type ref
//	template <typename sl,typename ms,typename uz,typename i,typename e>
//	typename unit_typeat<mixture<sl,ms,uz,i,e>,addr,modifier>::type
//	operator()(mixture<sl,ms,uz,i,e>& obj) const
//	{
//		typedef typename unit_typeat<mixture<sl,ms,uz,i,e>,addr,int*>::type mixture_pointer;
//		return static_cast<mixture_pointer>(&obj);
//	}
//
//	/// mixture type ref
//	template <typename sl,typename ms,typename uz,typename i,typename e>
//	typename unit_typeat<mixture<sl,ms,uz,i,e> const,addr,modifier>::type
//	operator()(mixture<sl,ms,uz,i,e> const& obj) const
//	{
//		typedef typename unit_typeat<mixture<sl,ms,uz,i,e> const,addr,int*>::type mixture_pointer;
//		return static_cast<mixture_pointer>(&obj);
//	}
//
//	/// mixture type ptr
//	template <typename sl,typename ms,typename uz,typename i,typename e>
//	typename unit_typeat<mixture<sl,ms,uz,i,e>,addr,modifier>::type
//	operator()(mixture<sl,ms,uz,i,e>* obj) const
//	{
//		typedef typename unit_typeat<mixture<sl,ms,uz,i,e>,addr,int*>::type mixture_pointer;
//		return static_cast<mixture_pointer>(obj);
//	}
//
//	template <typename sl,typename ms,typename uz,typename i,typename e>
//	typename unit_typeat<mixture<sl,ms,uz,i,e> const,addr_tn,modifier>::type
//	operator()(mixture<sl,ms,uz,i,e> const* obj) const
//	{
//		typedef typename unit_typeat<mixture<sl,ms,uz,i,e> const,addr_tn,int*>::type mixture_pointer;
//		return static_cast<mixture_pointer>(obj);
//	}
//
//	/// any type
//	template <typename mixture_tn>
//	typename unit_typeat<mixture_tn,addr_tn,int*>::type
//	operator()(mixture_tn* obj) const
//	{
//		typedef typename unit_typeat<mixture_tn,addr_tn,int*>::type mixture_pointer;
//		return static_cast<mixture_pointer>(obj);
//	}
//};
//




___namespace2_end()






/*
	get method object
*/


___namespace2_begin(ox,mix)





template <typename addr_tn>
struct method_addrat
{
	typedef typename met::rmv_ref<addr_tn>::type addr0;
	static bool const __is_ptr = met::is_sptr<addr0>::value;
	typedef typename met::rmv<int const*const>::from<addr0>::type addr;
	typedef typename met::add<addr_tn>::to<int>::type mods;
	/// ensure we donot return value, just pointer or reference
	typedef typename met::iff<__is_ptr,met::rmv_ref<mods>,met::add_ref<mods>>::type::type modifier;


	template <bool isptr> struct ret
	{
		template <typename t> t& operator()(t* obj) { return *obj; }
	};
	template <> struct ret <true>
	{
		template <typename t> t* operator()(t* obj) { return obj; }
	};

	template <typename mixture_tn,typename mod=modifier>
	struct typeat
	{
		typedef typename method_typeat<mixture_tn,addr,mod>::type type;
	};

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename method_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>& obj) const
	{
		typedef typename method_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ref
	template <typename sl,typename uz,typename ms,typename e>
	typename method_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const& obj) const
	{
		typedef typename method_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(&obj));
	}

	/// mixture type ptr
	template <typename sl,typename uz,typename ms,typename e>
	typename method_typeat<mixture<sl,uz,ms,e>,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e>* obj) const
	{
		typedef typename method_typeat<mixture<sl,uz,ms,e>,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	template <typename sl,typename uz,typename ms,typename e>
	typename method_typeat<mixture<sl,uz,ms,e> const,addr,modifier>::type
	operator()(mixture<sl,uz,ms,e> const* obj) const
	{
		typedef typename method_typeat<mixture<sl,uz,ms,e> const,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}

	/// any type
	template <typename mixture_tn>
	typename method_typeat<mixture_tn,addr,modifier>::type
	operator()(mixture_tn* obj) const
	{
		typedef typename method_typeat<mixture_tn,addr,int*>::type mixture_pointer;
		return ret<__is_ptr>()(static_cast<mixture_pointer>(obj));
	}
};



template <typename sym,typename mixture>
typename method_typeat<mixture,ox::mix::saddr<sym>,int&>::type
method(mixture& mix)
{
	return method_addrat<ox::mix::saddr<sym>>()(mix);
}
template <size_t i,typename mixture>
typename method_typeat<mixture,ox::mix::index<i>,int&>::type
method(mixture& mix)
{
	return method_addrat<ox::mix::index<i>>()(mix);
}
template <size_t i,size_t j,typename mixture>
typename method_typeat<mixture,ox::mix::index<i,j>,int&>::type
method(mixture& mix)
{
	return method_addrat<ox::mix::index<i,j>>()(mix);
}



___namespace2_end()