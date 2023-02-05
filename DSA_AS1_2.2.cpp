#include <iostream>
#include <algorithm>
/*
 * Vsevolod Klyushev BS20-02
 * This is a program for task 2.2.
 * link to codeforces:
 * https://codeforces.com/group/C71Rz4W66e/contest/316517/submission/108029030
 */
using namespace std;
const int Size = 8;

//=====description of List (based on arrays)============================================================================
template <class T>
class List {
private:
    T *data;//pointer to the array of our data
    int length;//full size of initial array
    int listSize;//last index
    void resize();//using the doubling strategy we increase the capacity by 2
    bool needResize();//using compare operation we check if we need to do resize() or not
public:
    List() {//constructor of our List
        data = new T[Size];
        length=Size;
        listSize=0;
    }
    void add(T item);// add a new item to the List
    void add(int i, T item);// add a new item to the i'th position in List
    void print(int begin);//print all list
    void printPair(int begin);//print all list of pairs
};

//=====implementation of List (based on arrays)=========================================================================
template <class T>
void List<T>::resize() {
    T *temp= new T[length*2];
    for (int i=0; i<length; i++) {
        temp[i]=data[i];
    }
    data=temp;
    length*=2;
}

template <class T>
void List<T>::print(int begin) {
    for (int i=begin; i<listSize; i++)
        cout<<data[i]<<",  ";
    cout<<endl;
}

template <class T>
void List<T>::printPair(int begin) {
    if (listSize==0) {
        cout<<endl;
    }
    cout.precision(2);
    for (int i=begin; i<listSize-1; i++)
        cout<<'$'<<fixed<<data[i].first<<data[i].second<<", ";
    cout.precision(2);
    if (listSize>0)
        cout<<'$'<<fixed<<data[listSize-1].first<<data[listSize-1].second<<endl;
}

template <class T>
bool List<T>::needResize() {
    return length==listSize;
}

template <class T>
void List<T>::add(T item) {
    if (needResize())
        resize();
    data[listSize]=item;
    listSize++;
}

template <class T>
void List<T>::add(int i, T item) {
    if (needResize())
        resize();
    for (int j=listSize; j>i; j--) {
        data[j]=data[j-1];
    }
    data[i]=item;
    listSize++;
}

//=====description of Sorted List ADT===================================================================================
/*
 * This is an interface of Sorted List. Since there is no such words as abstract & interface I used virtual function
 * an overloaded them later.
 */
template <typename T>
class SortedList {
public:
    virtual void add(T item)=0;// add a new item to the List
    virtual T least()=0;// return the least element
    virtual T greatest()=0;// return the greatest element
    virtual T get(int i)=0;// return the i-th least element
    virtual int indexOf (T item)=0;// return the index of an element (in a sorted sequence)
    virtual void remove(int i)=0;// remove i-th least element from the list
    virtual List<T> searchRange(T from, T to)=0;// find all items between from and to
    virtual int size()=0;// return the size of the list
    virtual bool isEmpty()=0;// return whether the list is empty
};

//=====implementation of Sorted List ADT with Array List================================================================
/*
 * This is an implementation of a Sorted List based on arrays. Our Array List has the following functions:
 * -void resize() - using the doubling strategy we increase the capacity by 2, the amortized time complexity O(n);
 *
 * -bool needResize() - using compare operation we check if we need to do resize() or not, return bool value,
 * worst case time complexity O(1);
 *
 * -ArraySortedList() - constructor for our class that assign initial values to our variables, worst case
 * time complexity O(1);
 *
 * -void add(T item) - this function gets element "item" of generic class T, check if we need resize, looking for a
 * suitable place and insert an "item" in our array with the shifting of subsequent elements. Since amortised time
 * complexity of resize() is O(n), worst-case time complexity searching of suitable place O(n) and shifting O(n) too,
 * so since all that steps are follow each other the total time complexity is O(n);
 *
 * -T least(), get(int i), greatest() return corresponding elements from array and have worst case time complexity O(1);
 *
 * -indexOf(T item) - return the index of an element (in a sorted sequence), worst case time complexity O(n);
 *
 * -void remove() - remove i-th least element from the list, worst case time complexity O(n);
 *
 * -List<T> searchRange(T from, T to) - find all items between "from" and "to" and returns List of elements,
 * implementation of the usual list that we return is locating above the interface of SortedList, the worst-case time
 * complexity without looking on add to List function is O(n), considering adding the function of List,
 * time complexity O(n^2) due to resize function in add function in List;
 *
 * -List<T> searchRangeForPair(double from, double  to) - doing the same things as previous, but with working with
 * data of class "pair", time complexity is the same as in previous function;
 *
 * -int size() - return the size of the list, time complexity O(1);
 *
 * -bool isEmpty() - return whether the list is empty, time complexity O(1);
 */
