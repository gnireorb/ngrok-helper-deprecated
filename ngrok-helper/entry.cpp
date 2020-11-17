#include <stdio.h>

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_glfw.h"
#include "../dependencies/imgui/imgui_impl_opengl3.h"
#include "../dependencies/imgui/imgui_rubik.h"
#include "../dependencies/imgui/imgui_stdlib.h"
#include "../dependencies/rapidjson/document.h"
#include "../dependencies/rapidjson/stringbuffer.h"
#include "../dependencies/rapidjson/writer.h"

#include "ngrok.hpp"
#include "util.hpp"
#include "settings.hpp"

using namespace rapidjson;

void SetupImGuiStyle2( );

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
        std::system( "taskkill /f /im ngrok.exe" );
        exit( -1 );
        break;
    default:
        return FALSE;
    }
}

static void load_ngrok_settings_properly( )
{
    Document doc;
    doc.Parse( util::read_file( "settings.json" ).c_str( ) );

    settings::region = doc[ "ngrok_region" ].GetInt( );
    std::cout << "[region]: " << doc[ "ngrok_region" ].GetInt( ) << std::endl;
    settings::port = doc[ "last_port" ].GetInt( );
    std::cout << "[port]: " << doc[ "last_port" ].GetInt( ) << std::endl << std::endl;
}

int main( int, char** )
{
    /* init */
    ngrok::init( );
    load_ngrok_settings_properly( );
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

    SetupImGuiStyle2( );

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( glsl_version );

    ImFont* font = ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( const_cast<std::uint8_t*>( rubik ), sizeof( rubik ), 20.f, NULL );
    IM_ASSERT( font != NULL );

    ImVec4 clear_color = ImVec4( 0.039f, 0.066f, 0.105f, 1.00f );

    /* loop */
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );

        ImGui_ImplOpenGL3_NewFrame( );
        ImGui_ImplGlfw_NewFrame( );
        ImGui::NewFrame( );

        static std::string ip = "tunel inexistente.";
        static std::string authtoken;

        static bool debug_mode = false;

        {
            ImGui::SetNextWindowSize( ImVec2( 360, 656 ), ImGuiCond_FirstUseEver );
            ImGui::SetNextWindowPos( ImVec2( 15, 12 ), ImGuiCond_FirstUseEver );
            if ( ImGui::Begin( "ngrok-helper", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
            {
                ImGui::Text( "ngrok-helper" );
                ImGui::Separator( );
                ImGui::Text( "tunnel" );
                ImGui::PushItemWidth( 200 );
                if ( ImGui::InputInt( "port", &settings::port ) )
                {
                    Document doc;
                    doc.Parse( util::read_file( "settings.json" ).c_str() );
                    Value& var = doc[ "last_port" ];
                    var.SetInt( settings::port );
                    StringBuffer buffer;
                    Writer<StringBuffer> writer( buffer );
                    doc.Accept( writer );
                    util::write_to_file( "settings.json", buffer.GetString( ) );
                }
                ImGui::PopItemWidth( );
                if ( ImGui::Button( "create a tunnel" ) )
                {
                    ngrok::create_tunnel( settings::port, settings::region );
                }
                ImGui::SameLine( );
                if ( ImGui::Button( "close tunnel" ) )
                {
                    std::system( "taskkill /f /im ngrok.exe" );
                }
                ImGui::InputText( "IP", &ip );
                if ( ImGui::Button( "get IP" ) )
                {
                    ip = ngrok::get_public_url( );
                }
                ImGui::SameLine( );
                if ( ImGui::Button( "copy IP" ) )
                {
                    HWND hwnd = GetDesktopWindow( );
                    util::to_clipboard( hwnd, ngrok::get_public_url( ) );
                }
                ImGui::Separator( );
                ImGui::Text( "authtoken" );
                ImGui::InputText( "authtoken", &authtoken );
                if ( ImGui::Button( "set ngrok authtoken" ) )
                {
                    std::string commandline = "ngrok authtoken " + authtoken;
                    std::system( commandline.c_str( ) );
                }
                ImGui::Separator( );
                ImGui::Text( "region" );
                if ( ImGui::Combo( "tunnel region", &settings::region, settings::regions, sizeof( settings::regions ) / sizeof( *settings::regions ) ) )
                {
                    Document doc;
                    doc.Parse( util::read_file( "settings.json" ).c_str() );
                    Value& var = doc[ "ngrok_region" ];
                    var.SetInt( settings::region );
                    StringBuffer buffer;
                    Writer<StringBuffer> writer( buffer );
                    doc.Accept( writer );
                    util::write_to_file( "settings.json", buffer.GetString( ) );
                }
                ImGui::Separator( );
                if ( ImGui::CollapsingHeader( "debug stuff" ) )
                {
                    if ( ImGui::Checkbox( "debug mode", &debug_mode ) )
                    {
                        ::ShowWindow( ::GetConsoleWindow( ), debug_mode ? SW_SHOW : SW_HIDE );
                    }
                }

                ImGui::Separator( );

                ImGui::Text( "github.com/gnireorb/ngrok-helper" );
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

struct ImVec3 { float x, y, z; ImVec3( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) { x = _x; y = _y; z = _z; } };

void imgui_easy_theming( ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops )
{
    ImGuiStyle& style = ImGui::GetStyle( );

    style.Colors[ ImGuiCol_Text ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 1.00f );
    style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.58f );
    style.Colors[ ImGuiCol_WindowBg ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.95f );
    style.Colors[ ImGuiCol_ChildBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.58f );
    style.Colors[ ImGuiCol_Border ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.00f );
    style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.00f );
    style.Colors[ ImGuiCol_FrameBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_TitleBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.75f );
    style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.47f );
    style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.21f );
    style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.78f );
    style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 1.f, 1.f, 1.f, 0.80f );
    style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 1.f, 1.f, 1.f, 0.50f );
    style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 1.f, 1.f, 1.f, 1.00f );
    style.Colors[ ImGuiCol_Button ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.50f );
    style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.86f );
    style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_Header ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.76f );
    style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.86f );
    style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.15f );
    style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.78f );
    style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_PlotLines ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.43f );
    style.Colors[ ImGuiCol_PopupBg ] = ImVec4( color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f );
    style.Colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.73f );
}

void SetupImGuiStyle2( )
{
    // 15 24 37
    // 22 35 53
    // 14 22 34
    // 30 47 73
    // 0 57 107
    // 6 31 57
    static ImVec3 color_for_text = ImVec3( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f );
    static ImVec3 color_for_head = ImVec3( 30.f / 255.f, 47.f / 255.f, 73.f / 255.f ); /* parte de cima */
    static ImVec3 color_for_area = ImVec3( 30.f / 255.f, 47.f / 255.f, 73.f / 255.f );
    static ImVec3 color_for_body = ImVec3( 15.f / 255.f, 24.f / 255.f, 37.f / 255.f ); /* body */
    static ImVec3 color_for_pops = ImVec3( 22.f / 255.f, 35.f / 255.f, 53.f / 255.f );
    imgui_easy_theming( color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops );
}
