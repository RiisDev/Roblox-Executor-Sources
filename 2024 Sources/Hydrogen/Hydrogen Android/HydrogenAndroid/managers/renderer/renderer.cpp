#include "renderer.hpp"

#include "../managers.hpp"
#include <managers/roblox/environment/drawing/drawing.hpp>

#include <dependencies/imgui/imgui.h>
#include <android/native_window.h>
#include <dependencies/imgui/imgui_impl_android.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <dependencies/imgui/stb_image.h>

#include <dependencies/ghc/filesystem.hpp>
#include <managers/jni/jnihttp.hpp>

namespace fs = ghc::filesystem;

static float dpi_scale = 2.25f;

int on_create( void* ptr, ANativeWindow* wnd ) {	
	auto res = managers::hook_mgr.hooks.gl_context_ctor( ptr, wnd );
	
	static std::once_flag imgui_init;
	std::call_once( imgui_init, [ ] ( ) {
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );

		ImGuiIO& io = ImGui::GetIO( );

		io.IniFilename = NULL;

		ImGui::StyleColorsDark( );		
		// ImGui::GetStyle( ).ScaleAllSizes( 3.f );
	} );

	ImGui_ImplAndroid_Init( );
	ImGui_ImplOpenGL3_Init( );

	managers::menu_mgr.initiate();

	managers::utility_mgr.log<ANDROID_LOG_INFO>( "OpenGL ES Context created | ptr: %p | wnd: %p", ptr, wnd );
	return res;
}

int on_destroy( void* ptr ) {
	managers::menu_mgr.dispose( );

	ImGui_ImplOpenGL3_Shutdown( );
	ImGui_ImplAndroid_Shutdown( );
	
	managers::utility_mgr.log<ANDROID_LOG_INFO>( "OpenGL ES Context destroying | ptr: %p", ptr );
	return managers::hook_mgr.hooks.gl_context_dtor( ptr );
}

void keyboard_check( ) {
	auto& io = ImGui::GetIO( );

	std::unique_lock< std::mutex > locker{ managers::menu_mgr.show_mutex };

	if (managers::menu_mgr.menu_toggled)
	{
		if (managers::menu_mgr.editor.focused && !managers::roblox_mgr.is_keyboard_focused()) 
			managers::roblox_mgr.focus_keyboard(true);
		else if (!managers::menu_mgr.editor.focused && managers::roblox_mgr.is_keyboard_focused())
			managers::roblox_mgr.unfocus_keyboard(true);
	}

	locker.unlock();
}

EGLBoolean on_swap( void* ptr ) {	
	int W, H;
	EGLDisplay display = eglGetCurrentDisplay( );
	EGLSurface surface = eglGetCurrentSurface( EGL_READ );
	eglQuerySurface( display, surface, EGL_WIDTH, &W );
	eglQuerySurface( display, surface, EGL_HEIGHT, &H );
	managers::renderer_mgr.width = static_cast< float >( W );
	managers::renderer_mgr.height = static_cast< float >( H );

	ImGui_ImplOpenGL3_NewFrame( );
	ImGui_ImplAndroid_NewFrame( ImVec2{ managers::renderer_mgr.width, managers::renderer_mgr.height } );
	ImGui::NewFrame( );

	//keyboard_check( );

	for (auto obj : managers::drawing_mgr.drawing_objects) if ( obj ) obj->draw( );
	//managers::menu_mgr.render( );	

	ImGui::Render( );
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) ); 
	
	return managers::hook_mgr.hooks.egl_swapbuffers( ptr );
}

int on_input( void* a1, void* eventId, int eventType, float xPos, float yPos )
{
	auto& io = ImGui::GetIO( );

	const auto relxpos = xPos * dpi_scale;
	const auto relypos = yPos * dpi_scale;

	switch ( eventType ) {
		case 0: // BEGIN
		{
			io.AddMousePosEvent( relxpos, relypos );
			io.AddMouseButtonEvent( 0, true );
			break;
		}
		case 1: // MOVE
		{
			io.AddMousePosEvent( relxpos, relypos );
			break;
		}
		case 2: // END
		{
			io.AddMousePosEvent( relxpos, relypos );
			io.AddMouseButtonEvent( 0, false );
			break;
		}
	}

	if (io.WantCaptureMouse || managers::menu_mgr.menu_toggled)
		return managers::hook_mgr.hooks.native_pass_input(a1, eventId, 2, 0, 0);

	return managers::hook_mgr.hooks.native_pass_input( a1, eventId, eventType, xPos, yPos );
}

