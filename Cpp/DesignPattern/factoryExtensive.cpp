#include <iostream>
#include <string>
#include <map>

using std::cout;
using std::endl;

///
/// An abstract interface for plugins to create new renderers.
///
class IRenderer
{
public:
    virtual ~IRenderer() {}
    virtual bool LoadScene(const std::string &filename) = 0;
    virtual void SetViewportSize(int w, int h) = 0;
    virtual void SetCameraPos(double x, double y, double z) = 0;
    virtual void SetLookAt(double x, double y, double z) = 0;
    virtual void Render() = 0;
};

///
/// A factory object that creates instances of different
/// 3D renderers. New renderers can be dynamically added
/// and removed from the factory object.
///
class RendererFactory
{
public:
    /// The type for the callback that creates an IRenderer instance
    typedef IRenderer *(*CreateCallback)();

    /// Add a new 3D renderer to the system
    static void RegisterRenderer(const std::string &type,
        CreateCallback cb);
    /// Remove an existing 3D renderer from the system
    static void UnregisterRenderer(const std::string &type);

    /// Create an instance of a named 3D renderer
    static IRenderer *CreateRenderer(const std::string &type);

private:
    typedef std::map<std::string, CreateCallback> CallbackMap;
    static CallbackMap mRenderers;
    // Need to declare as static because it will be used in static member function
    //CallbackMap mRenderers;
};


// static member need to be instantiated!!!
// instantiate the static variable in RendererFactory
RendererFactory::CallbackMap RendererFactory::mRenderers;

void RendererFactory::RegisterRenderer(const std::string &type,
                                       CreateCallback cb)
{
    mRenderers[type] = cb;
}

void RendererFactory::UnregisterRenderer(const std::string &type)
{
    mRenderers.erase(type);
}

IRenderer *RendererFactory::CreateRenderer(const std::string &type)
{
    CallbackMap::iterator it = mRenderers.find(type);
    if (it != mRenderers.end())
    {
        // call the creation callback to construct this derived type
        return (it->second)();
    }

    return NULL;
}

/// An OpenGL-based 3D renderer
class OpenGLRenderer : public IRenderer
{
public:
    ~OpenGLRenderer() {}
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "OpenGL Render" << endl; }
    static IRenderer *Create() { return new OpenGLRenderer; }
};

/// A DirectX-based 3D renderer
class DirectXRenderer : public IRenderer
{
public:
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "DirectX Render" << endl; }
    static IRenderer *Create() { return new DirectXRenderer; }
};

/// A Mesa-based software 3D renderer
class MesaRenderer : public IRenderer
{
public:
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "Mesa Render" << endl; }
    static IRenderer *Create() { return new MesaRenderer; }
};


int main(int, char **)
{
    // register the various 3D renderers with the factory object
    RendererFactory::RegisterRenderer("opengl", OpenGLRenderer::Create);
    RendererFactory::RegisterRenderer("directx", DirectXRenderer::Create);
    RendererFactory::RegisterRenderer("mesa", MesaRenderer::Create);

    // create an OpenGL renderer
    IRenderer *ogl = RendererFactory::CreateRenderer("opengl");
    ogl->Render();
    delete ogl;

    // create a Mesa software renderer
    IRenderer *mesa = RendererFactory::CreateRenderer("mesa");
    mesa->Render();
    delete mesa;

    // unregister the Mesa renderer
    RendererFactory::UnregisterRenderer("mesa");
    mesa = RendererFactory::CreateRenderer("mesa");
    if (!mesa)
    {
        cout << "Mesa renderer unregistered" << endl;
    }

    return 0;
}
