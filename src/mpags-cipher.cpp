#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Project headers
#include "TransformChar.hpp"

bool processCommandLine(
    const std::vector<std::string>& args,
    std::string& inputFileName,
    std::string& outputFileName,
    bool& overwriteOutput,
    bool& helpRequested,
    bool& versionRequested,
    bool& encrypt,
    size_t& key
    )
{
    /* Processes command line arguments and extracts relevant information

    const vector& args: vector containing all command line arguments
    const size_t nArgs: number of elements in args
    string& inputFileName: Supplied path to an input file, if given
    string& outputFileName: Supplied path to an output file, if given
    bool& helpRequested: Set to true if help was requested
    bool& helpRequested: Set to true if te version number was requested

    return bool: Returns true if there was an error in processing the arguments
    */

    const std::size_t nArgs{args.size()};
    // Flag whether encryption has aldready been set
    bool encryptionSet{false};

    // Process command line arguments - ignore zeroth element, as we know this
    // to be the program name and don't need to worry about it
    for (std::size_t i{1}; i < nArgs; ++i) {

        // Help/version args
        if (args[i] == "-h" || args[i] == "--help") {
            helpRequested = true;
        } else if (args[i] == "--version") {
            versionRequested = true;
        }
        
        // Input/out file args
        else if (args[i] == "-i") {
            // Handle input file option
            // Next element is filename unless "-i" is the last argument
            if (i == nArgs - 1) {
                std::cerr << "[error] -i requires a filename argument"
                          << std::endl;
                // exit function with return true to indicate failure
                return 1;
            } else {
                // Got filename, so assign value and advance past it
                inputFileName = args[i + 1];
                ++i;
            }
        } else if (args[i] == "-o" || args[i] == "-ow" || args[i] == "-wo") {
            // If called with w, set overwriteOutput to true
            if (args[i] == "-ow" || args[i] == "-wo") {
                overwriteOutput = true;
            }
            // Handle output file option
            // Next element is filename unless "-o" is the last argument
            if (i == nArgs - 1) {
                std::cerr << "[error] -o requires a filename argument"
                          << std::endl;
                // exit function with return true to indicate failure
                return true;
            } else {
                // Got filename, so assign value and advance past it
                outputFileName = args[i + 1];
                ++i;
            }
        } else if (args[i] == "-w") {
            overwriteOutput = true;
        }
        
        // Cipher arguments
        else if (args[i] == "-e") {
            if (encryptionSet) {
                std::cerr << "[error] -e cannot be used with -d" << std::endl;
                // exit function with return true to indicate failure
                return true;
            }
            encryptionSet = true;
        } else if (args[i] == "-d") {
            if (encryptionSet) {
                std::cerr << "[error] -d cannot be used with -e" << std::endl;
                // exit function with return true to indicate failure
                return true;
            }
            encrypt = false;
            encryptionSet = true;
        } else if (args[i] == "-k") {
            // Handle key setting
            // Next element is key value unless "-k" is the last argument
            if (i == nArgs - 1) {
                std::cerr << "[error] -k requires a shift argument" << std::endl;
                // exit function with return true to indicate failure
                return true;
            } else {
                // Got key, so assign value
                try {
                    // Want to proof against negative numbers
                    int keyVal {stoi(args[i + 1])};
                    key =  keyVal % 26 + 26 * (keyVal<0);
                }
                // Check whether stoi threw an exception => non-int input
                catch (...) {
                    std::cerr << "[error] -k requires an integer argument" << std::endl;
                    // exit function with return true to indicate failure
                    return true;
                }
                ++i;
            }
        }

        // Error case
        else {
            // Have an unknown flag to output error message and return non-zero
            // exit status to indicate failure
            std::cerr << "[error] unknown argument '" << args[i]
                      << "'\n";
            return true;
        }
    }

    // Handle help, if requested
    if (helpRequested) {
        // Line splitting for readability
        std::cout
            << "Usage: mpags-cipher [-h/--help] [--version] [-i <file>] [-o <file>]\n\n"
            << "Encrypts/Decrypts input alphanumeric text using classical ciphers\n\n"
            << "Available options:\n\n"
            << "  -h|--help        Print this help message and exit\n\n"
            << "  --version        Print version information\n\n"
            << "  -i FILE          Read text to be processed from FILE\n"
            << "                   Stdin will be used if not supplied\n\n"
            << "  -o FILE          Write processed text to FILE\n"
            << "                   Stdout will be used if not supplied\n\n"
            << "  -w               If passed with -o, overwrites the contents\n"
            << "                     of the output file, instead of appending.\n\n"
            << "  -e               Sets the cipher to encrypt (mutually\n"
            << "                     exclusive with -d)\n\n"
            << "  -d               Sets the cipher to decrypt (mutually\n"
            << "                     exclusive with -e)\n\n"
            << "  -k INTEGER       Shift value to use for the caesar cipher\n"
            << "                   Should be in range 0-25\n\n"
            << std::endl;
    }

    // Handle version, if requested
    if (versionRequested) {
        std::cout << "0.1.0" << std::endl;
    }

    if (overwriteOutput && outputFileName.empty()){
        std::cerr << "[error] -w requires output file to be defined using -o" << std::endl;
        return true;
    }

    return false;
}