int on_text( JNIEnv* a1, int a2, int a3, int a4, jstring str, int a6 ) {
	static int input_last_text_size;
	if (managers::menu_mgr.editor.focused && managers::menu_mgr.game_joined)
	{
		auto& io = ImGui::GetIO( );

		jboolean is_copy;
		const auto cstr = a1->GetStringUTFChars( str, &is_copy );

		if ( strlen( cstr ) > input_last_text_size ) // characters added
			io.AddInputCharactersUTF8( reinterpret_cast< const char* >( reinterpret_cast< std::uintptr_t >( cstr ) + input_last_text_size ) );
		else if ( input_last_text_size > strlen( cstr ) ) // characters removed
		{
			auto remove_count = input_last_text_size - strlen( cstr );

			while ( remove_count-- > 0 ) {
				io.AddKeyEvent( ImGuiKey_Backspace, true );
				io.AddKeyEvent( ImGuiKey_Backspace, false );
			}
		}

		input_last_text_size = strlen( cstr );

		if ( is_copy == JNI_TRUE ) {
			a1->ReleaseStringUTFChars( str, cstr );
		}
		if ( a6 ) {
			io.AddKeyEvent( ImGuiKey_Enter, true );
			io.AddKeyEvent( ImGuiKey_Enter, false );

			return 0;
		}
	}
	else {
		input_last_text_size = 0;		
	}

	return managers::hook_mgr.hooks.native_pass_text( a1, a2, a3, a4, str, a6 );
}

//int on_text( JNIEnv* a1, int a2, int a3, int a4, jstring str, int a6 ) {
//	static int input_last_text_size;
//	if ( managers::menu_mgr.editor.focused && managers::menu_mgr.game_joined )
//	{
//		auto& io = ImGui::GetIO( );
//		
//		jboolean is_copy;
//		const auto cstr = a1->GetStringUTFChars( str, &is_copy );
//
//		//managers::utility_mgr.log<ANDROID_LOG_DEBUG>( "text: %s", cstr );
//		managers::menu_mgr.editor.SetText( reinterpret_cast< const char* >( cstr ) );
//		
//		if ( is_copy == JNI_TRUE ) {
//			a1->ReleaseStringUTFChars( str, cstr );
//		}
//	}
//
//	return managers::hook_mgr.hooks.native_pass_text( a1, a2, a3, a4, str, a6 );
//}

extern ImGuiKey ImGui_ImplAndroid_KeyCodeToImGuiKey( int32_t key_code );
int on_key( int a1, int a2, int a3, int a4, int a5, int a6 ) {
	if ( managers::roblox_mgr.keyboard_focused ) {
		auto& io = ImGui::GetIO( );

		const int input_began = a3 != 0;
		ImGuiKey key = ImGui_ImplAndroid_KeyCodeToImGuiKey( a5 );
		if ( key == ImGuiKey_Enter )
		{

			managers::roblox_mgr.unfocus_keyboard( );
			managers::roblox_mgr.focus_keyboard( );

			return 0;
		}
	}
	return managers::hook_mgr.hooks.native_pass_key( a1, a2, a3, a4, a5, a6 );
}

int on_obscure_stuff( int a1, int a2, int a3, const char* a4 ) {
	auto res = managers::hook_mgr.hooks.some_obscure_shit( a1, a2, a3, a4 );
	if ( !strcmp( a4, "dpiScale" ) ) {
		dpi_scale = *reinterpret_cast< float* >( &res );
		managers::utility_mgr.log<ANDROID_LOG_DEBUG>("%f", dpi_scale);
	}
	return res;
}

void managers::renderer_manager_t::initiate( )
{
	*reinterpret_cast< bool* >( managers::memory_mgr.rebase( offsets::vulkan_disabled ) ) = true;

	managers::hook_mgr.hooks.gl_context_ctor.hook(
		reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::gl_context_ctor ) ), reinterpret_cast< void* >( &on_create )
	);
	
	managers::hook_mgr.hooks.gl_context_dtor.hook(
		reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::gl_context_dtor ) ), reinterpret_cast< void* >( &on_destroy )
	);
	
	managers::hook_mgr.hooks.egl_swapbuffers.hook(
		reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::egl_swapbuffers ) ), reinterpret_cast< void* >( &on_swap )
	);

	//managers::hook_mgr.hooks.native_pass_input.hook(
	//	reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::native_pass_input ) ), reinterpret_cast< void* >( &on_input )
	//);

	//managers::hook_mgr.hooks.native_pass_text.hook(
	//	reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::native_pass_text ) ), reinterpret_cast< void* >( &on_text )
	//);

	//managers::hook_mgr.hooks.native_pass_key.hook(
	//	reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::native_pass_key ) ), reinterpret_cast< void* >( &on_key )
	//);

	managers::hook_mgr.hooks.some_obscure_shit.hook(
		reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::some_obscure_shit ) ), reinterpret_cast< void* >( &on_obscure_stuff )
	);

	managers::utility_mgr.log<ANDROID_LOG_INFO>( "Renderer manager initiated" );
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
	return dpi_scale;
}

float managers::renderer_manager_t::epoch( )
{
	return static_cast< float >( clock( ) ) / CLOCKS_PER_SEC;
}
