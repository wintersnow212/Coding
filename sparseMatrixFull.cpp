# include<iostream>
# include<assert.h>
using namespace std;
struct Trituple{        //自定义数据结构：矩阵元素的行，列，值；
	int row,col;
	int value;
	Trituple& operator=(Trituple& x){  //赋值运算符重载
		row=x.row;
		col=x.col;
		value=x.value;
		return *this;
	}
};


using namespace std;
const int DefaultSize=100;
class SparseMatrix{    //稀疏矩阵
private:
	int Rows,Cols,Terms;   //行数，列数，非零元素的个数
	Trituple *smArray;   //存非零元素的三元数组
	int maxTerms;    //三元组最大可容纳的元素个数
public:
	SparseMatrix(int maxSz=DefaultSize);  //构造函数
	SparseMatrix(SparseMatrix& SM);      //赋值构造函数
	~SparseMatrix(); //析构函数
	SparseMatrix& operator=(SparseMatrix& SM); //赋值运算符重载
	SparseMatrix Transpose();     //矩阵转置
	SparseMatrix Add(SparseMatrix& b);      //矩阵的加法
	SparseMatrix Multiply(SparseMatrix& b);      //矩阵的乘法
	friend ostream& operator<<(ostream& ostr,SparseMatrix& SM); //矩阵的输出重载函数
	friend istream& operator>>(istream& istr,SparseMatrix& SM); //矩阵的输入重载函数
};
SparseMatrix::SparseMatrix(int maxSz):maxTerms(maxSz){     //构造函数：构造一个大小为maxTerm的三元组，行列数和非零元素个数都置零
	if(maxSz<1){
		cerr<<"矩阵初始化错误！"<<endl;
		exit(1);
	}
	smArray=new Trituple[maxSz];
	assert(smArray!=NULL);
	Rows=Cols=Terms=0;
}
SparseMatrix::SparseMatrix(SparseMatrix& SM){  //复制构造函数
	Rows=SM.Rows;      //赋值矩阵的性质
	Cols=SM.Cols;
	Terms=SM.Terms;
	maxTerms=SM.maxTerms;
	smArray=new Trituple[maxTerms];  //构造三元组并赋与SM相同的值
	assert(smArray!=NULL);
	for(int i=0;i<Terms;i++)
		smArray[i]=SM.smArray[i];
}
SparseMatrix::~SparseMatrix(){   //析构函数：释放所有存储
	delete[]smArray;
}
SparseMatrix& SparseMatrix::operator=(SparseMatrix& SM){ //赋值运算符重载
	Rows=SM.Rows;      //元素性质的赋值
	Cols=SM.Cols;
	Terms=SM.Terms;
	maxTerms=SM.maxTerms;
	for(int i=0;i<Terms;i++)      //三元组所有元素赋值
		smArray[i]=SM.smArray[i];
	return *this;    //返回的是对调用该函数的对象的引用，需显式使用this指针；
}
ostream& operator<<(ostream& ostr,SparseMatrix& SM){  //输出运算符重载 （为啥代模板就不能调用row？ ）
	ostr<<"# Rows="<<SM.Rows<<endl;    //输出该矩阵的性质
	ostr<<"# Cols="<<SM.Cols<<endl;
	ostr<<"# Terms="<<SM.Terms<<endl;
	for(int i=0;i<SM.Terms;i++)   //输出该矩阵非零元素的位置及值
		ostr<<i+1<<": "<<"SM<"<<SM.smArray[i].row<<","<<SM.smArray[i].col<<">="<<
		SM.smArray[i].value<<endl;
	return ostr;
}
istream& operator>> (istream& istr,SparseMatrix& SM){  //输入运算符重载
	cout<<"Please enter number of rows,columns,and terms of Matrix"<<endl;
	istr>>SM.Rows>>SM.Cols>>SM.Terms;  //输入元素的性质
	if(SM.Terms>SM.maxTerms){
		cerr<<"Numbers of Terms overflow!"<<endl;
		exit(1);
	}
	for(int i=0;i<SM.Terms;i++){   //依次输入非零元素的坐标和值
		cout<<"Enter row,column,and value of term:"<<i+1<<endl;
		cin>>SM.smArray[i].row>>SM.smArray[i].col>>SM.smArray[i].value;
	}
	return istr;
}
/*SparseMatrix SparseMatrix::Transpose(){ //转置函数
	SparseMatrix b(maxTerms);
	b.Rows=Rows;
	b.Cols=Cols;
	b.Terms=Terms;
	b.maxTerms=maxTerms;
	if(Terms>0){
		int i,k,CurrentB=0;
		for(k=0;k<b.Cols;k++)     
			for(i=0;i<Terms;i++)
				if(smArray[i].col==k){
					b.smArray[CurrentB].row=smArray[i].col;
					b.smArray[CurrentB].col=smArray[i].row;
					b.smArray[CurrentB].value=smArray[i].value;
					CurrentB++;
				}
	}
	return b;
}*/
SparseMatrix SparseMatrix::Transpose(){   //转置函数
	int *rowSize=new int[Cols]; //转置矩阵每行非零元素的个数
	int *rowStart=new int[Cols]; //转置矩阵每行第一个非零元素对应其三元组的下标
	SparseMatrix b(maxTerms); //转置后的矩阵对应的三元组
	b.Rows=Rows;       //b的性质
	b.Cols=Cols;
	b.Terms=Terms;
	b.maxTerms=maxTerms;
	if(Terms>0){  
		int i,j,CurrentB=0;
		for(i=0;i<Cols;i++)     //对rowSize数组赋值
			rowSize[i]=0;
		for(i=0;i<Terms;i++)
			rowSize[smArray[i].col]++;
		rowStart[0]=0;        //对rowStart数组赋值
		for(i=1;i<b.Rows;i++)
			rowStart[i]=rowStart[i-1]+rowSize[i-1];
		for(i=0;i<Terms;i++){ //遍历三元组a，把各个元素按rowStart数组存在b中相应的位置
			j=rowStart[smArray[i].col]; //a数组中行号按从小到大的顺序排列，所以相同列最先遇到的元素肯定处在相应转置矩阵相应行中的最前面
			b.smArray[j].row=smArray[i].col; //把该元素按照找到的下标j存入b中
			b.smArray[j].col=smArray[i].row;
			b.smArray[j].value=smArray[i].value;
			rowStart[smArray[i].col]++;  //因为该值已经存入b，所以转置矩阵的该行下一个元素在b中对应的下标为rowStart[smArray[i].col]++；
		}
	}
	delete[] rowSize; //释放new申请的存储空间
	delete[] rowStart;
	return b;
}
SparseMatrix SparseMatrix::Add(SparseMatrix& b){   //转置矩阵的加法
	SparseMatrix Result(Rows*Cols);     //结果存于Result里面
	if(Rows!=b.Rows||Cols!=b.Cols){    //规格相同的矩阵才能相加
		cout<<"Incompatible matrices"<<endl;
		return Result;
	}
	Result.Rows=Rows;  
	Result.Cols=Cols;
	Result.Terms=0;
	Result.maxTerms=Rows*Cols;
	int i=0,j=0,index_a,index_b; //i：遍历a三元组；index_a:当前所指的a中元素在矩阵中的位置；
	while(i<Terms&&j<b.Terms){
	index_a=smArray[i].row*Cols+smArray[i].col;
	index_b=b.smArray[j].row*b.Cols+b.smArray[i].col;
	if(index_a<index_b){      //当前所指的a，b中两个元素，a中元素位置在前
		Result.smArray[Result.Terms].row=smArray[i].row;   //直接把a的元素放在Result里面
		Result.smArray[Result.Terms].col=smArray[i].col;
		Result.smArray[Result.Terms].value=smArray[i].value;
		i++; //i指针指向a中下一个元素
	}
	if(index_a>index_b){
		Result.smArray[Result.Terms].row=b.smArray[j].row;
		Result.smArray[Result.Terms].col=b.smArray[j].col;
		Result.smArray[Result.Terms].value=b.smArray[j].value;
		j++;
	}
	if(index_a==index_b){  //位置相同
		if(smArray[i].value+b.smArray[j].value){   //如果两个值相加的和不为零
					Result.smArray[Result.Terms].row=smArray[j].row;   //把相加的结果放在Result中
		            Result.smArray[Result.Terms].col=smArray[j].col;
		            Result.smArray[Result.Terms].value=smArray[i].value+b.smArray[j].value;
					i++;
					j++;
		}
	}
	Result.Terms++;  //存一个元素，非零元素的个数+1；
	}
	for(;i<Terms;i++){       //b中元素已经遍历完，把a剩余的元素放入Result里面，此时i所指的第一个元素位置肯定在b中最后一个元素后面
		Result.smArray[Result.Terms].row=smArray[i].row;
		Result.smArray[Result.Terms].col=smArray[i].col;
		Result.smArray[Result.Terms].value=smArray[i].value;
		i++;
		Result.Terms++;
	}
	for(;j<b.Terms;j++){
		Result.smArray[Result.Terms].row=b.smArray[j].row;
		Result.smArray[Result.Terms].col=b.smArray[j].col;
		Result.smArray[Result.Terms].value=b.smArray[j].value;
		j++;
	    Result.Terms++;
	}
	return Result;
}
SparseMatrix SparseMatrix::Multiply(SparseMatrix& b){  //矩阵的乘法
	SparseMatrix Result(Rows*b.Cols);      //存放矩阵相乘的结果
	if(Cols!=b.Rows){         //两个矩阵能相乘的先决条件：第一个的列数等于第二个的行数
		cerr<<"Incompatible matrices"<<endl;
		return Result;
	}
	int *rowSize=new int[b.Rows];   //b矩阵每行的非零元素个数
	int *rowStart=new int[b.Rows+1];    //b矩阵每行第一个非零元素在b中的下标；为何加一？
	int *temp=new int[b.Cols];  // 暂时存放Result每一行每个元素的运算结果
	int i,Current,lastInResult,RowA,ColA,ColB;  //Current：a的指针； lastInResult：Result的指针
	for(i=0;i<b.Rows;i++)    //对roeSize数组赋值
		rowSize[i]=0;
	for(i=0;i<b.Terms;i++)
		rowSize[b.smArray[i].row]++;
	rowStart[0]=0;      //对rowStart数组赋值
	for(i=1;i<b.Rows;i++)
		rowStart[i]=rowStart[i-1]+rowSize[i-1];
	Current=0;   //从下标0开始遍历a
	lastInResult=-1; //赋初值-1是为了计算方便，看完代码即可了解
	while(Current<Terms){  //遍历三元组a的每一个元素
	RowA=smArray[Current].row; //取得第一个元素的所在行
	for(i=0;i<b.Cols;i++) //把temp数组赋初值为0；
		temp[i]=0;
	while(Current<Terms&&smArray[Current].row==RowA){ //对该行的所有元素进行操作
		ColA=smArray[Current].col; //该元素所在的列ColA就是该元素对应相乘的b中元素的行数
		for(i=rowStart[ColA];i<rowStart[ColA+1];i++){ //遍历b中该行的所有元素
			ColB=b.smArray[i].col; //a的该元素与b中对应行第ColB列的元素相乘的结果应该被放到temp[ColB]中；
			temp[ColB]+=smArray[Current].value*b.smArray[i].value;
		}
		Current++; //a中该元素的已经完成使命，指向a中下一个元素
	}
	for(i=0;i<b.Cols;i++){  //把该行的运算结果存入Result里面
		if(temp[i]!=0){
		lastInResult++;
		Result.smArray[lastInResult].row=smArray[Current].row;
		Result.smArray[lastInResult].col=i;
		Result.smArray[lastInResult].value=temp[i];
		}
	}
}
	Result.Rows=Rows;     //对Result的性质进行赋值操作
	Result.Cols=b.Cols;
	Result.Terms=lastInResult+1;
	delete[] rowSize; //释放new创建的存储空间
	delete[] rowStart;
	delete[] temp;
	return Result;
}