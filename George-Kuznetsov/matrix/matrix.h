#include <iostream>

#include <vector>

#include <string>

#include <algorithm>

class BigInteger {
public:
    //Constructors
    BigInteger() = default;
    BigInteger(long long num) {
        size = 1;
        blocks = std::vector < long long > (size);
        positive = num >= 0;
        blocks[0] = std::abs(num);
    }
    BigInteger(const std::string & num) {
        if (num[0] == '-') {
            positive = false;
        }
        long long in_size = num.size() - 1 + positive;
        size = in_size / pr + 1;
        blocks = std::vector < long long > (size);
        int counter = 0;
        for (long long i = num.size() - 1; i >= 1 - positive; i -= pr) {
            if (i >= pr - 1 + 1 - positive) {
                blocks[counter] = std::stoi(num.substr(i - pr + 1, pr));
                counter++;
            } else {
                blocks[counter] = std::stoi(num.substr(1 - positive, i + positive));
            }
        }
        resize();
        adduction();
    }
    BigInteger(const BigInteger& num) {
        size = num.size;
        blocks = num.blocks;
        positive = num.positive;
    }
    //Operators
    BigInteger & operator += (const BigInteger & num) {
        bool old_sign = positive;
        if (positive == num.positive) {
            tot_unsigned(num);
            positive = old_sign;
        } else {
            subtract_unsigned(num);
            if (!old_sign) {
                positive = !positive;
            }
        }
        resize();
        adduction();
        return *this;
    }
    BigInteger & operator -= (const BigInteger & num) {
        bool old_sign = positive;
        if (positive == num.positive) {
            subtract_unsigned(num);
            if (!old_sign) {
                positive = !positive;
            }
        } else {
            tot_unsigned(num);
            positive = old_sign;
        }
        resize();
        adduction();
        return *this;
    }
    BigInteger & operator *= (const BigInteger & num) {
        std::vector < long long > copy(size + num.size);
        for (long long i = 0; i < size; ++i) {
            for (int j = 0; j < num.size; ++j) {
                copy[i + j] += blocks[i] * num.blocks[j];
                if (copy[i + j] > base) {
                    copy[i + j + 1] += copy[i + j] / base;
                    copy[i + j] %= base;
                }
            }
        }
        positive = !(positive ^ num.positive);
        blocks = copy;
        size += num.size;
        resize();
        adduction();
        return *this;
    }
    BigInteger & operator /= (const BigInteger & num) {
        divide_unsigned(num);
        positive = !(positive ^ num.positive);
        resize();
        adduction();
        return *this;
    }
    BigInteger & operator %= (const BigInteger & num) {
        blocks = divide_unsigned(num);
        size = blocks.size();
        resize();
        adduction();
        return *this;
    }
    BigInteger & operator = (const BigInteger & num) {
        size = num.size;
        blocks = num.blocks;
        positive = num.positive;
        return *this;
    }
    BigInteger & operator++() {
        *this += 1;
        return *this;
    }
    BigInteger & operator--() {
        *this -= 1;
        return *this;
    }
    BigInteger operator++(int) {
        BigInteger copy = * this;
        ++(*this);
        return copy;
    }
    BigInteger operator--(int) {
        BigInteger copy = * this;
        --(*this);
        return copy;
    }
    //Implicit conversions
    explicit operator int() {
        long long result = 0;
        if (size > 1) result += blocks[1];
        if (size > 0) result += blocks[0] * base;
        return int(result);
    }
    explicit operator long long() {
        return blocks[0];
    }
    explicit operator bool() {
        bool sooth = false;
        if (positive && size > 0 && blocks[0] > 0) sooth = true;
        return sooth;
    }
    friend bool operator < (const BigInteger & num_1,
                            const BigInteger & num_2);
    //Methods
    void clear() {
        blocks.clear();
        size = 0;
    }
    [[nodiscard]] std::string toString() const {
        std::string output;
        if (!positive) {
            output.push_back('-');
        }
        for (long long i = size - 1; i >= 0; --i) {
            auto number = std::to_string(blocks[i]);
            if (i == size - 1) {
                output += number;
            } else {
                output += std::string(9 - number.length(), '0') + number;
            }
        }
        return output;
    }
    [[nodiscard]] BigInteger abs() const {
        BigInteger copy = *this;
        copy.positive = true;
        return copy;
    }
private:
    std::vector < long long > blocks; // in reverse order
    long long size = 0; //amount of blocks
    int base = 1e9;
    bool positive = true;
    int pr = 9;
    void adduction() {
        for (long long i = 0; i < size; ++i) {
            if (blocks[i] >= base) {
                if (i == size - 1) {
                    blocks.push_back(0);
                    size++;
                }
                blocks[i + 1] += blocks[i] / base;
                blocks[i] %= base;
            }
            if (blocks[i] < 0) {
                if (i == size - 1) {
                    blocks[i] *= -1;
                    positive = false;
                } else {
                    blocks[i] += base;
                    blocks[i + 1]--;
                }
            }
        }
        if (size == 1 && blocks[0] == 0) {
            positive = true;
        }
    }
    void resize() {
        for (long long i = size - 1; i > 0; --i) {
            if (blocks[i] == 0) {
                blocks.pop_back();
                size--;
            } else {
                i = 0;
            }
        }
    }
    void tot_unsigned(const BigInteger & num) {
        blocks.resize(std::max(size, num.size) + 1);
        for (long long i = 0; i < num.size; ++i) {
            blocks[i] += num.blocks[i];
            if (blocks[i] >= base) {
                blocks[i + 1]++;
                blocks[i] -= base;
            }
        }
        positive = true;
        size = std::max(size, num.size) + 1;
    }
    void subtract_unsigned(const BigInteger & num) {
        blocks.resize(std::max(size, num.size));
        for (long long i = 0; i < num.size; ++i) {
            blocks[i] -= num.blocks[i];
            if (blocks[i] < 0 && i != num.size - 1) {
                blocks[i] += base;
                blocks[i + 1]--;
            }
        }
        positive = true;
        size = std::max(size, num.size);
        if (blocks[size - 1] < 0) {
            positive = false;
            blocks[size - 1] *= -1;
            for (long long i = size - 2; i >= 0; --i) {
                blocks[i] = base - blocks[i];
                blocks[i + 1]--;
            }
        }
    }
    std::vector < long long > divide_unsigned(const BigInteger & num) {
        std::vector < long long > carry(num.size);
        std::vector < long long > result(size);
        std::vector < long long > count_arr;
        long long upper_bound = 0;
        long long bottom_bound = 0;
        long long current_bound = 0;
        for (long long i = 0; i < size; ++i) {
            carry.insert(carry.begin(), blocks[size - 1 - i]);
            carry[carry.size() - 2] += carry[carry.size() - 1] * base;
            carry.pop_back();
            //quick check
            upper_bound = carry[carry.size() - 1] / num.blocks[num.size - 1];
            bottom_bound = carry[carry.size() - 1] / (num.blocks[num.size - 1] + 1);
            current_bound = (upper_bound + bottom_bound) / 2;
            while (upper_bound != bottom_bound) {
                count_arr = carry;
                for (long long j = 0; j < num.size; ++j) {
                    count_arr[j] -= current_bound * num.blocks[j];
                    if (count_arr[j] < 0) {
                        if (j != num.size - 1) {
                            count_arr[j + 1] -= std::abs(count_arr[j]) / base;
                            if (std::abs(count_arr[j]) % base) count_arr[j + 1]--;
                            count_arr[j] %= base;
                            count_arr[j] += base;
                        }
                    }
                }
                if (count_arr[num.size - 1] < 0) {
                    if (upper_bound - bottom_bound == 1) upper_bound = bottom_bound;
                    else {
                        upper_bound = current_bound;
                        current_bound = (bottom_bound + upper_bound) / 2;
                    }
                } else {
                    if (upper_bound - bottom_bound == 1) {
                        if (current_bound == upper_bound) bottom_bound = upper_bound;
                        else current_bound = upper_bound;
                    } else {
                        bottom_bound = current_bound;
                        current_bound = (bottom_bound + upper_bound) / 2;
                    }
                }
            }
            result[i] = bottom_bound;
            for (long long j = 0; j < num.size; ++j) {
                carry[j] -= bottom_bound * num.blocks[j];
                if (carry[j] < 0) {
                    carry[j + 1] -= std::abs(carry[j]) / base;
                    if (std::abs(carry[j]) % base != 0) carry[j + 1]--;
                    carry[j] %= base;
                    if (carry[j] < 0) carry[j] += base;
                }
            }
        }
        std::reverse(result.begin(), result.end());
        blocks = result;
        size = result.size();
        return carry;
    }
};
//Operators
BigInteger operator + (const BigInteger & num_1,
                       const BigInteger & num_2) {
    BigInteger copy = num_1;
    copy += num_2;
    return copy;
}
BigInteger operator - (const BigInteger & num_1,
                       const BigInteger & num_2) {
    BigInteger copy = num_1;
    copy -= num_2;
    return copy;
}
BigInteger operator * (const BigInteger & num_1,
                       const BigInteger & num_2) {
    BigInteger copy = num_1;
    copy *= num_2;
    return copy;
}
BigInteger operator / (const BigInteger & num_1,
                       const BigInteger & num_2) {
    BigInteger copy = num_1;
    copy /= num_2;
    return copy;
}
BigInteger operator % (const BigInteger & num_1,
                       const BigInteger & num_2) {
    BigInteger copy = num_1;
    copy %= num_2;
    return copy;
}
BigInteger operator - (const BigInteger & num) {
    BigInteger copy = num;
    copy *= -1;
    return copy;
}
//Logic operators
bool operator < (const BigInteger & num_1,
                 const BigInteger & num_2) {
    int result = 0;
    // 0 - less
    //1 - more
    //2 - equal
    if (num_1.positive && !num_2.positive) {
        return false;
    } else if (!num_1.positive && num_2.positive) {
        return true;
    } else {
        if (num_2.size < num_1.size) {
            result = 1;
        } else if (num_2.size == num_1.size) {
            long long i = num_1.size - 1;
            while(num_1.blocks[i] == num_2.blocks[i] && i > 0) --i;
            if (num_1.blocks[i] > num_2.blocks[i]) {
                result = 1;
            } else if (num_1.blocks[i] < num_2.blocks[i]) {
                result = 0;
            } else {
                return false;
            }
        }
        if (!num_1.positive) {
            result += 1;
            result %= 2;
        }
        return !result;
    }

}
bool operator > (const BigInteger & num_1,
                 const BigInteger & num_2) {
    return num_2 < num_1;
}
bool operator == (const BigInteger & num_1,
                  const BigInteger & num_2) {
    return !((num_2 < num_1) || (num_1 < num_2));
}
bool operator != (const BigInteger & num_1,
                  const BigInteger & num_2) {
    return !(num_1 == num_2);
}
bool operator <= (const BigInteger & num_1,
                  const BigInteger & num_2) {
    return (num_1 < num_2) || (num_1 == num_2);
}
bool operator >= (const BigInteger & num_1,
                  const BigInteger & num_2) {
    return (num_1 > num_2) || (num_1 == num_2);
}
//Output operator
std::ostream & operator << (std::ostream & output,
                            const BigInteger & biggie) {
    output << biggie.toString();
    return output;
}
//Input operator
std::istream & operator >> (std::istream & input, BigInteger & biggie) {
    input.tie(nullptr);
    biggie.clear();
    std::string input_num;
    input >> input_num;
    biggie = BigInteger(input_num);
    return input;
}
//######################################################################################################################
//######################################################################################################################
class Rational {
public:
    //Constructors
    Rational() =default;
    Rational(const BigInteger & num): numerator(num) {}
    Rational(int num): numerator(num) {}
    //Operators
    Rational & operator += (const Rational & num) {
        numerator *= num.denominator;
        numerator += num.numerator * denominator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational & operator -= (const Rational & num) {
        numerator *= num.denominator;
        numerator -= num.numerator * denominator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational & operator *= (const Rational & num) {
        numerator *= num.numerator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational & operator /= (const Rational & num) {
        numerator *= num.denominator;
        if (num.numerator < 0) {
            numerator *= -1;
            denominator *= (-num.numerator);
        } else {
            denominator *= num.numerator;
        }
        shrink();
        return *this;
    }
    friend bool operator < (const Rational & num_1,
                            const Rational & num_2);
    explicit operator double() {
        std::string output = asDecimal(20);
        return std::atof(output.c_str());
    }
    //Methods
    std::string toString() const {
        std::string output = numerator.toString();
        if (denominator != 1) {
            output.push_back('/');
            output += denominator.toString();
        }
        return output;
    }
    std::string asDecimal(size_t precision = 0) {
        std::string precised;
        BigInteger copy = numerator;
        if (numerator * denominator < 0 && copy / denominator == 0) precised += '-';
        precised += (copy / denominator).toString();
        copy = copy.abs();
        copy %= denominator;
        for (size_t i = 0; i <= precision; ++i) {
            if (i == 0) precised.push_back('.');
            else {
                copy *= 10;
                precised += (copy / denominator).toString();
                copy %= denominator;
            }
        }
        return precised;
    }
private:
    BigInteger numerator;
    BigInteger denominator = 1;
    //Methods
    BigInteger gcd(const BigInteger & num_1,
                   const BigInteger & num_2) {
        if (num_2 == 0) return num_1;
        return gcd(num_2, num_1 % num_2);
    }
    void shrink() {
        BigInteger num = numerator.abs();
        BigInteger den = denominator.abs();
        if (denominator != 1) {
            while (den != 0) {
                std::swap(num, den);
                den %= num;
            }
            BigInteger gcf = num;
            if (gcf != 1) {
                numerator /= gcf;
                denominator /= gcf;
            }
        }
    }
};
//Operators
Rational operator + (const Rational & num_1,
                     const Rational & num_2) {
    Rational copy = num_1;
    copy += num_2;
    return copy;
}
Rational operator - (const Rational & num_1,
                     const Rational & num_2) {
    Rational copy = num_1;
    copy -= num_2;
    return copy;
}
Rational operator * (const Rational & num_1,
                     const Rational & num_2) {
    Rational copy = num_1;
    copy *= num_2;
    return copy;
}
Rational operator / (const Rational & num_1,
                     const Rational & num_2) {
    Rational copy = num_1;
    copy /= num_2;
    return copy;
}
Rational operator - (const Rational & num_1) {
    Rational copy = num_1;
    copy *= -1;
    return copy;

}
//Logical operators
bool operator < (const Rational & num_1,
                 const Rational & num_2) {
    return num_1.numerator * num_2.denominator < num_2.numerator * num_1.denominator;
}
bool operator > (const Rational & num_1,
                 const Rational & num_2) {
    return num_2 < num_1;
}
bool operator == (const Rational & num_1,
                  const Rational & num_2) {
    return !(num_2 < num_1 || num_1 < num_2);
}
bool operator != (const Rational & num_1,
                  const Rational & num_2) {
    return !(num_1 == num_2);
}
bool operator <= (const Rational & num_1,
                  const Rational & num_2) {
    return !(num_1 > num_2);
}
bool operator >= (const Rational & num_1,
                  const Rational & num_2) {
    return !(num_1 < num_2);
}
std::istream& operator >> (std::istream& input,
                                Rational& rattie) {
    input.tie(nullptr);
    std::string input_num;
    input >> input_num;
    rattie = Rational(input_num);
    return input;
}
std::ostream & operator << (std::ostream & output,
                            const Rational & biggie) {
    output << biggie.toString();
    return output;
}
//######################################################################################################################
//######################################################################################################################
template < size_t N, size_t D, bool end = false >
struct is_Prime {
    static
    const bool is_prime = (N == D) || ((N % D != 0) && is_Prime < N, D + 1, D * D >= N > ::is_prime);
};
template < size_t N, size_t D >
struct is_Prime < N, D, true > {
    static
    const bool is_prime = true;
};

template < size_t N >
class Residue {
public:
    explicit Residue < N > (int x) {
        while (x < 0) x += N;
        value = x % N;
    }
    explicit Residue < N > (size_t x): value(x % N) {}
    Residue < N > (const Residue < N > & residue): value(residue.value) {}
    explicit Residue < N > (): value(0) {}
    Residue < N > & operator += (const Residue < N > & res) {
        value = (value + res.value) % N;
        return *this;
    }
    Residue < N > & operator -= (const Residue < N > & res) {
        value += N;
        value -= res.value;
        value %= N;
        return *this;
    }
    Residue < N > & operator *= (const Residue < N > & res) {
        value *= res.value;
        value %= N;
        return *this;
    }
    template < typename = typename std::enable_if_t < is_Prime < N, 2 > ::is_prime >>
    Residue < N > & operator /= (const Residue < N > & res) {
        value *= get_opposite(res.value);
        value %= N;
        return *this;
    }
    explicit operator int() const {
        return int(value);
    }
    ~Residue < N > () = default;
    bool operator == (const Residue < N > & res) const {
        return value == res.value;
    }

    bool operator != (const Residue < N > & res) const {
        return !(value == res.value);
    }
private:
    size_t value;

    size_t get_opposite(size_t val) {
        size_t res = 1;
        size_t power = N - 2;
        while (power > 0) {
            if (power & 1) res = (res * val) % N;
            if (power != 1) res = (res * res) % N;
            power >>= 1;
        }
        return res;
    }

};

template < size_t N >
Residue < N > operator + (const Residue < N > & res_1,
                          const Residue < N > & res_2) {
    Residue < N > copy(res_1);
    copy += res_2;
    return copy;
}
template < size_t N >
Residue < N > operator - (const Residue < N > & res_1,
                          const Residue < N > & res_2) {
    Residue < N > copy(res_1);
    copy -= res_2;
    return copy;
}
template < size_t N >
Residue < N > operator * (const Residue < N > & res_1,
                          const Residue < N > & res_2) {
    Residue < N > copy(res_1);
    copy *= res_2;
    return copy;
}
template < size_t N, typename = typename std::enable_if_t < is_Prime < N, 2 > ::is_prime >>
Residue < N > operator / (const Residue < N > & res_1,
                          const Residue < N > & res_2) {
    Residue < N > copy(res_1);
    copy /= res_2;
    return copy;
}
template < size_t N >

std::ostream & operator >> (std::ostream & input,
                            Residue < N > & jennifer) {
    input.tie(nullptr);
    jennifer(input);
    return input;
}
template < size_t N >
std::ostream & operator << (std::ostream & output,
                            const Residue < N > & jennifer) {
    output.tie(nullptr);
    output << int(jennifer);
    return output;
}
//######################################################################################################################
//######################################################################################################################
template < size_t M, size_t N, typename Field = Rational >
class Matrix {
public:
    Matrix() {
        for (size_t i = 0; i < std::min(M, N); ++i) {
            matrix[i][i] = Field(1);
        }
    }
    explicit Matrix(Field val) {
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {
                matrix[i][j] = val;
            }
        }
    }
    explicit Matrix(const std::vector < std::vector < Field >> val): matrix(std::move(val)) {}
    Matrix(std::initializer_list < std::initializer_list < int >> mat): matrix(std::vector(M, std::vector(N, Field()))) {
        size_t row = 0;
        for (auto i: mat) {
            size_t column = 0;
            for (int j: i)
                matrix[row][column++] = Field(j);
            ++row;
        }
    }
    Matrix(const Matrix < M, N, Field > & mat): matrix(mat.matrix) {}
    template < size_t P, size_t K >
    bool operator == (const Matrix < P, K, Field > & mat) const {
        if (M == P && N == K) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    if (matrix[i][j] != mat.matrix[i][j]) return false;
                }
            }
            return true;
        }
        return false;
    }
    template < size_t P, size_t K >
    bool operator != (const Matrix < P, K, Field > & mat) const {
        return !( * this == mat);
    }
    std::vector < Field > & operator[](const size_t ind) {
        return matrix[ind];
    }
    const std::vector < Field > & operator[](size_t ind) const {
        return matrix[ind];
    }
    Field det() {
        Field result = Field(1);
        std::pair < Matrix < M, N, Field > , int > triangle = triangulate();
        result *= Field(triangle.second);
        for (size_t i = 0; i < std::min(M, N); ++i) {
            result *= triangle.first[i][i];
        }
        return result;
    }
    [[nodiscard]] Field trace() const {
        Field res(0);
        for (size_t i = 0; i < std::min(M, N); ++i) res += matrix[i][i];
        return res;
    }
    [[nodiscard]] size_t rank() const {
        std::pair < Matrix < M, N, Field > , int > triangle = triangulate();
        size_t row = 0, col = 0;
        while ((row < M) && (col < N)) {
            if (triangle.first[row][col] != Field(0)) {
                ++row;
            }
            ++col;
        }
        return row;
    }
    [[nodiscard]] Matrix < N, M, Field > transposed() const {
        Matrix < N, M, Field > transposed;
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j) transposed[i][j] = matrix[j][i];
        return transposed;
    }
    template < typename = typename std::enable_if < M == N >>
    Matrix < N, M, Field > & invert() {
        //create unity matrix and do parallel reforms with both matrix
        Matrix < N, M, Field > inv; //unity matrxi
        for (size_t row = 0; row < M; ++row) {
            size_t leader_row = row;
            while ((leader_row < M) && (matrix[leader_row][row] == Field(0)))
                ++leader_row; //finding index of column leader
            if (leader_row != row && leader_row != M) { //if leader is not on current row
                std::swap(matrix[row], matrix[leader_row]);
                std::swap(inv[row], inv[leader_row]);
            }
            if (matrix[row][row] != Field(0)) {
                Field leader = matrix[row][row];
                for (size_t i = 0; i < N; ++i) { //we make row starting with 1
                    matrix[row][i] /= leader;
                    inv[row][i] /= leader;
                }
                for (size_t j = row + 1; j < M; ++j) { //now recount other rows for in column under leader will be zeros
                    Field temp = matrix[j][row] / matrix[row][row];
                    for (size_t k = 0; k < N; ++k) {
                        matrix[j][k] -= temp * matrix[row][k];
                        inv[j][k] -= temp * inv[row][k];
                    }
                }
            }
        }
        Field leader = Field(0); //now matrix is triangulated with 1 on diagonals, so we must transform matrix to unity
        for (size_t i = M - 1; i > 0; --i) {
            for (size_t j = 0; j < i; ++j) {
                leader = matrix[j][i];
                matrix[j][i] = Field(0);
                for (size_t k = 0; k < N; ++k) {
                    inv[j][k] -= leader * inv[i][k];
                }
            }
        }
        * this = inv;
        return *this;
    }
    Matrix < N, M, Field > inverted() {
        Matrix < N, M, Field > inverted = * this;
        return inverted.invert();
    }
    std::vector < Field > getRow(size_t i) {
        return matrix[i];
    }
    std::vector < Field > getColumn(size_t i) {
        std::vector < Field > column(M);
        for (size_t j = 0; j < M; ++j) column[j] = matrix[i][j];
        return column;
    }
    Matrix < M, N, Field > & operator += (const Matrix < M, N, Field > & matrix1) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j) matrix[i][j] += matrix1.matrix[i][j];
        return *this;
    }
    Matrix < M, N, Field > & operator -= (const Matrix < M, N, Field > & matrix1) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j) matrix[i][j] -= matrix1.matrix[i][j];
        return *this;
    }
    Matrix < M, N, Field > & operator *= (const Field & val) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                matrix[i][j] *= val;
        return *this;
    }
    template < typename = typename std::enable_if < M == N >>
    Matrix < M, N, Field > & operator *= (const Matrix < M, N, Field > & mat) {
        * this = ( * this) * mat;
        return *this;
    }
    Matrix < M, N, Field > triangulated() {
        return triangulate().first;
    }
