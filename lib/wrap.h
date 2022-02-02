#ifndef WRAP_H
#define WRAP_H

// Interface Wrapper (Memory Management)
template <class T>
class wrap {
public:
    wrap() {
        wrapPtr = 0;
    }
    wrap(const T& object) {
        wrapPtr = object.copy();
    }
    wrap(const wrap<T>& wrapObject) {
        if (wrapObject.wrapPtr != 0)
            wrapPtr = wrapObject.wrapPtr->copy();
        else wrapPtr = 0;
    }
    ~wrap() {
        if (wrapPtr != 0) delete wrapPtr;
    }
    wrap& operator=(const wrap<T>& wrapObject) {
        if (this != &wrapObject) {
            if (wrapPtr != 0) delete wrapPtr;
            wrapPtr = (wrapObject.wrapPtr != 0) ? wrapObject.wrapPtr->copy() : 0;
        }
        return *this;
    }
    T& operator*() {
        return *wrapPtr;
    }
    const T& operator*() const {
        return *wrapPtr;
    }
    T* operator->() {
        return wrapPtr;
    }
    const T* operator->() const {
        return wrapPtr;
    }
private:
    T* wrapPtr;
};

#endif