template <typename T>
class ArraySortedList: public SortedList<T> {
private:
    T *data;//pointer to the array of our data
    int length;//full size of initial array
    int listSize;//last index
    void resize() {
        T *temp= new T[length*2];
        for (int i=0; i<length; i++) {
            temp[i]=data[i];
        }
        data=temp;
        length*=2;
    }
    bool needResize() {
        return length==listSize;
    }
public:
    ArraySortedList() {
        data = new T[Size];
        length=Size;
        listSize=0;
    }
    void add(T item) override {
        if (needResize())
            resize();
        int index=0;
        while ((index<listSize) && (item>=data[index]))
            index++;
        for (int i=listSize; i>index; i--) {
            data[i]=data[i-1];
        }
        data[index]=item;
        listSize++;
    }
    T least() override{
        return data[listSize-1];
    }
    T greatest() override{
        return data[0];
    }
    T get(int i) override{
        return data[i];
    }
    int indexOf(T item) override{
        bool found= false;
        for (int i=0; i< listSize; i++) {
            if (item==data[i]) {
                found= true;
                return i;
            }
        }
    }
    void remove(int i) override {
        if (i<listSize) {
            for (int j=i; j<listSize-1; j++) {
                data[j]=data[j+1];
            }
            listSize--;
        }
    }
    List<T> searchRange(T from, T to) override {
        List<T> list = List<T>();
        if (from<=to) {
            for (int i=0; i<listSize; i++) {
                if (data[i]<=to && data[i]>=from) {
                    list.add(data[i]);
                }
            }
        }
        return list;
    }
    List<T> searchRangeForPair(double from, double  to) {
        List<T> list = List<T>();
        if (from<=to) {
            for (int i=0; i<listSize; i++) {
                if (((data[i].first)<=to) && (data[i].first>=from)) {
                    list.add(data[i]);
                }
            }
        }
        return list;
    }
    int size() override{
        return listSize;
    }
    bool isEmpty() override{
        return listSize==0;
    }
};

//=====Main part========================================================================================================
//This function erase first char and parse the result to double
double getPrice (string dollar) {
    dollar.erase(dollar.begin());
    return stod(dollar);
}

int main() {
    ArraySortedList<pair<double,string>> list;
    int num;//number of commands
    cin>>num;
    string command;//string with command
    string dollar;//string with price value
    double price;//variable to store price value
    double left;//first price value (from) in order to use it in "LIST" command
    double right;//second price value (from) in order to use it in "LIST" command
    string item;//sring with item
    for (int i=0;i<num; i++) {
        cin>>command;
        if (command=="ADD") {
            cin>>dollar;
            price=getPrice(dollar);
            getline(cin,item, '\n');
            list.add(make_pair(price,item));
        }
        if (command=="REMOVE") {
            cin>>dollar;
            price=getPrice(dollar);
            getline(cin,item, '\n');
            int index=list.indexOf(make_pair(price,item));
            if (index!=-1)
                list.remove(index);
        }
        if (command=="LIST") {
            cin>>dollar;
            left=getPrice(dollar);
            cin>>dollar;
            right=getPrice(dollar);
            List<pair<double,string>> lll = list.searchRangeForPair(left,right);
            lll.printPair(0);
        }
    }
}