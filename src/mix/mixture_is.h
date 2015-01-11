
/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 2009-3-24
 */

#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../met/lambda0.h" /// for methodof_meta
#include "./mixture_decl.h"

#pragma once


___namespace2_begin(ox,mix)


template <typename type_tn>
struct is_mixture
{
	static bool const value = false;
};
template
<
	typename sl,
	typename uz,
	typename ms,
	typename edi//,
	//typename idi
>
struct is_mixture <mixture<sl,uz,ms,edi/*,idi*/>>
{
	static bool const value = true;
};

template <typename type_tn>
struct is_atom_of_mixture
{
private:
	typedef typename met::atom<type_tn>::type mixture_type;

public:
	static bool const value = is_mixture<mixture_type>::value;
};

template <typename type_tn>
struct mixture_to_slist
{
	typedef type_tn type;
};
template
<
	typename sl,
	typename uz,
	typename ms,
	typename edi//,
	//typename idi
>
struct mixture_to_slist <mixture<sl,uz,ms,edi/*,idi*/>>
{
	typedef sl type;;
};




___namespace2_end()
