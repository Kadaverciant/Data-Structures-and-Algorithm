#include <iostream>
#include <algorithm>
/*
 * Vsevolod Klyushev BS20-02
 * This is a program for task 2.1.
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
    int size() override{
        return listSize;
    }
    bool isEmpty() override{
        return listSize==0;
    }
};

int main() {
    cout<<"Hello there... General Kenobi";
}
