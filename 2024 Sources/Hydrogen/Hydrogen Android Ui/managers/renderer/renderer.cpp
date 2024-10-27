#define STB_IMAGE_IMPLEMENTATION
#include "renderer.hpp"

#include <string>
#include <format>
#include <thread>

#include <managers/managers.hpp>
#include <dependencies/imgui/stb_image.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#pragma comment(lib, "opengl32.lib")

static void glfw_error_callback( int error, const char* description )
{
    fprintf( stdout, "Glfw Error %d: %s\n", error, description );
}

void renderer_thread( ) {
    // Setup window
    glfwSetErrorCallback( glfw_error_callback );
    if ( !glfwInit( ) )
    {
        printf( "failed to initiate glfw.\n" );
        managers::request_exit( );
        return;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );  // 3.2+ only
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow( 1280, 720, "Hydrogen User Interface for Windows & Android", NULL, NULL );
    if ( window == NULL ) {
        printf( "failed to create a glfw window.\n" );
        managers::request_exit( );
        return;
    }
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 ); // Enable vsync

    GLenum err = glewInit( );
    if ( err != GLEW_OK )
    {
        fprintf( stdout, "Glew Error: %s\n", glewGetErrorString( err ) );
        managers::request_exit( );
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); ( void )io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark( );

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( glsl_version );

    managers::menu_mgr.initiate( );

    // Our state
    ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

    // Main loop
    while ( !glfwWindowShouldClose( window ) && !managers::should_exit() )
    {
        glfwPollEvents( );

        ImGui_ImplOpenGL3_NewFrame( );
        ImGui_ImplGlfw_NewFrame( );
        ImGui::NewFrame( );
        managers::menu_mgr.render( );        

        // Rendering
        ImGui::Render( );
        int display_w, display_h;
        glfwGetFramebufferSize( window, &display_w, &display_h );
		managers::renderer_mgr.width = display_w;
		managers::renderer_mgr.height = display_h;
        glViewport( 0, 0, display_w, display_h );
        glClearColor( clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

        glfwSwapBuffers( window );
    }

    // Cleanup
    managers::menu_mgr.dispose( );
    ImGui_ImplOpenGL3_Shutdown( );
    ImGui_ImplGlfw_Shutdown( );
    ImGui::DestroyContext( );

    glfwDestroyWindow( window );
    glfwTerminate( );

    managers::request_exit( );

    return;
}

void managers::renderer_manager_t::initiate( )
{
	std::thread{ renderer_thread }.detach( );
}

bool managers::renderer_manager_t::load_texture( const char* data, std::size_t sz, GLuint* out_tex, int* out_width, int* out_height )
{
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory( reinterpret_cast< const unsigned char* >( data ), sz, &image_width, &image_height, NULL, 4 );
    if ( image_data == NULL )
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures( 1, &image_texture );
    glBindTexture( GL_TEXTURE_2D, image_texture );

    // Setup filtering parameters for display
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // This is required on WebGL for non power-of-two textures (android)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // Same  (android)

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
#endif
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
    stbi_image_free( image_data );

    *out_tex = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

float managers::renderer_manager_t::get_display_density( )
{
    return 1.0f;
}

float managers::renderer_manager_t::epoch( )
{
    return static_cast< float >( clock( ) ) / CLOCKS_PER_SEC;
}