int getInputText (const std::string& fileName, std::string& inputText)
{
    /* Gets the input text from a specified file and saves it to a string variable

    const string& fileName: Path to file to be read (if empty gets interactive input)
    string& inputText: String to which the file contents are saved to.  

    return int: Error code, 0 is success, 1 is failure
    */

    // Initialise variables
    char inputChar{'x'};

    // Read in user input from stdin/file
    if (!fileName.empty()) {
        std::ifstream inputData {fileName};
        if (!inputData.good()) {
            std::cerr << "[error] problem reading file '" << fileName
                    << "', please confirm the path\n";
            return 1;
        }
        while (inputData >> inputChar) {
            inputText += transformChar (inputChar);
        }
    }
    else
    {
        std::cout << "Please enter the message then press 'Enter' followed by 'ctrl+D':" << std::endl;
        // loop over each character from user input
        while (std::cin >> inputChar) {
            inputText += transformChar (inputChar);
        }
    }
    return 0;
}

int printOutput (const std::string& fileName, const std::string& outputText, const bool overwrite = false)
{
    /* Prints text to a specified file or console

    const string& fileName: Path to file to be printed to (if empty prints to console)
    string& outputText: String to be printed

    return int: Error code, 0 is success, 1 is failure
    */

    // Open and write to output file if given
    if (!fileName.empty()) {
        std::ofstream outFile;
        // Make the file overwrite if overwrite is set to true
        if (overwrite) {
            outFile.open(fileName);
        } else {
            outFile.open(fileName, std::ios::app);
        }

        if (!outFile.good()) {
            std::cerr << "[error] problem opening file '" << fileName << "'\n";
            return 1;
        }
        outFile << outputText << "\n";
    }
    // Else print out the text
    else
    {
        std::cout << outputText << std::endl;
    }
    return 0;
}

std::string runCaesarCipher( const std::string& inputText, const size_t key, const bool encrypt)
{
    /* Encrypt/decrypt a caesar cipher using the input text with specified key

    const string& inputText: String to be encrypted/decrypted
    const size_t key: integer in range 0-25 specifying the shift
    const bool encrypt: true when encrypting, false when decrypting

    return string: encrypted/decrypted string
    */

    // Initialise variables
    std::string outputText;
    char cipherChar{'x'};

    // Apply the cipher character by character
    for (const char &c: inputText) {
        // (encrypt * 2 - 1) maps boolean 1/0 to 1/-1
        cipherChar = c + key * (encrypt * 2 - 1);
    
        // Wrap character back to upper case if necessary
        cipherChar -= 26 * (encrypt * 2 - 1) * (!isupper(cipherChar));
        
        outputText += cipherChar;
    }

    return outputText;
}


int main(int argc, char* argv[])
{
    // Convert the command-line arguments into a more easily usable form
    const std::vector<std::string> cmdLineArgs{argv, argv + argc};

    // Options that might be set by the command-line arguments
    bool helpRequested{false};
    bool versionRequested{false};
    std::string inputFile{""};
    std::string outputFile{""};
    bool overwriteOutput{false};
    bool encrypt{true};
    size_t key{0};

    // Exit with error is processCommandLine returns and error (true)
    // ? Bad practice to call a functionn inside and if, or worth it to save deinining another variable?
    if (processCommandLine(
        cmdLineArgs,
        inputFile,
        outputFile,
        overwriteOutput,
        helpRequested,
        versionRequested,
        encrypt,
        key
    ) == true)
    {
        return 1;
    }

    // If help/version number was requested, exit the program now
    // Text was already printed out in processCommandLine to save space here
    // ? Would it be better to keep the text and return together, or OK to save space?
    if (helpRequested || versionRequested) {
        return 0;
    }

    // Get the input text
    std::string inputText;
    int inputError { getInputText(inputFile, inputText) };
    if (inputError != 0)
    {
        return inputError;
    }

    const std::string outputText {runCaesarCipher(inputText, key, encrypt)};

    // Save/print the text
    int outputError { printOutput(outputFile, outputText, overwriteOutput) };
    if (outputError != 0)
    {
        return outputError;
    }

    // No requirement to return from main, but we do so for clarity
    // and for consistency with other functions
    return 0;
}