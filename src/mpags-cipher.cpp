#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


bool processCommandLine(
    const std::vector<std::string>& args,
    std::string& inputFileName,
    std::string& outputFileName,
    bool& helpRequested,
    bool& versionRequested )
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

    // Process command line arguments - ignore zeroth element, as we know this
    // to be the program name and don't need to worry about it
    for (std::size_t i{1}; i < nArgs; ++i) {
        if (args[i] == "-h" || args[i] == "--help") {
            helpRequested = true;
        } else if (args[i] == "--version") {
            versionRequested = true;
        } else if (args[i] == "-i") {
            // Handle input file option
            // Next element is filename unless "-i" is the last argument
            if (i == nArgs - 1) {
                std::cerr << "[error] -i requires a filename argument"
                          << std::endl;
                // exit main with non-zero return to indicate failure
                return 1;
            } else {
                // Got filename, so assign value and advance past it
                inputFileName = args[i + 1];
                ++i;
            }
        } else if (args[i] == "-o") {
            // Handle output file option
            // Next element is filename unless "-o" is the last argument
            if (i == nArgs - 1) {
                std::cerr << "[error] -o requires a filename argument"
                          << std::endl;
                // exit main with non-zero return to indicate failure
                return true;
            } else {
                // Got filename, so assign value and advance past it
                outputFileName = args[i + 1];
                ++i;
            }
        } else {
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
            << std::endl;
    }

    // Handle version, if requested
    if (versionRequested) {
        std::cout << "0.1.0" << std::endl;
    }

    return false;
}

std::string transformChar (const char in_char)
{
    /* Transliterate input character to uppercase form

    const char in_char: Input character to be transliterated

    return: String containing only upper case letters based on in_char
    */

    std::string out_str;
    // Uppercase alphabetic characters
    if (std::isalpha(in_char)) {
        out_str += std::toupper(in_char);
        return out_str;
    }

    // Transliterate digits to English words
    switch (in_char) {
        case '0':
            out_str += "ZERO";
            return out_str;
        case '1':
            out_str += "ONE";
            return out_str;
        case '2':
            out_str += "TWO";
            return out_str;
        case '3':
            out_str += "THREE";
            return out_str;
        case '4':
            out_str += "FOUR";
            return out_str;
        case '5':
            out_str += "FIVE";
            return out_str;
        case '6':
            out_str += "SIX";
            return out_str;
        case '7':
            out_str += "SEVEN";
            return out_str;
        case '8':
            out_str += "EIGHT";
            return out_str;
        case '9':
            out_str += "NINE";
            return out_str;
    }

    // If the character isn't alphabetic or numeric, DONT add it
    return out_str;
}

unsigned short int getInputText (const std::string& fileName, std::string& inputText)
{
    // Initialise variables
    char inputChar{'x'};

    // Read in user input from stdin/file
    // Warn that input file option not yet implemented
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
        // loop over each character from user input
        while (std::cin >> inputChar) {
            inputText += transformChar (inputChar);
        }
    }
    return 0;
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

    // Exit with error is processCommandLine returns and error (true)
    // ? Bad practice to call a functionn inside and if, or worth it to save deinining another variable?
    if (processCommandLine(
        cmdLineArgs,
        inputFile,
        outputFile,
        helpRequested,
        versionRequested
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
    unsigned short int inputError { getInputText(inputFile, inputText) };
    if (inputError != 0)
    {
        return inputError;
    }

    // Print out the transliterated text

    // Warn that output file option not yet implemented
    if (!outputFile.empty()) {
        std::cerr << "[warning] output to file ('" << outputFile
                  << "') not implemented yet, using stdout\n";
    }

    std::cout << inputText << std::endl;

    // No requirement to return from main, but we do so for clarity
    // and for consistency with other functions
    return 0;
}