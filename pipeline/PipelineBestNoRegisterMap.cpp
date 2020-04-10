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
                               "Blur 2.5",
                               "BlendWith http://www.solstation.com/stars/earth3au.jpg"};

  // The images to process.
  // This is a small number of images to allow testing the process function.
  // Again stored in memory as mock images because we don't have IOs.
  vector<Image> imgs = {Image ("one"),
                        Image ("two"),
                        Image ("three")};

  process(operations, imgs);

  return 0;
}

class IImageOp
{
public:
    IImageOp() {}
    virtual ~IImageOp(){}
    virtual void ImageProc(Image & img) = 0;
    virtual void ImageParse(istringstream& in) = 0;    
};

typedef shared_ptr<IImageOp> (*CreateCallback)();

/*
Factory 的精髓
1. abstract class 和 pure virtual function的应用
   每一个derived class 返回的都是base class pointer 但是这里其实有一个implicit的cast
    static shared_ptr<IImageOp> Create() {return make_shared<ConvertToGrayScale>(); }
2. 用map记录好string name 和 create function; 这里并不是记录string name和base class object
   这里的class naming 其实有点tricky 需要和我们的input的完全一样 这样我们runtime interpret的时候
   才能create正确的class
3. 因为我们其实是lazy create：先register map 然后再要用的时候再create
4. 这题还有一个难点是base 的IImage op 和derived class的 IImage的op的argument不同 
   处理方法就是1. 相同的argument 我们就在base class上面改， 不同的argument 我们作为每个derived class
   独有的class member存起来. 我们可以通过一个pure virtual function 也就是ImageParse来进行setup
   这里我们不在constructor里面setup的原因是 我们factory的map是要存一个generic的create factory function 
   所以如果是不同的argument比较难操作 可能要用到variabic template function之类的
*/
class Pipeline
{
public:
    
    // 这里其实是lazy create !!!
    // 先注册register 好create callback function
    // 然后再在真正用到它的时候来一个lazy create!!!!!
    static shared_ptr<IImageOp> GetImgOp(string str)
    {
        auto it = m_opCreate.find(str);
        if (it != m_opCreate.end())
        {
            return it->second();
        }
        
        return nullptr;
    }
    
    using CreateMap = unordered_map<string, CreateCallback>;
    static CreateMap m_opCreate;
    
    // 这里也可以不用RegisterMap 这个其实和RegisterMap 不可能完全dynamic
    // 都是要根据类名建立手动建立map的 Register map写起来太tricky了  
    // static CreateMap Pipeline::m_opCreate = 
    // {
    //       {"ConvertToGrayScale", ConvertToGrayScale::Create},
    //       {"Blur", Blur::Create},
    //       {"Resize", Resize::Create},
    //       {"BlendWith", Blend::Create}
    // };
private:
    // typedef unordered_map<string, shared_ptr<IImageOp>> ImageOp;
    // static ImageOp m_ops;  
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
        in >> str;
        m_x = stoi(str);
        in >> str;
        m_y = stoi(str);
    }
private:
    int m_x;
    int m_y;
};

class Blend : public IImageOp
{
public:
    static shared_ptr<IImageOp> Create() {return make_shared<Blend>(); }
    
    virtual void ImageProc(Image& img) override
    {
        ImgLib::blend(img, *other);
    }
    
    virtual void ImageParse(istringstream& in)
    {
        string str;
        in >> str;
        other = new Image(str);
    }
    
    ~Blend()
    {
        delete other;
    }
private:
    Image* other;
};

// 写在这里因为需要ConvertToGrayScale::Create 这些create functionn
Pipeline::CreateMap Pipeline::m_opCreate = 
{
      {"ConvertToGrayScale", ConvertToGrayScale::Create},
      {"Blur", Blur::Create},
      {"Resize", Resize::Create},
      {"BlendWith", Blend::Create}
};

void process(const vector<string>& operations, vector<Image>& imgs) {
    std::vector<shared_ptr<IImageOp>> pipelineFactory;
    for (auto& op : operations)
    {
        istringstream in(op);
        string str;
        in >> str;
        shared_ptr<IImageOp> imgOp = Pipeline::GetImgOp(str);
        imgOp->ImageParse(in);
        pipelineFactory.push_back(imgOp);
    }
  
    for (auto& i : imgs)
    {
        for (auto& op : pipelineFactory) {
          op->ImageProc(i);
        }
    }
}
