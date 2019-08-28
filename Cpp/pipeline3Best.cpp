/*
We are working on a batch image processing pipeline. We want to be able to list
the operations to be applied on all the images (and their parameters) in a
config file. For example:
ConvertToGrayScale
Blur 3.1
Resize 256 256
BlendWith 
The idea is to load the config file, create the pipeline, and process all the
images through the pipeline. We have access to a library for image manipulation
(in the ImgLib namespace below) that has all the functions that we need for
those operations.
How would you design such a pipeline?
Keep in mind that you have 40 minutes, so you are encouraged to first go for
a decently simple solution, and then discuss extensions.
In this case we will mock the behavior of image loading and image processing
with print statements. This is because we don't have IO in coderpad.
Also in this case we have 4 commands and 3 images, but think that in reality
there could be a lot more images and commands.
*/

// 思考 1.singelton ?
// 思考 2.blend
// 思考 3. main 函数的if check 不能移除吧 因为我们要按照类型来create derived class object
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <sstream>
using namespace std;

// Mock image class.
// Do not modify.
class Image {
public:
  Image (const string& name) : name_(name) {}
  const string& name() const { return name_; }
private:
  string name_;
};

int cnt = 0;

// Mock image manipulation library.
// Do not modify.
namespace ImgLib {

void convertToGrayScale (Image & img) {
  cout << img.name() << ": Converting to gray scale" << endl;
}

void blur (Image & img, float factor) {
  cout << img.name() << ": Blurring with factor " << factor << endl;
}

void resize (Image & img, int x, int y) {
  cout << img.name() << ": Resizing img with x = " << x << " and y = " << y << endl;
}

void blend (Image & img, const Image& other) {
  cout << img.name() << ": Blending with " << other.name() << endl;
}

// ******************************************************************
// *                                                                *
// *  NOTE: There are at least 1000 additional functions.           *
// *  The rest have been omitted due to the scope of the exercise.  *
// *                                                                *
// ******************************************************************

} // namespace ImgLib


// // 但是有很多不同的function arg啊！！！
// typedef void (*Func)(void); // function pointer type
// typedef std::unordered_map<std::string, Func> script_map;

class IImageOp
{
public:
    IImageOp() {}
    virtual ~IImageOp() {}
    virtual void ImageProc(Image& img) = 0;
    //virtual void ParseOp(const string& str) = 0;
    virtual void ParseOp(istringstream& in) = 0;
};

class Pipeline
{
public:
    
//     template<typename ...T>
//     static IImageOp* CreateImageOp(std::string const & name, T&&...args)
//     {
//         auto it = m_varMap.find(name);
//         if ( it == m_varMap.end()) 
//             return nullptr;
        
//         typedef IImageOp* (*CreateFunc)(T...);
//         auto create_fun = reinterpret_cast<CreateFunc>(it->second);
//         return create_fun(args...);
//     }
    
    // 这里只能create 一种类型？？？不能有不同的constructor
    //typedef IImageOp* (*CreateCallback)();
    typedef shared_ptr<IImageOp> (*CreateCallback)();
    
    static void RegisterImageOp(const string &type, CreateCallback cb)
    {
        if (m_OpCreateMap.find(type) == m_OpCreateMap.end())
            m_OpCreateMap[type] = cb;
    }
    
    static shared_ptr<IImageOp> CreateImageOp(const std::string &type)
    {
        if (m_sharedOps.find(type) == m_sharedOps.end())
        {
            CallbackMap::iterator it = m_OpCreateMap.find(type);

            if (it != m_OpCreateMap.end())
            {
                // call the creation callback to construct this derived type
                auto ret = (it->second)();
                m_sharedOps[type] = ret;
                return ret;
            }
            
            return nullptr;
        }
        else
        {
            return m_sharedOps[type];
        }
        
    }
    
    static shared_ptr<IImageOp> GetImageOp(const std::string &type, CreateCallback cb)
    {
        if (m_sharedOps.find(type) == m_sharedOps.end())
        {
            cnt++;
            auto ret = cb();
            m_sharedOps[type] = ret;
            return ret;
        }
        else
        {
            //cout << "found" << endl;
            return m_sharedOps[type];
        }
        
    }
    
//     template<typename ...T>
//     static IImageOp* Instantiate(std::string const & name, T&&...args)
//     {
//         auto it = m_varMap.find(name);
//         if ( it == m_varMap.end()) 
//             return 0;
        
//         typedef IImageOp* (*create_type)(T...);
//         auto create_fun = reinterpret_cast<create_type>(it->second);
//         return create_fun(args...);
//     }
    
    ~Pipeline()
    {

    }
    
    typedef unordered_map<std::string, CreateCallback> CallbackMap;
    static CallbackMap m_OpCreateMap;
private:
    
    typedef unordered_map<std::string, shared_ptr<IImageOp>> ImageOpShared;
    static ImageOpShared m_sharedOps;
};

// Init static member!!!!
Pipeline::CallbackMap Pipeline::m_OpCreateMap;
Pipeline::ImageOpShared Pipeline::m_sharedOps;


struct Registrer
{
    template<typename F>
    Registrer(std::string name, F func)
    {
        // 这里forward delcaration 不会work的
        /* The forward declaration is an "incomplete type", the only thing you can do with such a type is instantiate a pointer to it, or reference it in a function declaration (i.e. and argument or return type in a function prototype). In line 52 in your code, you are attempting to instantiate an object.

At that point the compiler has no knowledge of the object's size nor its constructor, so cannot instantiate an object.
*/
        Pipeline::m_OpCreateMap[name] = func;
    }
};

