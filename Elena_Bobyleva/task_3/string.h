//
// Created by Goddess on 13.11.2021.
//

#ifndef ATP_CPP_SEMESTER_1_STRING_H
#define ATP_CPP_SEMESTER_1_STRING_H

#include <iostream>
#include <cstring>

class String {
public:
    //Constructors
    String() : _pointer(new char[10]), _size(0), _memory(10) {}

    String(const char* _string) : _pointer(new char[2 * strlen(_string)]), _size(strlen(_string)),
                                  _memory(2 * strlen(_string)) {
        memcpy(_pointer, _string, _size);
    }

    String(size_t n, char c) : _pointer(new char[2 * n]), _size(n), _memory(2 * n) {
        memset(_pointer, c, n);
    }

    String(const String& orig) {
        copy_method(orig);
    }

    // Operators
    String& operator=(const String& orig) {
        if (this != &orig) {
            clear();
            copy_method(orig);
        }
        return *this;
    }

    bool operator==(const String& orig) {
        if (orig.length() == _size) {
            for (size_t i = 0; i < _size; ++i) {
                if (orig[i] != _pointer[i])
                    return false;
            }
        }
        return true;
    }

    //
    String& operator+=(char _char) {
        push_back(_char);
        return *this;
    }

    String& operator+=(const String& _myString) {
        while (_size + _myString._size >= _memory)
            double_memory();
        for (size_t i = _size; i < _size + _myString._size; i++)
            _pointer[i] = _myString._pointer[i - _size];
        _size += _myString.length();
        return *this;
    }

    //
    String operator+(char _char) const {
        String _result(*this);
        _result += _char;
        return _result;
    }

    String operator+(const String& _myString) const {
        String _result(*this);
        _result += _myString;
        return _result;
    }

    char& operator[](size_t _index) {
        return _pointer[_index];
    }

    char operator[](size_t _index) const {
        return _pointer[_index];
    }

    friend std::ostream& operator<<(std::ostream& out, const String& s);

    friend std::istream& operator>>(std::istream& in, String& s);

    // Destructor
    ~String() {
        delete[] _pointer;
    }

    //Methods
    size_t length() const {
        return _size;
    }

    void pop_back() {
        _size--;
        if (_size * 2 < _memory) {
            char* new_str = new char[3 * _memory / 4];
            memcpy(new_str, _pointer, _size);
            delete[] _pointer;
            _memory = 3 * _memory / 4;
            _pointer = new_str;
        }
    }

    void push_back(char _char) {
        if (_size >= _memory)
            double_memory();

        _pointer[_size] = _char;
        _size++;
    }

    char& front() {
        return _pointer[0];
    }

    const char& front() const {
        return _pointer[0];
    }

    char& back() {
        return _pointer[_size - 1];
    }

    const char& back() const {
        return _pointer[_size - 1];
    }

    void swap(String& _myString) {
        std::swap(_pointer, _myString._pointer);
        std::swap(_size, _myString._size);
        std::swap(_memory, _myString._memory);
    }

    String substr(size_t start, size_t count) const {
        String s(count, '0');
        for (size_t i = start; i < start + count; ++i) {
            s[i - start] = _pointer[i];
        }
        return s;
    }

    size_t find(const String& s) const {
        size_t len = s.length();
        for (size_t i = 0; i <= _size - len; ++i) {
            if (this->substr(i, len) == s) {
                return i;
            }
        }
        return _size;
    }

    size_t rfind(const String& s) const {
        size_t len = s.length();
        size_t ans = _size;
        for (size_t i = 0; i <= _size - len; ++i) {
            if (this->substr(i, len) == s) {
                ans = i;
            }
        }
        return ans;
    }

    void clear() {
        delete[] _pointer;
        _size = 0;
        _memory = 10;
        _pointer = new char[_memory];
    }

    bool empty() {
        return _size == 0;
    }

private:
    char* _pointer;
    size_t _size;
    size_t _memory;

    void copy_method(const String& orig) {
        _size = orig._size;
        _memory = orig._memory;
        _pointer = new char[_memory];
        memcpy(_pointer, orig._pointer, _size);
    }

    void double_memory() {
        String _tmp(_memory, '0');
        for (size_t i = 0; i < _size; i++)
            _tmp._pointer[i] = _pointer[i];
        _tmp._size = _size;
        _tmp.swap(*this);
    }

};

void swap(String& _left_operand, String& _right_operand) {
    _left_operand.swap(_right_operand);
}

String operator+(char _char, const String& _myString) {
    return String(1, _char) + _myString;
}

std::ostream& operator<<(std::ostream& out, const String& s) {
    for (size_t i = 0; i < s._size; ++i) {
        out << s[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& s) {
    s.clear();
    char c = '1';
    while (in.get(c) && c != ' ' && !std::isspace(c)) {
        s.push_back(c);
    }
    return in;
}


#endif //ATP_CPP_SEMESTER_1_STRING_H
