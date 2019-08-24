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
    virtual void ParseOp(const string& str) = 0;
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
     
     virtual void ParseOp(const string& str)
     {
        istringstream in(str);
        string s;
        int i = 0;
        while (in >> s) {
            if (i == 0) {
                continue;
            }   
        }
     }
};

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
     
     
     virtual void ParseOp(const string& str)
     {
        istringstream in(str);
        string s;
        int i = 0;
        while (in >> s)
        {
            if (i == 1)
            {
               m_factor = stof(s);
            }

            i++;
        }
     }
     
private:
    float m_factor;
};

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
     
     
     virtual void ParseOp(const string& str)
     {
        istringstream in(str);
        string s;
        int i = 0;
        while (in >> s) {
            if (i == 1)
            {
                m_x = stoi(s);
            }
            else if (i == 2)
            {
                m_y = stoi(s);
            }
            i++;
        }
     }
private:
    int m_x;
    int m_y;
};

class Blend : public IImageOp
{
public:
      
     Blend() { }
     
     Blend(Image img)
        :other(&img)
     {

     }
    
     //static IImageOp* Create() { return new Blend; }
     static shared_ptr<IImageOp> Create() {return make_shared<Blend>();}
     virtual void ImageProc(Image & img) override
     {
        cout << img.name() << ": Blending with " << other->name() << endl;
     }
     
     virtual void ParseOp(const string& str) override
     {
        istringstream in(str);
        string s;
        // How to resolve this ????
        // Assume that the Image class constructor will take care of downloading
        // and loading the blend image used in the BlendWith operation.
        while (in >> s)
        {
            other = new Image("ImageUnknow");
        }
     }
private:
   Image* other;
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
    
    
    ~Pipeline()
    {

    }
    
private:
    
    typedef map<std::string, CreateCallback> CallbackMap;
    static CallbackMap m_OpCreateMap;
    
    typedef map<std::string, shared_ptr<IImageOp>> ImageOpShared;
    static ImageOpShared m_sharedOps;
    
    
    typedef map<std::string, void*> VaridicMap;
    static VaridicMap m_varMap;
};

// Init static member!!!!
Pipeline::CallbackMap Pipeline::m_OpCreateMap;
Pipeline::ImageOpShared Pipeline::m_sharedOps;

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
    //Pipeline p;
    // 这里得free啊 还是用smart pointer比较好

//     for (auto& op : operations)
//     {
//         istringstream in(op);
//         string str;
//         getline(in, str, ' ');
//         // 这里不解析怎么知道是谁的create??        
//         // 有了这个register函数的话 就不用在pipeline class里面解析命令了！！！ 
//         if (str == "ConvertToGrayScale")
//             Pipeline::RegisterImageOp(str, ConvertToGrayScale::Create);
//         else if (str == "Blur")
//             Pipeline::RegisterImageOp(str, Blur::Create);
//         else if (str == "Resize")
//             Pipeline::RegisterImageOp(str, Resize::Create);
//         else if (str == "BlendWith") {
//             Pipeline::RegisterImageOp(str, Blend::Create);
//         }
//     }
    
    shared_ptr<IImageOp> imageOp;
    for (auto& i : imgs) 
    {
        for (auto& op : operations)
        {
            istringstream in(op);
            string str;
            getline(in, str, ' ');
            // 这里不解析怎么知道是谁的create??
            if (str == "ConvertToGrayScale") {                
                //shared_ptr<IImageOp> imageOp = Pipeline::CreateImageOp("ConvertToGrayScale");
                //imageOp = Pipeline::CreateImageOp("ConvertToGrayScale");                
                imageOp = Pipeline::GetImageOp(str, ConvertToGrayScale::Create);
                //imageOp = p.GetImagOp()
                imageOp->ParseOp(op);
                imageOp->ImageProc(i);
            }
            else if (str == "Blur") {
                //IImageOp* imageOp = Pipeline::CreateImageOp("Blur");
                //shared_ptr<IImageOp> imageOp = Pipeline::CreateImageOp("Blur");
                //imageOp = Pipeline::CreateImageOp("Blur");
                imageOp = Pipeline::GetImageOp(str, Blur::Create);
                imageOp->ParseOp(op);
                imageOp->ImageProc(i);
            }
            else if (str == "Resize") {
                //IImageOp* imageOp = Pipeline::CreateImageOp("Resize");
                // imageOp = Pipeline::CreateImageOp("Resize");
                imageOp = Pipeline::GetImageOp(str, Resize::Create);
                imageOp->ParseOp(op);
                imageOp->ImageProc(i);
            }
            else if (str == "BlendWith") {
                //IImageOp* imageOp = Pipeline::CreateImageOp("BlendWith");
                //shared_ptr<IImageOp> imageOp = Pipeline::CreateImageOp("BlendWith");
                //imageOp = Pipeline::CreateImageOp("BlendWith");
                imageOp = Pipeline::GetImageOp(str, Blend::Create);
                imageOp->ParseOp(op);
                imageOp->ImageProc(i);
            }
        }
    }
    
    cout << imageOp.use_count() << endl;
//     // 这个不好 这样你只改变一下参数就要新create一个instance吗？？？？！！！
//     p.AddImageOpSmart("ConvertToGrayScale", make_unique<ConvertToGrayScale>());
//     p.AddImageOpSmart("Blur", make_unique<Blur>(3.1));
//     p.AddImageOpSmart("Resize", make_unique<Resize>(256, 256));
//     p.AddImageOpSmart("Blend", make_unique<Blend>(imgs[2]));
    
//     p.AddImageOpSmart("ConvertToGrayScale", move(make_unique<ConvertToGrayScale>()));
//     p.AddImageOpSmart("Blur", move(make_unique<Blur>(3.1)));
//     p.AddImageOpSmart("Resize", move(make_unique<Resize>(256, 256)));
//     p.AddImageOpSmart("Blend", move(make_unique<Blend>(imgs[2])));
      
//     p.ImageOpSmart("ConvertToGrayScale", imgs[0]);
//     p.ImageOpSmart("Blur", imgs[1]);
//     p.ImageOpSmart("Resize", imgs[0]);
//     p.ImageOpSmart("Blend", imgs[1]);
}