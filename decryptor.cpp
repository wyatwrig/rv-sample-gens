#include <iostream>
#include <fstream>
#include <string>

void xorDecrypt(const std::string &inputFile, char key) {
    std::ifstream input(inputFile, std::ios::binary);
    if (!input) {
        printf("[!] Failed to open the input file.\n");
        exit(1);
    }

    std::ofstream output("decrypt.txt", std::ios::binary);
    if (!output) {
        printf("[!] Failed to open the output file.\n");
        exit(1);
    }

    char byte;
    while (input.get(byte)) {
        char decryptedChar = byte ^ key;
        output.put(decryptedChar);
    }

    input.close();
    output.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <key>", argv[0]);
        return 1;
    }

    std::string inputFile = argv[1];
    char key = std::stoi(argv[2]);

    xorDecrypt(inputFile, key);
    return 0;
}