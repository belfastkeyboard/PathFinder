/* C code produced by gperf version 3.1 */
/* Command-line: gperf -o -c -N valid_filetype -L C -k '*' whitelist.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "whitelist.gperf"

#include <string.h>

#define TOTAL_KEYWORDS 55
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 212
/* maximum key range = 212, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif

static unsigned int hash (register const char *str,
                          register size_t len)
{
  static unsigned char asso_values[] =
    {
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213,   0, 213, 213,  11,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213,   0, 213, 213,
        213, 213, 213, 213, 213, 213,   0,   0, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213,   0,  55,   5,
        25,    0,   5,  45,   0,   0,  25,  46,  10,  15,
          0,   0,  10, 213,  30,   0,  15,   0,  35,   0,
          5,  40,   1, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
        213, 213, 213, 213, 213, 213
    };

    register unsigned int hval = len;

    switch (hval)
    {
        default:
            hval += asso_values[(unsigned char)str[13]];
            /*FALLTHROUGH*/
        case 13:
            hval += asso_values[(unsigned char)str[12]];
            /*FALLTHROUGH*/
        case 12:
            hval += asso_values[(unsigned char)str[11]];
            /*FALLTHROUGH*/
        case 11:
            hval += asso_values[(unsigned char)str[10]];
            /*FALLTHROUGH*/
        case 10:
            hval += asso_values[(unsigned char)str[9]];
            /*FALLTHROUGH*/
        case 9:
            hval += asso_values[(unsigned char)str[8]];
            /*FALLTHROUGH*/
        case 8:
            hval += asso_values[(unsigned char)str[7]];
            /*FALLTHROUGH*/
        case 7:
            hval += asso_values[(unsigned char)str[6]];
            /*FALLTHROUGH*/
        case 6:
            hval += asso_values[(unsigned char)str[5]];
            /*FALLTHROUGH*/
        case 5:
            hval += asso_values[(unsigned char)str[4]];
            /*FALLTHROUGH*/
        case 4:
            hval += asso_values[(unsigned char)str[3]];
            /*FALLTHROUGH*/
        case 3:
            hval += asso_values[(unsigned char)str[2]];
            /*FALLTHROUGH*/
        case 2:
            hval += asso_values[(unsigned char)str[1]];
            /*FALLTHROUGH*/
        case 1:
            hval += asso_values[(unsigned char)str[0]];
            break;
    }

    return hval;
}

const char *valid_filetype(register const char *str,
                           register size_t len)
{
    static const char *wordlist[] =
    {
        "",
        "h",
        "sh",
        "ini",
        "zsh",
        "",
        "c",
        "cs",
        "css",
        "scss",
        "", "",
        "cc",
        "lua",
        "conf",
        "", "",
        "ts",
        "cxx",
        "unit",
        "", "",
        "pl",
        "hpp",
        "ps1",
        "swift",
        "",
        "js",
        "cpp",
        "json",
        "", "",
        "rs",
        "xml",
        "adoc",
        "", "", "",
        "txt",
        ".env",
        "", "",
        "md",
        "csv",
        "html",
        "", "",
        "go",
        "rst",
        "ksh",
        "", "",
        "py",
        "tsv",
        "", "", "", "",
        "log",
        "bash",
        "", "",
        "pom.xml",
        "kt",
        "java",
        "timer",
        "", "",
        "yml",
        "yaml",
        "", "", "",
        "bat",
        "dart",
        "", "",
        "service",
        "", "", "", "", "", "",
        "makefile",
        "", "",
        "rb",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "",
        "CMakeLists.txt",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "",
        "Cargo.toml",
        "", "", "", "", "", "",
        ".dockerfile",
        "", "", "", "", "",
        ".editorconfig",
        "", "", "", "",
        "package.json",
        "",
        ".gitignore",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "",
        "pyproject.toml",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "",
        "build.gradle"
    };

    if (len <= MAX_WORD_LENGTH &&
        len >= MIN_WORD_LENGTH)
    {
        register unsigned int key = hash(str,
                                         len);

        if (key <= MAX_HASH_VALUE)
        {
            register const char *s = wordlist[key];

            if (*str == *s &&
                !strncmp (str + 1, s + 1, len - 1) &&
                s[len] == '\0')
            {
                return s;
            }
        }
    }

    return NULL;
}