private:
    std::vector < std::vector < Field >> matrix = std::vector < std::vector < Field >> (M, std::vector < Field > (N));
    [[nodiscard]] std::pair < Matrix < M, N, Field > , int > triangulate() const {
        Matrix < M, N, Field > copy( * this);
        int sign = 1;
        size_t row = 0;
        size_t column = 0;
        while (row < M && column < N) {
            size_t leader_row = row;
            while ((leader_row < M) && (copy[leader_row][column] == Field(0))) ++leader_row;
            if (leader_row != row && leader_row != M) {
                std::swap(copy[row], copy[leader_row]);
                sign *= -1;
            }
            if (leader_row < M) {
                if (copy[row][column] != Field(0)) {
                    for (size_t j = row + 1; j < M; ++j) {
                        Field temp = copy[j][column] / copy[row][column];
                        for (size_t k = column; k < N; ++k) {
                            copy[j][k] -= copy[row][k] * temp;
                        }
                    }
                }
                row++;
            }
            ++column;
        }
        size_t cur_col = 0;
        for (size_t i = 0; i < M; ++i) {
            bool empty = true;
            for (size_t j = cur_col; j < N; ++j) {
                if (copy.matrix[i][j] != Field(0)) {
                    cur_col = j;
                    j = N;
                    empty = false;
                }
            }
            if (empty)
                for (size_t j = i; j < M - 1; ++j) {
                    std::swap(copy.matrix[j], copy.matrix[j + 1]);
                }
        }
        return { copy, sign };
    }

};

