#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_glfw.h"
#include "../dependencies/imgui/imgui_impl_opengl3.h"
#include "../dependencies/imgui/font_rubik.hpp"
#include "settings.hpp"
#include "functions.hpp"
#include <stdio.h>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h>
#include <iostream>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback( int error, const char* description )
{
    fprintf( stderr, "Glfw Error %d: %s\n", error, description );
}

BOOL WINAPI CtrlHandler( DWORD fdwCtrlType )
{
    switch ( fdwCtrlType )
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
        /* bad way to close a .exe */
        std::system( "taskkill /f /im ngrok.exe" );
        FreeConsole( );
        exit( -1 );
        break;
    default:
        return FALSE;
    }
}

int main( int, char** )
{
    /* init */
    init( );
    SetConsoleCtrlHandler( CtrlHandler, TRUE );
    ::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );

    glfwSetErrorCallback( glfw_error_callback );
    if ( !glfwInit( ) )
        return 1;

#if __APPLE__
    const char* glsl_version = "#version 150";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#endif

    GLFWwindow* window = glfwCreateWindow( 390, 680, "ngrok-helper", NULL, NULL );
    if ( window == NULL )
        return 1;
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit( ) != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit( ) != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL( ) == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize( );
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize( []( const char* name ) { return ( glbinding::ProcAddress )glfwGetProcAddress( name ); } );
#else
    bool err = false;
