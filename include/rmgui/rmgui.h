#ifndef RMGUI_H
#define RMGUI_H


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/* UNDERSTANDING RMGUI


rmgui follows that both state management and rendering are controlled via nodes

		| head |
	 |node1| |node2|

the nodes as both a way to generate a list of draw commands to be sent to the gpu as well as a way of handling state

during creation
nodes are allocated ON the array before a reference is sent to the tree 
the id corisponds to the index in the array

nodes also can hold reference to state to directly manipulate in functionland via a void*
nodes have four callbacks to set state

on_hover;
on_press;
on_release;
on_hold;

*/


// === Forward Declarations ===


typedef struct gui_str gui_str;
typedef struct gui_text gui_text;

// === data related to layouts ===
typedef struct gui_input gui_input;
typedef struct gui_float_elem gui_float_elem;
typedef struct gui_clip gui_clip;
typedef struct gui_border gui_border;
typedef struct gui_img gui_img;

typedef struct gui_dock_layout gui_dock_layout;
typedef struct gui_dock_elem gui_dock_elem;

typedef struct gui_children_layout gui_children_layout;
typedef struct gui_parent_layout gui_parent_layout;

typedef struct gui_scroll_data gui_scroll_data;

// === core info ===
typedef struct gui_node gui_node;
typedef struct gui_core gui_core;

typedef struct gui_node_id gui_node_id;

typedef struct gui_transform gui_transform;

// === render info ===

// render commands
typedef struct gui_ren_img  gui_ren_img;
typedef struct gui_ren_custom gui_ren_custom;

typedef struct gui_ren_cmd gui_ren_cmd;

// === unions ===
typedef union gui_rect gui_rect;
typedef union gui_color gui_color;

typedef union gui_mat_intern gui_mat3;
typedef union gui_vec3_intern gui_vec3;


// === Basic Unions ===
union gui_color
{
	struct { float r, g, b, a; } rgba;
	float color[4];
};

union gui_rect
{
	struct { uint32_t l, r, u, d; } sides;
	uint32_t lrud[4];
};

union gui_mat_intern
{
	float mat[3][3];
	struct {
		float x1, y1, z1, 
			  x2, y2, z2, 
			  x3, y3, z3;
	} rc;
};

union gui_vec3_intern
{
	float vec[3];
	struct {float x,y,z;} pos;
};


// === Enums ===
typedef enum 
{
	GUI_MAIN,
	GUI_SUB,
	GUI_BUTTON,
	GUI_TEXT,
} gui_type;

typedef enum 
{
	GUI_LAYOUT_TOP_BOT,
	GUI_LAYOUT_LEFT_RIGHT,
} gui_layout_type;


typedef enum
{
	GUI_POINT_TOP_LEFT,
	GUI_POINT_TOP_RIGHT,
	GUI_POINT_LEFT,
	GUI_POINT_RIGHT,
	GUI_POINT_TOP,
	GUI_POINT_BOTTOM,
	GUI_POINT_BOTTOM_LEFT,
	GUI_POINT_BOTTOM_RIGHT,
} gui_rect_pos;

typedef enum
{
	GUI_NODE_IS_ACTIVE 		= 1 << 0,
	GUI_NODE_IS_RENDERED 	= 1 << 1,
	GUI_NODE_IS_ACTIONABLE = 1 << 2,
	GUI_NODE_CAN_CHILD 		= 1 << 3,	
	GUI_NODE_IS_HEAD 			= 1 << 4 
} gui_node_flag;

typedef enum
{
	GUI_HEAD_IS_RENDERED,
	GUI_INTERMEDIATE_MODE,
} gui_core_flags;


// === Function Pointer Typedefs ===
typedef void (*gui_action)(void*);

typedef void (*gui_render)(gui_transform* transform);
typedef void (*gui_set_layout)(gui_parent_layout* parent, gui_children_layout* child, gui_transform* transform);


// === Struct Definitions ===


struct gui_transform
{
	gui_mat3 transform;

	gui_vec3 position; // z represents depth in this case 
	float rotate;
	float scale_x, scale_y;
};


struct gui_input 
{
	void* temp;
};

struct gui_text 
{
	gui_str* string;
	gui_color* color;
	int font_size;
	int letter_spacing;
	int line_height;
	uint32_t wrap_mode;
};

