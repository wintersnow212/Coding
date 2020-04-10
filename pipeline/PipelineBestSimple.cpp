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

class IImageOp
{
public:
    // 因为这个Image& image 是所有继承类都共享的
    // 所以我们可以把它centralized到pure virtual base里面去
    virtual void imageProc(Image& image) = 0;
    virtual void imageParse(istringstream& in) = 0;
    // 这里base 要是virtual destructor 这样保证了我们derived class会被call！！！！
    virtual ~IImageOp() {}
};


// 这个是function pointer的typedef callback function 经常用！！！！
typedef IImageOp* (*CreateFn)();


class Pipeline
{
public:
    //void install
    static IImageOp* GetImgOp(string str)
    {
        auto it = m_strToCreateFn.find(str);
        if (it != m_strToCreateFn.end())
        {
            return it->second();
        }
    
        return nullptr;
    }
    using CreateMap = unordered_map<string, CreateFn>;
    static CreateMap m_strToCreateFn;
};


class ConvertToGrayScale : public IImageOp
{
public:
    static IImageOp* create()
    {
        return new ConvertToGrayScale();
    }
    
    virtual void imageProc(Image& image)
    {
        ImgLib::convertToGrayScale(image);
    }
    
    virtual void imageParse(istringstream& in)
    {
    
    }
};

class Blur : public IImageOp
{
public:
    static IImageOp* create()
    {
        return new Blur();
    }

    virtual void imageProc(Image& image)
    {
        ImgLib::blur(image, factor);
    }
    
    virtual void imageParse(istringstream& in)
    {
        string str;
        in >> str;
        factor = stof(str);
    }
private:
    float factor;
};

class Resize : public IImageOp
{
public:
    static IImageOp* create()
    {
        return new Resize();
    }
    virtual void imageProc(Image& image)
    {
        ImgLib::resize(image, x, y);
    }
    
    virtual void imageParse(istringstream& in)
    {
        string str;
        in >> str;
        x = stoi(str);
        in >> str;
        y = stoi(str);
    }
private:
    int x;
    int y;
};

class BlendWith : public IImageOp
{
public:
    static IImageOp* create()
    {
        return new BlendWith();
    }
    
    virtual void imageProc(Image& image)
    {
        ImgLib::blend(image, *other);
    }
    
    virtual void imageParse(istringstream& in)
    {
        string str;
        in >> str;
        other =  new Image(str);
    }
    
    ~BlendWith()
    {
        delete other;
    }
private:
    Image* other;
};

Pipeline::CreateMap Pipeline:: m_strToCreateFn = 
{
    {"ConvertToGrayScale", ConvertToGrayScale::create},
    {"Blur",   Blur::create},
    {"Resize", Resize::create},
    {"BlendWith", BlendWith::create},
};

void process(const vector<string>& operations, vector<Image>& imgs) {
    std::vector<IImageOp*> pipelineFactory;
    for (auto& op : operations)
    {
        istringstream in(op);
        string str;
        in >> str;
        IImageOp* imgOp = Pipeline::GetImgOp(str);
        imgOp->imageParse(in);
        pipelineFactory.push_back(imgOp);
    }
  
    for (auto& i : imgs)
    {
        for (auto& op : pipelineFactory) {
          op->imageProc(i);
        }
    }
    
    // 这个不能忘了 不然就memory leak了 这个也是smart ptr的好处
    for (auto p : pipelineFactory)
    {
        delete p;
    }
}
