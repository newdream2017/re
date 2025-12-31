#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <sstream>

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

class FinalSolution {
public:
    std::string compute(const std::string& input) {
        uint32_t ctx[4] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };

        // 1. Padding
        std::vector<uint8_t> msg(input.begin(), input.end());
        size_t origLen = input.length();
        msg.push_back(0x80);
        while (msg.size() % 64 != 56) msg.push_back(0x00);
        uint64_t bitLen = (uint64_t)origLen * 8;
        for (int i = 0; i < 8; i++) msg.push_back((uint8_t)((bitLen >> (i * 8)) & 0xFF));

        // 2. Constants
        const uint32_t K[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };
        const int S[64] = {
            7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
        };

        // 3. Main Loop
        for (size_t offset = 0; offset < msg.size(); offset += 64) {
            uint32_t M[16];
            std::memcpy(M, &msg[offset], 64);
            uint32_t a = ctx[0], b = ctx[1], c = ctx[2], d = ctx[3];

            for (int j = 0; j < 64; j++) {
                uint32_t f, g;
                if (j < 16) { f = (b & c) | ((~b) & d); g = j; }
                else if (j < 32) { f = (d & b) | ((~d) & c); g = (5 * j + 1) % 16; }
                else if (j < 48) { f = b ^ c ^ d; g = (3 * j + 5) % 16; }
                else { f = c ^ (b | (~d)); g = (7 * j) % 16; }
                uint32_t temp = a + f + K[j] + M[g];
                a = d; d = c; c = b;
                b = b + ROTL(temp, S[j]);
            }

            // 4. Final Aggregation (Verified)
            ctx[0] += a;
            ctx[1] += b;
            ctx[2] += c;
            ctx[3] += d;
        }
        return formatHex(ctx);
    }

private:
    std::string formatHex(uint32_t* states) {
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            uint32_t val = states[i];
            for (int j = 0; j < 4; j++) {
                ss << std::setw(2) << (int)((val >> (j * 8)) & 0xFF) << " ";
            }
        }
        return ss.str();
    }
};

int main() {
    FinalSolution solver;
    std::cout << "Verified Result: " << solver.compute("xuzhao20041108") << std::endl;
    return 0;
}
