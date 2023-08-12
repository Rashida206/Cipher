#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

string caesarEncrypt(const string &text, int shift) {
    string encrypted = text;
    for (char &ch : encrypted) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = ((ch - base + shift) % 26) + base;
        }
    }
    return encrypted;
}

string keywordEncrypt(const string &text, const string &keyword) {
    string encrypted = text;
    string key = keyword;
    for (char &ch : key) {
        ch = toupper(ch);
    }

    int keyIndex = 0;
    for (char &ch : encrypted) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            char keyChar = key[keyIndex % key.length()];
            int shift = keyChar - 'A';
            ch = ((ch - base + shift) % 26) + base;
            keyIndex++;
        }
    }
    return encrypted;
}

string hillEncrypt(const string &text, const string &key) {
    string encrypted = text;
    int keySize = static_cast<int>(sqrt(key.length()));

    vector<vector<int>> matrix(keySize, vector<int>(keySize));
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < keySize; ++j) {
            matrix[i][j] = key[i * keySize + j] - 'A';
        }
    }

    while (encrypted.length() % keySize != 0) {
        encrypted += 'X';
    }

    for (size_t i = 0; i < encrypted.length(); i += keySize) {
        for (int j = 0; j < keySize; ++j) {
            int sum = 0;
            for (int k = 0; k < keySize; ++k) {
                sum += matrix[j][k] * (encrypted[i + k] - 'A');
            }
            encrypted[i + j] = (sum % 26) + 'A';
        }
    }
    return encrypted;
}

string playfairEncrypt(const string &text, const string &key) {
    string encrypted = text;
    string playfairKey = key + "ABCDEFGHIKLMNOPQRSTUVWXYZ";

    map<char, pair<int, int>> keyMap;
    vector<vector<char>> keyGrid(5, vector<char>(5));

    int row = 0, col = 0;
    for (char ch : playfairKey) {
        if (keyMap.find(ch) == keyMap.end()) {
            keyMap[ch] = make_pair(row, col);
            keyGrid[row][col] = ch;
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }

    for (size_t i = 0; i < encrypted.length(); i += 2) {
        char ch1 = toupper(encrypted[i]);
        char ch2 = (i + 1 < encrypted.length()) ? toupper(encrypted[i + 1]) : 'X';

        if (ch1 == ch2) {
            ch2 = 'X';
            i--;
        }

        int row1 = keyMap[ch1].first;
        int col1 = keyMap[ch1].second;
        int row2 = keyMap[ch2].first;
        int col2 = keyMap[ch2].second;

        if (row1 == row2) {
            encrypted[i] = keyGrid[row1][(col1 + 1) % 5];
            encrypted[i + 1] = keyGrid[row2][(col2 + 1) % 5];
        } else if (col1 == col2) {
            encrypted[i] = keyGrid[(row1 + 1) % 5][col1];
            encrypted[i + 1] = keyGrid[(row2 + 1) % 5][col2];
        } else {
            encrypted[i] = keyGrid[row1][col2];
            encrypted[i + 1] = keyGrid[row2][col1];
        }
    }
    return encrypted;
}

string monoalphabeticEncrypt(const string &text, const string &key) {
    string encrypted = text;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    map<char, char> substitution;
    for (size_t i = 0; i < alphabet.length(); ++i) {
        substitution[alphabet[i]] = key[i];
    }

    for (char &ch : encrypted) {
        if (isalpha(ch)) {
            ch = substitution[toupper(ch)];
        }
    }
    return encrypted;
}

string hillDecrypt(const string &text, const string &key) {
    string decrypted = text;
    int keySize = static_cast<int>(sqrt(key.length()));

    vector<vector<int>> matrix(keySize, vector<int>(keySize));
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < keySize; ++j) {
            matrix[i][j] = key[i * keySize + j] - 'A';
        }
    }

    int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    int detInverse = 0;

    for (int i = 1; i < 26; ++i) {
        if ((det * i) % 26 == 1) {
            detInverse = i;
            break;
        }
    }

    while (det < 0) {
        det += 26;
    }

    while (det >= 26) {
        det -= 26;
    }

    for (size_t i = 0; i < decrypted.length(); i += keySize) {
        for (int j = 0; j < keySize; ++j) {
            int sum = 0;
            for (int k = 0; k < keySize; ++k) {
                sum += detInverse * matrix[j][k] * (decrypted[i + k] - 'A');
            }
            decrypted[i + j] = ((sum % 26) + 26) % 26 + 'A';
        }
    }
    return decrypted;
}

