# push_back()&emplace_back()

```cpp
class A
{
public:
    int i;
    A(int t) :i(t) { cout << "A()" << endl; }
    A(const A& a) :i(a.i) { cout << "A() copy" << endl; }
    A(A&& a) :i(a.i) { cout << "A() move" << endl; }
};

int main()
{
    A a(1);
    vector<A> v1;
    v1.push_back(a);
    cout << "--------1-------" << endl;

    vector<A> v11;
    v11.push_back(A(2));
    cout << "--------2-------" << endl;

    vector<A> v111;
    v111.push_back(5);
    cout << "--------3-------" << endl; // 此时，push_back仍需构造，再move

    vector<A> v2;
    A aa(1);
    v2.emplace_back(aa);
    cout << "--------4-------" << endl;

    vector<A> v22;
    v22.emplace_back(A(3));
    cout << "--------5-------" << endl;

    vector<A> v222;
    v222.emplace_back(5);
    cout << "--------6-------" << endl; // emplace_back直接构造，无需move
    
    return 0;
}
//A()
//A() copy
//--------1------
//A()
//A() move
//--------2------
//A()
//A() move
//--------3------
//A()
//A() copy
//--------4------
//A()
//A() move
//--------5------
//A()
//--------6------
//从结果中可以看出，如果直接传对象给 push_back 和 emplace_back ，无论是实名对象还是匿名对象，结果都是一样的，但是 emplace_back 不同的是你可以直接传构造对象的参数，然后emplace_back函数里通过参数来直接构造对象，从而少了一次构造，效率更高。
```

- push_back()需要先**构造临时对象**，再将这个对象**拷贝**到容器的末尾。
- emplace_back()则直接在容器的末尾**构造对象**，这样就**省去了拷贝**的过程。
- emplace_back()大部分时候可以取代push_back()，小部分时候某些操作发生异常时，则emplace_back()不如push_back()安全。

```cpp
using namespace std;
int i=0,j=0;
class A {
public:
    A(int i){
        str = to_string(i);
        cout << "构造函数" <<++i<< endl; 
    }
    ~A(){}
    A(const A& a): str(a.str){
        cout << "拷贝构造" <<++j<< endl;
    }
 
public:
    string str;
};
 
int main(){
    vector<A> vec;
    vec.reserve(10); // 开辟capacity
    for(int i=0;i<10;i++){
        vec.push_back(i); //调用了10次构造函数和10次拷贝构造函数。
		// vec.emplace_back(i);  //调用了10次构造函数，1次拷贝构造函数都没有调用过。
    }
}
```

![image-20220518183806743](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220518183806743.png)

![image-20220518183826860](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220518183826860.png)

### 参考链接

[(21条消息) C++11新特性，推荐使用emplace_back()替换push_back()的原因_Kuany_的博客-CSDN博客](https://blog.csdn.net/weixin_44718794/article/details/108321232?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)

[(21条消息) C++ std::vector 的 emplace_back 能否完全取代 push_back_Sprite丶雪碧的博客-CSDN博客](https://blog.csdn.net/u012088909/article/details/105309570?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)