class ConvertToGrayScale : public IImageOp
{
public:
     //static IImageOp* Create() { return new ConvertToGrayScale; }
     static shared_ptr<IImageOp> Create() {return make_shared<ConvertToGrayScale>();}
     virtual void ImageProc(Image & img) override
     {
        cout << img.name() << ": Converting to gray scale" << endl;
     }
     
     // virtual void ParseOp(const string& str)
     // {
     //    istringstream in(str);
     //    string s;
     //    int i = 0;
     //    while (in >> s) {
     //        if (i == 0) {
     //            continue;
     //        }   
     //    }
     // }
     virtual void ParseOp(istringstream& in)
     {
     
     }
private:
    static Registrer reg;
};
Registrer ConvertToGrayScale::reg ("ConvertToGrayScale", ConvertToGrayScale::Create);


class Blur : public IImageOp
{
public:
     Blur( ) { }
     
     Blur(float factor)
        : m_factor(factor)
     { }
    
     //static IImageOp* Create() { return new Blur; }
     static shared_ptr<IImageOp> Create() {return make_shared<Blur>();}
     virtual void ImageProc(Image & img) override
     {
        cout << img.name() << ": Blurring with factor " << m_factor << endl;
     }
     
     
//      virtual void ParseOp(const string& str)
//      {
//         istringstream in(str);
//         string s;
//         int i = 0;
//         while (in >> s)
//         {
//             if (i == 1)
//             {
//                m_factor = stof(s);
//             }

//             i++;
//         }
//      }
     
     virtual void ParseOp(istringstream& in)
     {
        string s;
        in >> s;
        m_factor = stof(s);
     }
     
private:
    float m_factor;
    static Registrer reg;
};

Registrer Blur::reg ("Blur", Blur::Create);

class Resize : public IImageOp
{
public:
     Resize( ) { }
     Resize(int x, int y)
        : m_x(x)
        , m_y(y)
     { }
     //static IImageOp* Create() { return new Resize; }
     static shared_ptr<IImageOp> Create() {return make_shared<Resize>();}
     virtual void ImageProc(Image & img) override
     {
        cout << img.name() << ": Resizing img with x = " << m_x << " and y = " << m_y << endl;
     }
     
     virtual void ParseOp(istringstream& in)
     {
        string s;
        int i = 0;
        while (in >> s) {
            if (i == 0)
            {
                m_x = stoi(s);
            }
            else if (i == 1)
            {
                m_y = stoi(s);
            }
            i++;
        }
     }
     // virtual void ParseOp(const string& str)
     // {
     //    istringstream in(str);
     //    string s;
     //    int i = 0;
     //    while (in >> s) {
     //        if (i == 1)
     //        {
     //            m_x = stoi(s);
     //        }
     //        else if (i == 2)
     //        {
     //            m_y = stoi(s);
     //        }
     //        i++;
     //    }
     // }
private:
    int m_x;
    int m_y;
    static Registrer reg;
};
Registrer Resize::reg ("Resize", Resize::Create);

class BlendWith : public IImageOp
{
public:
      
     BlendWith() { }
     
     BlendWith(Image img)
        :other(&img)
     {

     }
    
     //static IImageOp* Create() { return new Blend; }
     static shared_ptr<IImageOp> Create() {return make_shared<BlendWith>();}
     virtual void ImageProc(Image & img) override
     {
        cout << img.name() << ": Blending with " << other->name() << endl;
     }
     
     virtual void ParseOp(istringstream& in)
     {
        string s;
        //while (in >> s)
        {
            other = new Image("ImageUnknow");
        }
     }
     // virtual void ParseOp(const string& str) override
     // {
     //    istringstream in(str);
     //    string s;
     //    // How to resolve this ????
     //    // Assume that the Image class constructor will take care of downloading
     //    // and loading the blend image used in the BlendWith operation.
     //    while (in >> s)
     //    {
     //        other = new Image("ImageUnknow");
     //    }
     // }
private:
   Image* other;
   static Registrer reg;
};
Registrer BlendWith::reg ("BlendWith", BlendWith::Create);


// Forward declaration of the process function that you have to write (below).
void process(const vector<string>& cmds, vector<Image>& imgs);

// Simple main to allow testing the process function.
int main() {
  // The "config file".
  // These commands are guaranteed to be error-free and are assumed to
  // be space delimited, with the first word being a command idenifier.
  // The arguments to each command are always of the same types.
  // Because coderpad does not have IO, we store an example config file
  // in a string format directly. This is a simple config file to let us
  // test the process function with a small number of operations.
  // Assume that the Image class constructor will take care of downloading
  // and loading the blend image used in the BlendWith operation.
  vector<string> operations = {"ConvertToGrayScale",
                               "Blur 3.1",
                               "Resize 256 256",
                               "BlendWith ",
                               "ConvertToGrayScale",
                               "Blur 6.1",
                               "Resize 128 256",
                               "BlendWith "};

  // The images to process.
  // This is a small number of images to allow testing the process function.
  // Again stored in memory as mock images because we don't have IOs.
  vector<Image> imgs = {Image ("one"),
                        Image ("two"),
                        Image ("three")};

  process(operations, imgs);

  return 0;
}


// Your work goes here
void process(const vector<string>& operations, vector<Image>& imgs) 
{
    
    shared_ptr<IImageOp> imageOp;
    for (auto& i : imgs) 
    {
        for (auto& op : operations)
        {
            //这里不解析怎么知道是谁的create?? static register 提前register好
            istringstream in(op);
            string str;
            //getline(in, str, ' ');
            in >> str;
            imageOp = Pipeline::CreateImageOp(str);
            //imageOp->ParseOp(op);
            imageOp->ParseOp(in);
            imageOp->ImageProc(i);
        }
    }
}