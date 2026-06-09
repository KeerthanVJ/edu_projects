// Vector.h
//Keerthan Vijayavel

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Vector {
private:
    //Class Variables
    T* idata;
    int size;
    int capc;

    void rsize(int numCol) {
        T* temp = new T[numCol];
        int c = (numCol < size) ? numCol : size;
        
        //Create temp array
        for (int x = 0; x < c; ++x)
            temp[x] = idata[x];
        
        //Deletion
        delete[] idata;
        idata = temp;
        capc = numCol;
        
        if (size > capc)
            size = capc;
    }

public:
    // Default constructor
    Vector() : idata(nullptr), size(0), capc(0) {}
    
    
    // Initial size constructor
    Vector(int n) : size(n), capc(n) {
        idata = new T[capc];
        for (int x = 0; x < size; ++x)
            idata[x] = T();
    }
    
    // Assignment operator
    Vector& operator=(const Vector& source) {
        if (this == &source)
            return *this;
            
        delete[] idata;
        size = source.size;
        capc = source.size;
        idata = new T[capc];
        
        for (int x = 0; x < size; ++x)
            idata[x] = source.idata[x];
        
        return *this;
    }
    
    // Copy constructor
    Vector(const Vector& source) : size(source.size), capc(source.size) {
        idata = new T[capc];
        for (int x = 0; x < size; ++x)
            idata[x] = source.idata[x];
    }
    
    // Destructor
    ~Vector() {
        delete[] idata;
    }
    
    //attaching element
    void pushBack(const T& b) {
        if (size >= capc) {
            int numCol = capc == 0 ? 1 : capc * 2;
            rsize(numCol);
        }
        idata[size++] = b;
    }
    
    
    T& operator[](int i) {
        if (i < 0 || i >= size)
            throw std::out_of_range("This is out of range.");
        return idata[i];
    }
    
    const T& operator[](int i) const {
        if (i < 0 || i >= size)
            throw std::out_of_range("This is out of range.");
        return idata[i];
    }
    
    // Get size
    int getSize() const {
        return size;
    }
    
    // Clearing
    void clear() {
        delete[] idata;
        idata = nullptr;
        size = 0;
        capc = 0;
    }
};

#endif // VECTOR_H
