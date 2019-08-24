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

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
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
    virtual ~IImageOp() {}
    virtual void ImageProc(Image& img) = 0;
    
};

class ConvertToGrayScale : public IImageOp
{
public:     
     virtual void ImageProc(Image & img) 
     {
        cout << img.name() << ": Converting to gray scale" << endl;
     }
};

class Blur : public IImageOp
{
public:
     // virtual ImageOperation(Image& img)
     // {
     //    cout << img.name() << ": Converting to gray scale" << endl;
     // }
     Blur(float factor)
        : m_factor(factor)
     {

     }

     virtual void ImageProc(Image & img) 
     {
        cout << img.name() << ": Blurring with factor " << m_factor << endl;
     }
     // virtual void ImageOperation(Image & img, float factor) 
     // {
     //    cout << img.name() << ": Blurring with factor " << factor << endl;
     // }
private:
    float m_factor;
};


class Resize : public IImageOp
{
public:
     // virtual ImageOperation(Image& img)
     // {
     //    cout << img.name() << ": Converting to gray scale" << endl;
     // }
     Resize(int x, int y)
        : m_x(x)
        , m_y(y)
     {

     }

     virtual void ImageProc(Image & img) 
     {
        cout << img.name() << ": Resizing img with x = " << m_x << " and y = " << m_y << endl;
     }
     // virtual void ImageOperation(Image & img, float factor) 
     // {
     //    cout << img.name() << ": Blurring with factor " << factor << endl;
     // }
private:
    int m_x;
    int m_y;
};


class Blend : public IImageOp
{
public:
     // virtual ImageOperation(Image& img)
     // {
     //    cout << img.name() << ": Converting to gray scale" << endl;
     // }
     Blend(Image img)
        :other(img)
     {

     }

     virtual void ImageProc(Image & img) 
     {
        cout << img.name() << ": Blending with " << other.name() << endl;
     }
     // virtual void ImageOperation(Image & img, float factor) 
     // {
     //    cout << img.name() << ": Blurring with factor " << factor << endl;
     // }
private:
   Image other;
};

class Pipeline
{
public:
    typedef IImageOp* (*CreateCallback)();
//     void RemoveImageOp(const std::string& type,
//                       CreateCallback cb)
//     {
//         m_ops.erase(type);
//     }
    
    void AddImageOp(const std::string& type,
                    IImageOp* imgOp)
    {
        //m_ops[type] = imgOp;
        
        m_ops.insert({type, imgOp});
    }
    
    void ImageOp(const std::string& type,
                 Image& img)
    {
        m_ops[type]->ImageProc(img);
    }
          
    
    // void AddImageOpSmart(const std::string& type,
    //                      unique_ptr<IImageOp> imgOp)
    // {
    //     m_smartOps.insert({type, move(imgOp)});
    // }
    
    // Perfect forward???
    void AddImageOpSmart(const std::string& type,
                         unique_ptr<IImageOp>&& imgOp)
    {
        m_smartOps.insert({type, move(imgOp)});
    }
    
                                 
    void ImageOpSmart(const std::string& type,
                      Image& img)
    {
        m_smartOps[type]->ImageProc(img);
    }
    
    // IImageOp* CreateImageOp(const std::string& type)
    // {
    //     ImageOpMap::iterator it = m_ops.find(type);
    //     if (it != m_ops.end())
    //     {
    //         //m_ops[type] = (it->second)();
    //         return (it->second)();
    //     }
    //     return nullptr;
    // }
private:
    //typedef std::map<std::string, IImagOp*> ImageOpMap;
    typedef unordered_map<std::string, IImageOp*> ImageOpMap;
    //typedef std::map<std::string, CreateCallback> ImageOpMap;
    ImageOpMap m_ops;
    
    typedef unordered_map<string, unique_ptr<IImageOp>> ImageOpMapSmart;
    ImageOpMapSmart m_smartOps;
};

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
    Pipeline p;
    // 这里得free啊 还是用smart pointer比较好
   
    
    // p.AddImageOp("ConvertToGrayScale", ConvertToGrayScale::Create);
    // p.AddImageOp("Blur", Blur::Create(3.1));
     //IImageOp* c = p.CreateImageOp("ConvertToGrayScale");
    // p.AddImageOp("ConvertToGrayScale", new ConvertToGrayScale());
    // p.AddImageOp("Blur", new Blur(3.1));
    // p.ImageOp("ConvertToGrayScale", imgs[0]);
    // p.ImageOp("Blur", imgs[1]);
    
    
    
    
    
    // p.AddImageOpSmart("ConvertToGrayScale", make_unique<ConvertToGrayScale>());
    // p.AddImageOpSmart("Blur", make_unique<Blur>(3.1));
    // p.AddImageOpSmart("Resize", make_unique<Resize>(256, 256));
    // p.AddImageOpSmart("Blend", make_unique<Blend>(imgs[2]));
    
    p.AddImageOpSmart("ConvertToGrayScale", move(make_unique<ConvertToGrayScale>()));
    p.AddImageOpSmart("Blur", move(make_unique<Blur>(3.1)));
    p.AddImageOpSmart("Resize", move(make_unique<Resize>(256, 256)));
    p.AddImageOpSmart("Blend", move(make_unique<Blend>(imgs[2])));
      
    p.ImageOpSmart("ConvertToGrayScale", imgs[0]);
    p.ImageOpSmart("Blur", imgs[1]);
    p.ImageOpSmart("Resize", imgs[0]);
    p.ImageOpSmart("Blend", imgs[1]);
}
