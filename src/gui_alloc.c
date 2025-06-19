#include <rmgui/rmgui.h>


gui_node* guin_create_head( gui_parent_layout* layout, gui_children_layout* child_layout, gui_scroll_data* scroll, gui_action actions[4])
{

	assert(layout != NULL && "LAYOUT MUST BE FIRST IMPLEMENTED");
	gui_node* node = (gui_node*)calloc(1, sizeof(gui_node));	// for safetys sake :skull:
	if (!node) return NULL;


	node->parent_lay = layout;
	node->flags |= GUI_NODE_IS_ACTIVE | GUI_NODE_CAN_CHILD | GUI_NODE_IS_HEAD;
	node->text = NULL;

	node->parent = NULL;	// cannot parent a head obviously
	node->nodes = initialize_vec(sizeof(gui_node))

	
	if(layout) node->child_lay = child_layout;
	if(scroll) node->scroll_data = scroll;
	
	if(actions) 
	{
		if (actions[0]) node->actions.on_press 	= actions[0]; 
		if (actions[1]) node->actions.on_hover 	= actions[1]; 
		if (actions[2]) node->actions.on_hold 	 = actions[2];
		if (actions[3]) node->actions.on_release = actions[3];
	}
	//printf("%p\n", &node);
	return node;
}


gui_node* guic_init(gui_core* core, gui_node* node, uint32_t flags)
{
	
	core->flags = flags;

	core->gui_node_array = initialize_vec(sizeof(gui_node*));
	vector_push_back(&core->gui_node_array, node);

	core->head = node;
	core->input = NULL;	/// TEMPORARY DO NOT FORGET LKDFJS:LDSKFJ:

	printf("%p\n", node);
	node->id = (gui_node_id){0, gui_string("head", 4), NULL, 0};
	
	if(core->flags & GUI_HEAD_IS_RENDERED) node->flags |= GUI_NODE_IS_RENDERED;
	
	return node;
}

gui_node* guic_add_node(gui_core* core, gui_node* node, gui_str name)
{
	vector_push_back(&core->gui_node_array, node)
	printf("%p\n", node);
	node->id = (gui_node_id){0, name, NULL, 0};	
	return node;
}

gui_node* guin_create_node(gui_node* parent, gui_parent_layout* layout, gui_children_layout* child_layout, gui_scroll_data* scroll, gui_action actions[4])
{
	assert(layout != NULL && "LAYOUT MUST BE IMPLEMENTED");
	assert(parent != NULL && "A NODE MUST BE ATTACHED TO SOME SORT OF PARENT");
	
	gui_node* node = (gui_node*)calloc(1, sizeof(gui_node));	// for safetys sake :skull:

	node->parent_lay = layout;
	node->flags |= GUI_NODE_IS_ACTIVE | GUI_NODE_CAN_CHILD | GUI_NODE_IS_ACTIONABLE | GUI_NODE_IS_RENDERED;
	node->text = NULL;
	
	node->node_count = 0;
	node->parent = parent;	// cannot parent a head obviously
	node->nodes = initialize_vec(sizeof(gui_nodes));

	vector_push_back(&parent->nodes, *node); 

	
	if(parent->node_count <= parent->node_reserve) 
	{
		realloc(parent->nodes, sizeof(gui_node) * parent->node_reserve);
		parent->node_reserve += 10;
	}
	

	if(layout) node->child_lay = child_layout;
	if(scroll) node->scroll_data = scroll;
	
	if(actions) 
	{
		if (actions[0]) node->actions.on_press = actions[0]; 
		if (actions[1]) node->actions.on_hover = actions[1]; 
		if (actions[2]) node->actions.on_hold = actions[2];
		if (actions[3]) node->actions.on_release = actions[3];
	}
	
	return node;
};

void gui_dest_core(gui_core* core)
{
    if (!core) return;


		for(int i = 0; i < core->node_count; i++)
		{
			/*
				free as needed as well
			*/
			
			free(core->node_array[i]);
			core->node_array[i] = NULL;
		}

    core->node_array = NULL;
    core->head = NULL;
    
}