//continue
template < size_t M, size_t N, typename Field >
//sum
Matrix < M, N, Field > operator + (const Matrix < M, N, Field > & matrix_1,
                                   const Matrix < M, N, Field > & matrix_2) {
    Matrix < M, N, Field > copy(matrix_1);
    return copy += matrix_2;
}
template < size_t M, size_t N, typename Field >
//diff
Matrix < M, N, Field > operator - (const Matrix < M, N, Field > & matrix_1,
                                   const Matrix < M, N, Field > & matrix_2) {
    Matrix < M, N, Field > copy(matrix_1);
    return copy -= matrix_2;
}
template < size_t M, size_t N, typename Field, size_t K >
//matrix * matrix
Matrix < M, K, Field > operator * (const Matrix < M, N, Field > & matrix_1,
                                   const Matrix < N, K, Field > & matrix_2) {
    Matrix < M, K, Field > copy(std::vector < std::vector < Field >> (M, std::vector < Field > (K)));
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < K; ++j)
            for (size_t k = 0; k < N; ++k) {
                copy[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
    }
    return copy;
}
template < size_t M, size_t N, typename Field >
//scalar * matrix
Matrix < M, N, Field > operator * (const Field & val,
                                   const Matrix < M, N, Field > & matrix_1) {
    Matrix < M, N, Field > copy(matrix_1);
    copy *= val;
    return copy;
}
template < size_t N, typename Field = Rational >
using SquareMatrix = Matrix < N, N, Field > ;
