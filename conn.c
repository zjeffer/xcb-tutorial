#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// the display connection
xcb_connection_t *conn;

// setup info
const xcb_setup_t* setup;
// the X server's screen
xcb_screen_t *screen;

xcb_atom_t WM_STATE;


void get_atom(char* name, xcb_atom_t* atom){
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, xcb_intern_atom(conn, 0, strlen(name), name), NULL);
    if (reply != NULL) {
		*atom = reply->atom;
	} else {
		*atom = XCB_NONE;
	}
	free(reply);
}

void get_x_props(){
    setup = xcb_get_setup(conn);
    /* 
    printf("[X props] roots_len \t %i\n", setup->roots_len);
    printf("[X props] bitmap_format_scanline_unit \t %i \n", setup->bitmap_format_scanline_unit);
    printf("[X props] bitmap_format_scanline_pad \t %i \n", setup->bitmap_format_scanline_pad);
    printf("[X props] bitmap_format_bit_order \t %i \n", setup->bitmap_format_bit_order);
    printf("[X props] protocol_major_version \t %i \n", setup->protocol_major_version);
    printf("[X props] protocol_minor_version \t %i \n", setup->protocol_minor_version);
    printf("[X props] status \t %i \n", setup->status); 
    */
}

xcb_window_t create_window(xcb_screen_t *screen){
    // create a window id
    xcb_window_t window_id = xcb_generate_id(conn);

    int value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[2];
    value_list[0] = screen->white_pixel;
    value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_KEY_PRESS;

    // create the window
    xcb_create_window(conn, 
        screen->root_depth,
        window_id,
        screen->root,
        0, 0, 100, 100, 1,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        value_mask, 
        value_list);
    return window_id;
}

void handle_events(){

}

int main(){
    conn = xcb_connect(NULL, NULL);

	// if connection failed, exit
	if (xcb_connection_has_error(conn) != 0) {
        printf("Error opening display.\n");
		exit(1);
	}
    printf("Successfully made display connection.\n");

    
#define GETATOM(a) get_atom(#a, &a);
    GETATOM(WM_STATE)
#undef GETATOM

    // printf("WM_STATE: %i\n", WM_STATE);

    // obtain setup info and access the screen
    get_x_props();
    screen = xcb_setup_roots_iterator(setup).data;
    if (screen == NULL){
        printf("Could not access the X server's screen\n");
        exit(1);
    }
    printf("Successfully accessed the X server's screen.\n");
    
    // create a new window
    xcb_window_t window_id = create_window(screen);

    // display the created window
    xcb_map_window(conn, window_id);
    xcb_flush(conn);

    int running = 1;
    xcb_generic_event_t* event;
    while (running && (event = xcb_wait_for_event(conn))){
        xcb_flush(conn);

        int xerr;
        if ((xerr = xcb_connection_has_error(conn)) != 0) {
            printf("[ERROR] The server closed the connection\n");
            running = 0;
        }

        switch(event->response_type){
            case XCB_KEY_PRESS:
                // get keycode
                printf("Keycode: %d\n", ((xcb_key_press_event_t*)event)->detail);
                break;
            case XCB_BUTTON_PRESS:
                printf("Button pressed: %u\n", ((xcb_button_press_event_t*)event)->detail);
                printf("X: %u\n", ((xcb_button_press_event_t*)event)->event_x);
                printf("Y: %u\n", ((xcb_button_press_event_t*)event)->event_y);
                printf("==============\n");
                break;
            case XCB_EXPOSE:
                printf("XCB_EXPOSE");
                break;
        }
        free(event);   

    }


    /* Disconnect from X server */
    xcb_disconnect(conn);
    return 0;
}

