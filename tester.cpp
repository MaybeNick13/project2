#include <iostream>
#include <fstream>
#include <cstdint>

int main() {
    // Open the binary file for reading
    std::ifstream file("input.dat", std::ios::binary);

    if (file.is_open()) {
        // Seek to the 4th byte (0-based index)
        file.seekg(4, std::ios::beg);

        // Read 4 bytes into a buffer
        char buffer[4];
        file.read(buffer, 4);

        if (file.good()) {
            // Convert the little-endian bytes to an integer
            int result = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
                             (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
                             (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
                             (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );

            std::cout << "The integer value is: " << result << std::endl;
        } else {
            std::cerr << "Error reading the file." << std::endl;
        }

        file.close();
    } else {
        std::cerr << "Error opening the file." << std::endl;
    }

    return 0;
}