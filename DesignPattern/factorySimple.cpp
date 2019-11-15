#include <iostream>
#include <string>
using std::cout;
using std::endl;
/*
Use Factory Methods to provide more powerful class construction semantics and to hide subclass details.
However, one potential drawback is that it contains hardcoded knowledge of the available derived classes
*/

///
/// An abstract interface for a 3D renderer.
///
/*
An abstract base class is therefore useful to describe abstract units of behaviors that can be shared by multiple classes;it specifies a contract that all concrete derived classes must conform to
*/
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

class OpenGLRenderer : public IRenderer
{
public:
    ~OpenGLRenderer() {}
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "OpenGL Render" << endl; }
};

class DirectXRenderer : public IRenderer
{
public:
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "DirectX Render" << endl; }
};

class MesaRenderer : public IRenderer
{
public:
    bool LoadScene(const std::string &filename) { return true; }
    void SetViewportSize(int w, int h) {}
    void SetCameraPos(double x, double y, double z) {}
    void SetLookAt(double x, double y, double z) {}
    void Render() { cout << "Mesa Render" << endl; }
};

///
/// A factory object that creates instances of different
/// 3D renderers.
///
class RendererFactory
{
public:
    /// Create a new instance of a named 3D renderer.
    /// type can be one of "opengl", "directx", or "mesa"
    /*
    Note that this method cannot return an instance of the specific type         IRenderer because that’s an abstract base class and cannot be           
    instantiated. However, it can return instances of derived classes.
    */
    IRenderer* CreateRenderer(const std::string &type)
    {
        if (type == "opengl")
        return new OpenGLRenderer;

        if (type == "directx")
            return new DirectXRenderer;

        if (type == "mesa")
            return new MesaRenderer;

        return NULL;
    }
};


int main(int, char **)
{
    // create the factory object
    RendererFactory *f = new RendererFactory;

    // create an OpenGL renderer
    IRenderer *ogl = f->CreateRenderer("opengl");
    ogl->Render();
    delete ogl;

    // create a Mesa software renderer
    IRenderer *mesa = f->CreateRenderer("mesa");
    mesa->Render();
    delete mesa;

    delete f;
    return 0;
}
