
// the display connection
xcb_connection_t *conn;


// setup info
const xcb_setup_t* setup;
// the X server's screen
xcb_screen_t *screen;

xcb_atom_t WM_STATE;


/**
 * @brief Get the atom object
 * 
 * @param name 
 * @param atom 
 */
void get_atom(char* name, xcb_atom_t* atom);

/**
 * @brief Get the x props object
 * 
 */
void get_x_props();

/**
 * @brief Create a window object
 * 
 * @param screen 
 * @return xcb_window_t 
 */
xcb_window_t create_window(xcb_screen_t *screen);

/**
 * @brief Create a graphics context object
 * 
 * @param window_id 
 * @return xcb_gcontext_t 
 */
xcb_gcontext_t create_graphics_context(xcb_window_t window_id);

/**
 * @brief TODO: handle events in this function instead of main()
 * 
 */
void handle_events();