struct gui_str // includes null terminator
{
	uint32_t size;
	char* string;
};


struct gui_dock_layout
{
	uint8_t number_of_splits;	// a max of 4 can occur at a time
	uint8_t docked_index;			// tracks docked indexes via the first four bits
};

struct gui_dock_elem
{
	bool is_locked;
	bool is_resizable;
	
	uint8_t dock_index;
	uint32_t dockmode_window_size[2];
};

struct gui_float_elem 
{
	struct {int x,y;} offset;
	struct {int x,y;} expand;
	int zindex;
	gui_rect_pos corner_from;
	gui_rect_pos corner_to;
};

struct gui_clip 
{
	uint16_t horizontal, vertical;
};

struct gui_img 
{
	uint32_t width, height, bitdepth;
	void* image;
};


struct gui_ren_img 
{
	uint32_t width, height, bitdepth;
	void* image;
};
struct gui_ren_custom 
{
	void* custom_data;
	void* (*render_cmd)(void);
};

struct gui_ren_cmd
{
	uint32_t command;
	
	gui_transform transformation;
	gui_color color;
	union
	{
		gui_rect rect_cmd;
		gui_str text_cmd;
		gui_ren_img img_cmd;
		gui_ren_custom cust_cmd;
	} cmd;
};


// this configures the current node
struct gui_parent_layout
{
	bool is_dockable;
	
	gui_color* color;
	gui_rect* radius;

	float* aspect_ratio;

	gui_img* image;

	gui_float_elem* floating;	// IF NOT FLOATING SET TO NULL PLEASE
	
	void* custom;
	void* usr_data;
};

// these configure the children 

struct gui_children_layout 
{
	
	gui_layout_type* layout_dir;
	gui_rect* padding;
	
	uint32_t* children_gap;
	uint32_t* alignment;
	uint32_t* sizing;

	gui_clip* clip;
	
	void* custom;
	void* usr_data;
};

struct gui_scroll_data 
{	
	float *scroll_x, *scroll_y;
};

struct gui_node_id
{
	uint32_t id;
	gui_str string;

	gui_node_id* parent;

	uint32_t children_count;
	gui_node_id* children;
};


// id 0 is reserved for the head
struct gui_node 
{
	gui_node_id id;
	
	uint32_t flags;
	
	void* data;			// the state passed between each system
	
	gui_text* text;

	// if text is null, these must be null
	// all these do are dictate rules for how core_transform works under the hood
	gui_parent_layout* parent_lay;
	gui_children_layout* child_lay;
	gui_scroll_data* scroll_data;

	// true transformation
	gui_transform* core_transformation;

	struct
	{
		gui_action on_press;	// can interact with the renderer for both parent and imediate children
		gui_action on_hover;	// can interact with the renderer for both parent and imediate children
		gui_action on_hold;	// can interact with the renderer for both parent and imediate children
		gui_action on_release;	// can interact with the renderer for both parent and imediate children
	} actions;
	
	gui_render render;
	gui_set_layout pos;

	uint32_t node_count;
	uint32_t node_reserve;
	
	gui_node* nodes;
	gui_node* parent;
};

struct gui_core 
{
	uint32_t flags;
	
	uint32_t node_count;
	uint32_t node_reserve;

	void* data;
	
	gui_node** node_array; // an array of pointers, we are not copying just holding a reference basically
	
	gui_node* head;
	gui_input* input;
};

gui_core initialize_gui(void);
void destruct_gui(void);

// if color is set to null then it will not render itself
// if not it is considered its own window
// for example create_head({}, NULL, NULL, NULL, NULL)
gui_node* guin_create_head( gui_parent_layout* layout, gui_children_layout* child_layout, gui_scroll_data* scroll, gui_action actions[4]);
gui_node* guin_create_node(gui_node* parent, gui_parent_layout* layout, gui_children_layout* child_layout, gui_scroll_data* scroll, gui_action actions[4]);
gui_node* guin_create_node_txt(gui_str string, gui_text* layout);


gui_node* guic_init(gui_core* core, gui_node* node, uint32_t flags);
gui_node* guic_add_node(gui_core* core, gui_node* node, gui_str name);

void gui_dest_core(gui_core* core);

// handles the null esc char for you
static inline gui_str gui_string(const char* string, size_t len)
{
	return (gui_str){len++, (char*)string};
}


#endif

