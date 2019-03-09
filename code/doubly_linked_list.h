#ifndef doubly_linked_list_h
#define doubly_linked_list_h value
#define generate_doubly_linked_list(Type, type)\
struct Type##_node\
{\
	Type##_node* next;\
	Type##_node* prev;\
	Type type;\
};

#define iter_doubly_linked_list_node(sentinel)\
	for(auto node=sentinel->next;node!=sentinel;node=node->next)


#define iter_doubly_linked_list_with_name(sentinel,name)\
	for(auto name = sentinel->next; name !=sentinel; name = name ->next)

#define iter_doubly_linked_list_name(sentinel,name) iter_doubly_linked_list_with_name(sentinel,name)

#define iter_doubly_linked_list_node_back(sentinel)\
	for(auto node=sentinel->prev;node!=sentinel;node=node->prev)


#define iter_doubly_linked_list_with_name_back(sentinel,name)\
	for(auto name = sentinel->prev; name !=sentinel; name = name ->prev)

#define iter_doubly_linked_list_name_back(sentinel,name) iter_doubly_linked_list_with_name(sentinel,name)

#define connect_node(node,to_connect)\
    node->next=to_connect->next;\
      to_connect->next->prev=node;\
      to_connect->next=node;\
      node->prev=to_connect;

#define connect_node_backward(node,to_connect)\
    node->prev=to_connect->prev;\
      to_connect->prev->next=node;\
      to_connect->prev=node;\
      node->next=to_connect;

#define disconnect_node(node)\
      node->next->prev=node->prev;\
      node->prev->next=node->next;

#define connect_from_one_list_to_another_and_proceed(node, list)\
    auto next_node=node->prev;\
    disconnect_node(node);\
    connect_node(node,list);\
    node=next_node;

#define connect_from_one_list_to_another(node, list)\
    disconnect_node(node);\
    connect_node(node,list);



#define allocate_or_find_old_one(node, unused_sentinel, used_sentinel,MemoryBuffer)\
	if(unused_sentinel->next!=unused_sentinel)\
	{\
		node=unused_sentinel->next;\
		disconnect_node(node);\
	}\
	else\
	{\
		void** x=(void**)&node;\
		*x=_push_struct(MemoryBuffer,sizeof(*node));\
		*node={};\
	}\
	connect_node(node,used_sentinel);

#define allocate_node(node, used_sentinel,MemoryBuffer)\
{\
	void** x=(void**)&node;\
	*x=_push_struct(MemoryBuffer,sizeof(*node));\
	*node={};\
	connect_node(node,used_sentinel);\
}
#define dump_everything_to_unused(used_sentinel, unused_sentinel)\
	{auto dll_node=used_sentinel->next;\
		while(dll_node->next!=dll_node)\
		{\
			auto next=dll_node->next;\
			disconnect_node(dll_node);\
			connect_node(dll_node, unused_sentinel);\
			dll_node=next;	\
		}\
	}
#define make_sentinel(a)	a->next=a->prev=a;
#endif
