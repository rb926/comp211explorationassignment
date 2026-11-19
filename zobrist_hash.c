#include <stdio.h>
#include <stdint.h>

// Fast 64-bit random number generator using only bitwise shifts and XOR
uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13; // XOR with itself shifted left by 13 (bits from the right side are mixed into the left)
    x ^= x >> 7; // Scrambled bits are mixed back into the lower side
    x ^= x << 17; // Mixes the bits from the right into the left once more to make one small change in the starting state produce a very different number (randomness)
    *state = x; // The reason why the shifts aren't too large is that there would then be too many left side bits removed from shifting left and too many zeroes added
    return x;
}

int main() {
    uint64_t seed = 0x123456789ABCDEF0ULL; // Arbitrary non-zero seed
    uint64_t knight_keys[64];

    // Populate the 64 random keys for a Knight using xorshift
    for (int sq = 0; sq < 64; sq++) {
        knight_keys[sq] = xorshift64(&seed); // Pass-by-pointer sets seed to state every iteration. The previous state is given as the parameter for each new square.
    }

    // Start with a Knight on b1 (square 1)
    uint64_t board_hash = 0;
    board_hash ^= knight_keys[1]; // Square 1
    printf("Start (Knight on b1):  0x%016llX\n", (unsigned long long)board_hash);

    // Move Knight from b1 to c3 (square 18)
    board_hash ^= knight_keys[1];  // XORs knight_keys[1] with itself, toggling board hash to 0
    board_hash ^= knight_keys[18]; // Toggles c3 on
    printf("After moving knight to c3:      0x%016llX\n", (unsigned long long)board_hash);

    // 4. Move Knight back from c3 to b1
    board_hash ^= knight_keys[18]; // XORs knight_keys[18] with itself, toggling board hash to 0
    board_hash ^= knight_keys[1];  // Toggles b1 back on
    printf("Moved back to b1:      0x%016llX\n", (unsigned long long)board_hash, " (same as initial)"); // Should be same as initial

    return 0;
}
  
