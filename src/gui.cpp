#include <memory>
#include <string>

#include <RmlUi/Core.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/Platform.h>
#include <RmlUi/Core/RenderInterface.h>
#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/Types.h>
#include <SDL2/SDL.h>

// #include "Backends/RmlUi_Backend.h"
#include "Backends/RmlUi_Backend.h"
#include "Backends/RmlUi_Platform_SDL.h"
#include "Backends/RmlUi_Renderer_GL3.h"
#include "gui.h"

#ifdef _WIN32
  // mute conversion from 'float' to 'Type', possible loss of data
  #pragma warning(disable : 4244)
#endif

struct gui_window_t : public Rml::EventListener
{
protected:
    std::string m_name;

    Rml::Context* m_context;
    Rml::ElementDocument* m_document;
    Rml::DataModelHandle m_handle;

    virtual void process_event(Rml::Event& event) = 0;
    virtual void bind_data(void) = 0;

    void load_document(std::string filepath);

public:
    gui_window_t(std::string name, int width, int height);
    ~gui_window_t();

    void update(void);
    void render(void);
    void events(SDL_Event* event);
    void resize(float width, float height);

    void ProcessEvent(Rml::Event& event) override;
};

class EventListener : public Rml::EventListener
{
private:
    Rml::String value;
    Rml::Element* element;

public:
    EventListener(const Rml::String& value, Rml::Element* element)
        : value(value)
        , element(element)
    {
    }

    void ProcessEvent(Rml::Event& event) override
    {
    }

    void OnDetach(Rml::Element*) override
    {
        delete this;
    }
};

class EventListenerInstancer : public Rml::EventListenerInstancer
{
public:
    Rml::EventListener* InstanceEventListener(const Rml::String& value,
                                              Rml::Element* element) override
    {
        return new EventListener(value, element);
    }
};

class GuiSystemInterface : public SystemInterface_SDL
{
};

class GuiRenderInterface : public RenderInterface_GL3
{
public:
    GuiRenderInterface()
    {
    }

    bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
                     const Rml::String& source) override
    {
        return true;
    }
};

struct BackendData
{
    GuiSystemInterface system_interface;
    GuiRenderInterface render_interface;

    SDL_Window* window = nullptr;
    SDL_GLContext glcontext = nullptr;
};

static Rml::UniquePtr<BackendData> data;

// bool Backend::Initialize(const char* window_name, int width, int height, bool allow_resize)
// {
//     RMLUI_ASSERT(!data);

//     if (!RmlGL3::Initialize())
//     {
//         fprintf(stderr, "Could not initialize OpenGL");
//         return false;
//     }

//     data = Rml::MakeUnique<BackendData>();

//     if (!data->render_interface)
//     {
//         data.reset();
//         fprintf(stderr, "Could not initialize OpenGL3 render interface.");
//         return false;
//     }

//     return true;
// }

// void Backend::Shutdown()
// {
//     RMLUI_ASSERT(data);

//     SDL_GL_DeleteContext(data->glcontext);
//     SDL_DestroyWindow(data->window);

//     data.reset();

//     SDL_Quit();
// }

// Rml::SystemInterface* Backend::GetSystemInterface()
// {
//     RMLUI_ASSERT(data);
//     return &data->system_interface;
// }

// Rml::RenderInterface* Backend::GetRenderInterface()
// {
//     RMLUI_ASSERT(data);
//     return &data->render_interface;
// }

// bool Backend::ProcessEvents(Rml::Context* context, KeyDownCallback key_down_callback,
//                             bool power_save)
// {
//     RMLUI_ASSERT(data && context);

//     return true;
// }

// void Backend::RequestExit()
// {
//     RMLUI_ASSERT(data);
// }

// void Backend::BeginFrame()
// {
//     RMLUI_ASSERT(data);

//     data->render_interface.BeginFrame();
// }

// void Backend::PresentFrame()
// {
//     RMLUI_ASSERT(data);

//     data->render_interface.EndFrame();
// }

void gui_system_init(SDL_Window* window, SDL_GLContext glcontext, int width, int height)
{
    Backend::Initialize("", width, height, true);

    data->window = window;
    data->glcontext = glcontext;
    data->system_interface.SetWindow(window);
    data->render_interface.SetViewport(width, height);

    Rml::SetSystemInterface(&data->system_interface);
    Rml::SetRenderInterface(&data->render_interface);

    Rml::Initialise();

    Rml::String filepath = std::string("/fonts/roboto.ttf");
    Rml::LoadFontFace(filepath, true);
}

void gui_system_shutdown(void)
{
    Rml::Shutdown();
    Backend::Shutdown();
}

void gui_system_begin(void)
{
    Backend::BeginFrame();
}

void gui_system_end(void)
{
    Backend::PresentFrame();
}

gui_window_t::gui_window_t(std::string name, int width, int height)
    : m_name(name)
{
    Rml::Vector2i dimensions(width, height);
    Rml::Context* context = Rml::CreateContext(name, dimensions);

    m_context = context;
}

gui_window_t::~gui_window_t()
{
    if (m_document)
        m_document->Close();
    Rml::RemoveContext(m_context->GetName());
}

void gui_window_t::load_document(std::string filepath)
{
    Rml::ElementDocument* document = m_context->LoadDocument(filepath);
    if (document)
        document->Show();

    m_document = document;
}

void gui_window_t::update(void)
{
    if (!m_document)
        return;
    m_context->Update();
}

void gui_window_t::render(void)
{
    if (!m_document)
        return;
    m_context->Render();
}

void gui_window_t::events(SDL_Event* event)
{
    if (!m_document)
        return;

    switch (event->type)
    {
        case SDL_WINDOWEVENT:
        {
            switch (event->window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    Rml::Vector2i dimensions(event->window.data1, event->window.data2);

                    m_context->SetDimensions(dimensions);
                    data->render_interface.SetViewport(dimensions.x, dimensions.y);
                }
                break;
            }
            RmlSDL::InputEventHandler(m_context, *event);
        }
        break;

        case SDL_KEYDOWN:
            if (!RmlSDL::InputEventHandler(m_context, *event))
                break;
            break;

        default:
            RmlSDL::InputEventHandler(m_context, *event);
            break;
    }
}

void gui_window_t::ProcessEvent(Rml::Event& event)
{
    process_event(event);
}

void gui_window_t::resize(float width, float height)
{
    Rml::Vector2i dimensions(width, height);

    m_context->SetDimensions(dimensions);
    data->render_interface.SetViewport(dimensions.x, dimensions.y);
}

void gui_window_events(gui_window_t* gui, SDL_Event* event)
{
    gui->events(event);
}

void gui_window_update(gui_window_t* gui)
{
    gui->update();
}

void gui_window_render(gui_window_t* gui)
{
    gui->render();
}

void gui_window_resize(gui_window_t* gui, float width, float height)
{
    gui->resize(width, height);
}
