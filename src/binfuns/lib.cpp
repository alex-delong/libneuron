#include <cmath>
#include <binfuns.hpp>
namespace BinFuns {
    bool is_odd(int i) {
        return i % 2 != 0;
    }
    void init_bin(bool* bin_arr) {
        for (int j = 0; j < 32; j++) {
            bin_arr[j] = 0;
        }
    }
    //  post-conditions: 
    //      returns an array of bools that represent the argument in binary  
    bool* to_bin(unsigned int i) {
        unsigned int curr_pwr = 0;
        bool* bin_arr = new bool[32];
        init_bin(bin_arr);
        unsigned int result = i;
        while (result != 0 || curr_pwr > 31) {
            if (is_odd(result)) {
                bin_arr[31 - curr_pwr] = 1;
                result -= 1;
            } else {
                curr_pwr += 1;
                result = result / 2;
            }
        }
        return bin_arr;
    }
    unsigned int to_int(bool* bin_arr) {
        unsigned int output = 0;
        for (int i = 0; i < 32; i++) {
            output += pow(2, i)*bin_arr[31 - i];
        }
        delete[] bin_arr;
        return output;
    }
}