#endif
    if ( err )
    {
        fprintf( stderr, "Failed to initialize OpenGL loader!\n" );
        return 1;
    }

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::GetStyle( ).FrameRounding = 4.0f;
    ImGui::GetStyle( ).FramePadding = ImVec2( 5, 5 );
    ImGui::GetStyle( ).GrabRounding = 4.0f;
    ImGui::GetStyle( ).ChildRounding = 4.0f;
    ImGui::GetStyle( ).WindowTitleAlign = ImVec2( 0.5f, 0.5f );

    ImVec4* colors = ImGui::GetStyle( ).Colors;
    colors[ ImGuiCol_Text ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
    colors[ ImGuiCol_WindowBg ] = ImVec4( 0.06f, 0.06f, 0.06f, 0.94f );
    colors[ ImGuiCol_ChildBg ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.00f );
    colors[ ImGuiCol_PopupBg ] = ImVec4( 0.08f, 0.08f, 0.08f, 0.94f );
    colors[ ImGuiCol_Border ] = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
    colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ ImGuiCol_FrameBg ] = ImVec4( 0.20f, 0.21f, 0.22f, 0.54f );
    colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.40f, 0.40f, 0.40f, 0.40f );
    colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.18f, 0.18f, 0.18f, 0.67f );
    colors[ ImGuiCol_TitleBg ] = ImVec4( 0.04f, 0.04f, 0.04f, 1.00f );
    colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.29f, 0.29f, 0.29f, 1.00f );
    colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.51f );
    colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
    colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.02f, 0.02f, 0.02f, 0.53f );
    colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
    colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
    colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
    colors[ ImGuiCol_CheckMark ] = ImVec4( 0.94f, 0.94f, 0.94f, 1.00f );
    colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
    colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
    colors[ ImGuiCol_Button ] = ImVec4( 0.44f, 0.44f, 0.44f, 0.40f );
    colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.46f, 0.47f, 0.48f, 1.00f );
    colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.42f, 0.42f, 0.42f, 1.00f );
    colors[ ImGuiCol_Header ] = ImVec4( 0.70f, 0.70f, 0.70f, 0.31f );
    colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.70f, 0.70f, 0.70f, 0.80f );
    colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.48f, 0.50f, 0.52f, 1.00f );
    colors[ ImGuiCol_Separator ] = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
    colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.72f, 0.72f, 0.72f, 0.78f );
    colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
    colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.91f, 0.91f, 0.91f, 0.25f );
    colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.81f, 0.81f, 0.81f, 0.67f );
    colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.46f, 0.46f, 0.46f, 0.95f );
    colors[ ImGuiCol_PlotLines ] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
    colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
    colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.73f, 0.60f, 0.15f, 1.00f );
    colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
    colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.87f, 0.87f, 0.87f, 0.35f );
    colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );
    colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
    colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
    colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( glsl_version );

    ImFont* font = ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( const_cast<std::uint8_t*>( font_rubik ), sizeof( font_rubik ), 20.f, NULL );
    IM_ASSERT( font != NULL );

    ImVec4 clear_color = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );

    /* loop */
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );

        ImGui_ImplOpenGL3_NewFrame( );
        ImGui_ImplGlfw_NewFrame( );
        ImGui::NewFrame( );

        std::string ngrok_authtoken;

        static char buf[ 32 ];

        {
            ImGui::SetNextWindowSize( ImVec2( 360, 656 ), ImGuiCond_FirstUseEver );
            ImGui::SetNextWindowPos( ImVec2( 15, 12 ), ImGuiCond_FirstUseEver );
            if ( ImGui::Begin( "hibroering/ngrok-helper", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
            {
                ImGui::Text( "ngrok-helper" );
                ImGui::Text( "get your authtoken at ngrok.com" );

                ImGui::Separator( );

                ImGui::Text( "tunnel" );
                ImGui::PushItemWidth( 200 );
                ImGui::InputInt( "port###port", &settings::port );
                ImGui::PopItemWidth( );
                if ( ImGui::Button( "create a tunnel" ) )
                {
                    if (create_tunnel( settings::file_name, settings::port, settings::region ))
                        get_public_url( );
                }

                ImGui::InputText( "ip###ip", (char*)settings::ip_address.c_str( ), IM_ARRAYSIZE( buf ) );

                if ( ImGui::Button( "get ip (manually)" ) )
                {
                    get_public_url( );
                }
                ImGui::SameLine( );
                if ( ImGui::Button( "copy ip" ) )
                {
                    HWND hwnd = GetDesktopWindow( );
                    to_clipboard( hwnd, settings::ip_address );
                }

                ImGui::Separator( );

                ImGui::Text( "authtoken" );
                ImGui::InputText( "authtoken###authtoken", (char*)settings::authtoken, IM_ARRAYSIZE( settings::authtoken ) );
                if ( ImGui::Button( "set ngrok authtoken" ) )
                {
                    ngrok_authtoken = "ngrok authtoken " + ( std::string )settings::authtoken;

                    const char* execute = ngrok_authtoken.c_str( );

                    std::system( execute );
                }

                ImGui::Separator( );

                ImGui::Text( "region" );
                if ( ImGui::Combo( "tunnel region", &settings::region, settings::regions, sizeof( settings::regions ) / sizeof( *settings::regions ) ) )
                {
                    if ( !write_to_file( settings::file_name, settings::region ) )
                        return false;
                }

                ImGui::Separator( );

                ImGui::Text( "debug stuff" );
                if ( ImGui::Button( "debug window" ) )
                {
                    ::ShowWindow( ::GetConsoleWindow( ), SW_SHOW );
                }
                if ( ImGui::Button( "force close ngrok" ) )
                {
                    std::system( "taskkill /f /im ngrok.exe" );
                }
                if ( ImGui::Button( "close ngrok-helper" ) )
                {
                    exit( -1 );
                }

                ImGui::Separator( );

                ImGui::Text( "made by broering." );
                ImGui::Text( "thanks to ngrok.com to make it possible." );
                ImGui::Text( "github.com/hibroering/ngrok-helper" );
            }

            ImGui::End( );
        }

        ImGui::Render( );
        int display_w, display_h;
        glfwGetFramebufferSize( window, &display_w, &display_h );
        glViewport( 0, 0, display_w, display_h );
        glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

        glfwSwapBuffers( window );
    }

    ImGui_ImplOpenGL3_Shutdown( );
    ImGui_ImplGlfw_Shutdown( );
    ImGui::DestroyContext( );

    glfwDestroyWindow( window );
    glfwTerminate( );

    return 0;
}