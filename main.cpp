#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>       // std::isspace, std::isdigit
#include <algorithm>
#include <string>
#include <map>

using namespace std;

// see https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream &safeGetline(std::istream &is, std::string &t) {
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();

    for (;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if (sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if (t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char) c;
        }
    }
}


int main(int argc, char **argv) {
    if (argc != 3 || argv[1] != string("-w")) {
        cout << "You must add a dictionary as an input !" << endl;
        cout << "Usage : " << argv[0] << " -w [dictionary] " << endl;
        cout << argv << endl;
        return 1;
    }

    std::string path = argv[2];  // insert path to test file here

    std::ifstream ifs(path.c_str());
    if (!ifs) {
        std::cout << "Failed to open the file." << std::endl;
        return EXIT_FAILURE;
    }


    cout << "Using dictonary " << argv[2] << endl;

    std::multimap<string, string> dico;


    string t;
    string keyfied;
    string ordered;
    while (!safeGetline(ifs, t).eof()) {
        keyfied = t;
        std::transform(keyfied.begin(), keyfied.end(), keyfied.begin(), ::tolower);
        std::sort(keyfied.begin(), keyfied.end());
        dico.insert(std::make_pair(keyfied, t));
    }

    ifs.close();

    string in;
    cout << "Awaiting input" << endl;

    while (true) {
        cin >> in;

        keyfied = in;
        std::transform(keyfied.begin(), keyfied.end(), keyfied.begin(), ::tolower);
        std::sort(keyfied.begin(), keyfied.end());

        auto its = dico.equal_range(keyfied);
        for (auto it = its.first; it != its.second; ++it) {
            cout << it->second << endl;
        }

    }
    return 0;
}