/*
We are working on a batch image processing pipeline. We want to be able to list
the operations to be applied on all the images (and their parameters) in a
config file. For example:

ConvertToGrayScale
Blur 3.1
Resize 256 256
BlendWith http://www.solstation.com/stars/earth3au.jpg

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
                               "Blur 2.5"};
                               //"BlendWith http://www.solstation.com/stars/earth3au.jpg"};

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
class IImageOp
{
public:
    IImageOp() {}
    virtual ~IImageOp(){}
    virtual void ImageProc(Image & img) = 0;
    virtual void ImageParse(istringstream& in) = 0;
    
};

class ConvertToGrayScale : public IImageOp
{
public:
    
    static shared_ptr<IImageOp> Create() {return make_shared<ConvertToGrayScale>(); }
    virtual void ImageProc(Image & img) override
    {
        //cout << img.name() << ": Converting to gray scale" << endl;
        ImgLib::convertToGrayScale(img);
    }
    
    virtual void ImageParse(istringstream& in)
    {
        
    }
};

class Blur : public IImageOp
{
public:
    
    static shared_ptr<IImageOp> Create() { return make_shared<Blur>(); }

    virtual void ImageProc(Image & img) override
    {
        ImgLib::blur(img, m_factor);
    }
    
    virtual void ImageParse(istringstream& in)
    {
        string str;
        in >> str;
        m_factor = stof(str);
    }
private:
    float m_factor;
};

class Resize : public IImageOp
{
public:

    static shared_ptr<IImageOp> Create() {return make_shared<Resize>(); }
    
    virtual void ImageProc(Image& img) override
    {
        ImgLib::resize(img, m_x, m_y);
    }
    
    
    virtual void ImageParse(istringstream& in)
    {
        string str;
        int idx = 0;
        in >> str;
        m_x = stoi(str);
        in >> str;
        m_y = stoi(str);
    }
private:
    int m_x;
    int m_y;
};


//class Blend : public IImageOp
//{
//public:
//    static IImageOp* Create() {return new Blend(); }
    
//    virtual ImageProc(Image & img) override
//    {
//        ImgLib::blend(img, other);
//    }
//};

class Pipeline
{
public:
    typedef shared_ptr<IImageOp> (*CreateCallback)();
    
    
    // static shared_ptr<IImageOp> Register(const string& type, CreateCallback cb)
    // {
    //     if (m_opCreate.find(type) == m_opCreate.end())
    //     {
    //         m_opCreate[type] = cb;
    //     }
    // }
    
   static shared_ptr<IImageOp> GetImgOp(string str)
    {
        auto it = m_opCreate.find(str);
        if (it != m_opCreate.end())
        {
            return it->second();
        }
        
        return nullptr;
    }
    
private:
    // Blur
    // typedef unordered_map<string, shared_ptr<IImageOp>> ImageOp;
    // static ImageOp m_ops;
      
    typedef unordered_map<string, CreateCallback> CreateMap;
    static CreateMap m_opCreate;
};

Pipeline:: CreateMap Pipeline::m_opCreate = {
      {"ConvertToGrayScale", ConvertToGrayScale::Create},
      {"Blur", Blur::Create},
      {"Resize", Resize::Create}};

//Pipeline::CreateMap Pipeline::m_opCreate;

void process(const vector<string>& operations, vector<Image>& imgs) {
    std::vector<shared_ptr<IImageOp>> pipeline;
    for (auto& op : operations)
    {
        istringstream in(op);
        string str;
        in >> str;
        shared_ptr<IImageOp> imgOp = Pipeline::GetImgOp(str);
        imgOp->ImageParse(in);
        pipeline.push_back(imgOp);
    }
  
    for (auto& i : imgs)
    {
        for (auto& op : pipeline) {
          op->ImageProc(i);
        }
    }
}
