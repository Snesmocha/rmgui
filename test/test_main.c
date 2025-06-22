#include "rmgui/dynstr.h"
#include <string.h>
#include <rmgui/rmgui.h>


struct data_test
{
    int a;
};

int main(void)
{

    gui_core core = {0};
    struct data_test datatest = {.a=5};
    
    gui_node* test = guic_init(
        &core,
        guin_create_head(&(gui_parent_layout){false, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, NULL, NULL, NULL), 
        0);
    test->data = (void*)&datatest;

    guic_add_node(
        &core,
        guin_create_node( 
            test,
            &(gui_parent_layout){false, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, 
            NULL, 
            NULL, 
            NULL
        ),
        initialize_str_noalloc("test", 4)
    );
    gui_dest_core(&core);

    
    return 0;
}