string playfairDecrypt(const string &text, const string &key) {
    string decrypted = text;
    string playfairKey = key + "ABCDEFGHIKLMNOPQRSTUVWXYZ";

    map<char, pair<int, int>> keyMap;
    vector<vector<char>> keyGrid(5, vector<char>(5));

    int row = 0, col = 0;
    for (char ch : playfairKey) {
        if (keyMap.find(ch) == keyMap.end()) {
            keyMap[ch] = make_pair(row, col);
            keyGrid[row][col] = ch;
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }

    for (size_t i = 0; i < decrypted.length(); i += 2) {
        char ch1 = toupper(decrypted[i]);
        char ch2 = (i + 1 < decrypted.length()) ? toupper(decrypted[i + 1]) : 'X';

        if (ch1 == ch2) {
            ch2 = 'X';
            i--;
        }

        int row1 = keyMap[ch1].first;
        int col1 = keyMap[ch1].second;
        int row2 = keyMap[ch2].first;
        int col2 = keyMap[ch2].second;

        if (row1 == row2) {
            decrypted[i] = keyGrid[row1][(col1 - 1 + 5) % 5];
            decrypted[i + 1] = keyGrid[row2][(col2 - 1 + 5) % 5];
        } else if (col1 == col2) {
            decrypted[i] = keyGrid[(row1 - 1 + 5) % 5][col1];
            decrypted[i + 1] = keyGrid[(row2 - 1 + 5) % 5][col2];
        } else {
            decrypted[i] = keyGrid[row1][col2];
            decrypted[i + 1] = keyGrid[row2][col1];
        }
    }
    return decrypted;
}

string monoalphabeticDecrypt(const string &text, const string &key) {
    string decrypted = text;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    map<char, char> substitution;
    for (size_t i = 0; i < key.length(); ++i) {
        substitution[key[i]] = alphabet[i];
    }

    for (char &ch : decrypted) {
        if (isalpha(ch)) {
            ch = substitution[toupper(ch)];
        }
    }
    return decrypted;
}

string caesarDecrypt(const string &text, int shift) {
    return caesarEncrypt(text, 26 - shift);
}

string keywordDecrypt(const string &text, const string &keyword) {
    string decrypted = text;
    string key = keyword;
    for (char &ch : key) {
        ch = toupper(ch);
    }

    int keyIndex = 0;
    for (char &ch : decrypted) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            char keyChar = key[keyIndex % key.length()];
            int shift = keyChar - 'A';
            ch = ((ch - base - shift + 26) % 26) + base;
            keyIndex++;
        }
    }
    return decrypted;
}

int main() {
    cout << "Cipher Tool\n";

    string inputText;
    cout << "Enter the text: ";
    getline(cin, inputText);

    char choice;
    cout << "Encode (E) or Decode (D)?: ";
    cin >> choice;

    string keyword;
    string key;

    if (toupper(choice) == 'E') {
        char methodChoice;
        cout << "Choose Cipher Method:\n";
        cout << "1. Caesar Cipher\n";
        cout << "2. Keyword Cipher\n";
        cout << "3. Hill Cipher\n";
        cout << "4. Playfair Cipher\n";
        cout << "5. Monoalphabetic Cipher\n";
        cout << "Enter choice (1/2/3/4/5): ";
        cin >> methodChoice;

        if (methodChoice == '1') {
            int shift;
            cout << "Enter shift value: ";
            cin >> shift;
            string encryptedText = caesarEncrypt(inputText, shift);
            cout << "Encrypted Text: " << encryptedText << "\n";
        } else if (methodChoice == '2') {
            cout << "Enter keyword: ";
            cin.ignore();
            getline(cin, keyword);
            string encryptedText = keywordEncrypt(inputText, keyword);
            cout << "Encrypted Text: " << encryptedText << "\n";
        } else if (methodChoice == '3') {
            cout << "Enter key matrix (as a string): ";
            cin >> key;
            string encryptedText = hillEncrypt(inputText, key);
            cout << "Encrypted Text: " << encryptedText << "\n";
        } else if (methodChoice == '4') {
            cout << "Enter keyword: ";
            cin.ignore();
            getline(cin, keyword);
            string encryptedText = playfairEncrypt(inputText, keyword);
            cout << "Encrypted Text: " << encryptedText << "\n";
        } else if (methodChoice == '5') {
            cout << "Enter substitution key (26 letters in desired order): ";
            cin.ignore();
            getline(cin, key);
            string encryptedText = monoalphabeticEncrypt(inputText, key);
            cout << "Encrypted Text: " << encryptedText << "\n";
        } else {
            cout << "Invalid choice\n";
        }
    }  else if (toupper(choice) == 'D') {
        char methodChoice;
        cout << "Choose Decryption Method:\n";
        cout << "1. Caesar Cipher\n";
        cout << "2. Keyword Cipher\n";
        cout << "3. Hill Cipher\n";
        cout << "4. Playfair Cipher\n";
        cout << "5. Monoalphabetic Cipher\n";
        cout << "Enter choice (1/2/3/4/5): ";
        cin >> methodChoice;

        if (methodChoice == '1') {
            int shift;
            cout << "Enter shift value: ";
            cin >> shift;
            string decryptedText = caesarDecrypt(inputText, shift);
            cout << "Decrypted Text: " << decryptedText << "\n";
        } else if (methodChoice == '2') {
            cout << "Enter keyword: ";
            cin.ignore();
            getline(cin, keyword);
            string decryptedText = keywordDecrypt(inputText, keyword);
            cout << "Decrypted Text: " << decryptedText << "\n";
        } else if (methodChoice == '3') {
            cout << "Enter key matrix (as a string): ";
            cin >> key;
            string decryptedText = hillDecrypt(inputText, key);
            cout << "Decrypted Text: " << decryptedText << "\n";
        } else if (methodChoice == '4') {
            cout << "Enter keyword: ";
            cin.ignore();
            getline(cin, keyword);
            string decryptedText = playfairDecrypt(inputText, keyword);
            cout << "Decrypted Text: " << decryptedText << "\n";
        } else if (methodChoice == '5') {
            cout << "Enter substitution key (26 letters in desired order): ";
            cin.ignore();
            getline(cin, key);
            string decryptedText = monoalphabeticDecrypt(inputText, key);
            cout << "Decrypted Text: " << decryptedText << "\n";
        } else {
            cout << "Invalid choice\n";
        }
    } else {
        cout << "Invalid choice\n";
    }

    return 0;
}
