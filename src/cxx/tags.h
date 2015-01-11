
/*
	file : interval_carray.h
	author : xuhailong
	date: 20012-10-1
	history: the key parts of classification
 */

#include "../ox/nsab.h"
#include "../ox/def_macro.h"




#pragma once




#define ___def_tag(tag) \
struct tag \
{ \
	static char const* to_string() { return ___def_to_string(tag); } \
};


___namespace2_begin(ox,tags)




/// the category of mixture's member
___def_tag(data); /// data member of mixture
___def_tag(method); /// method member of mixture
___def_tag(any); /// unit of owner


/**
	stands for the static array, just like int [10]
	the data allcoated in the stack or global data segment
*/

___def_tag(cate);

/// range type: cate
___def_tag(heterogeneity);
___def_tag(homogeneity);

/// label classes
___def_tag(tag);

/// flag general using
___def_tag(flag);

/// the basic type classes: tag
___def_tag(dot);
___def_tag(range);
___def_tag(node);
___def_tag(allocator);
___def_tag(value);

___def_tag(leaf); /// for tree control

/// for instantiation points to the mixture owner
___def_tag(owner);
/// for mixture
___def_tag(unit);

/// for range : id
___def_tag(self2self);
___def_tag(static_array);
___def_tag(static_vector);
___def_tag(vector);
___def_tag(slist);
___def_tag(list);

/// for layout, for dot using: layout
___def_tag(success);
___def_tag(linelink);
___def_tag(descrete); // the use of memory allocator, at present
___def_tag(self); /// just refer itself
___def_tag(linecompact);


/// for vector range
___def_tag(begin);
___def_tag(bend); /// for discrete
___def_tag(end);
___def_tag(capacity);
//___def_tag(last); /// for discrete
___def_tag(size);

/// for range
___def_tag(is_end);

/// for slist range
___def_tag(next);
___def_tag(prev);

___def_tag(dnext); /// data-member-next


/// for allocator
___def_tag(basic);
//___def_tag(alloc);
//___def_tag(free);
___def_tag(construct);
___def_tag(destruct);

/// for set operation
___def_tag(bound);
___def_tag(foreach);

/// for scan
___def_tag(selfnext);


/// for dot
___def_tag(address);
___def_tag(isolate);
___def_tag(is_empty);
___def_tag(obj);


/// redo the current action
/*
	scan-control(redo)
*/
___def_tag(redo); 

___def_tag(control);

/*
	any container, remove a element but dont destroy the data
*/
___def_tag(remove);

/*
	any container, insert a element into a container
*/
___def_tag(insert);




___namespace2_end()
