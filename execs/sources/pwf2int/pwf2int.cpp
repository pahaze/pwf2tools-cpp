#include <dirent.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "int.h"
#include "types.h"
extern "C" {
    #include "lzss.h"
}
#include "pwf2cmd.h"
#include "pwf2common.h"

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(x, y) _mkdir(x)
#endif

#define INFO(s, ...) printf(s, __VA_ARGS__);
#define ERROR(s, ...) fprintf(stderr, s, __VA_ARGS__);
#define FATAL(ec, s, ...)          \
  fprintf(stderr, s, __VA_ARGS__); \
  exit(ec);

static int cmd_list(int argc, char *args[]);
static int cmd_extract(int argc, char *args[]);
static int cmd_create(int argc, char *args[]);

cmd_t commands[] = {
    {
        "list",
        "[intfile]",
        "Print the contents of an INT file.",
        {
            "l",
            "ls"
        },
        2,
        cmd_list
    },
    {
        "extract",
        "[intfile] [folder]",
        "Extract contents of an INT file to a folder.",
        {
            "e",
            "ex",
            "x"
        },
        3,
        cmd_extract
    },
    {
        "create",
        "[intfile] [folder]",
        "Create an INT file using the contents of a folder.",
        {
            "c",
            "make",
            "pack"
        },
        3,
        cmd_create
    }
};

#define assertheader(i, s, ...)                           \
  if (false == pwf2int::checkheader((const void *)(i))) { \
    fprintf(stderr, s, __VA_ARGS__);                      \
    return 1;                                             \
  }

bool direxists(const char *dirname) {
    struct stat st;
    if (stat(dirname, &st) != 0)
      return false;

    if (S_ISDIR(st.st_mode))
      return true;
    else
        return false;
}

bool makedir(const char *newdir) {
    struct stat st;
    if (direxists(newdir)) return true;
    int err = stat(newdir, &st);
    if (err == -1) {
        if (mkdir(newdir, S_IRWXU) != 0) {
            ERROR("Error making dir %s\n", newdir);
            return false;
        }
    } else if (0 == err) {
        if (S_ISDIR(st.st_mode))
            return true;
        else
            return false;
    }
    return true;
}

struct intfile_t {
    intfile_t(std::string fn, int filesize, int fdp);
    std::string filename;
    int filesize;
    int offset;
};

intfile_t::intfile_t(std::string fn, int filesize, int fdp): filename(fn), filesize(filesize), offset(fdp) {

}

struct intsection_offset_table_t {
    u32 hdr;
    u32 offsets;
    u32 filename_table;
    u32 characters;
    u32 lzss;
    u32 end;
};

#define ALIGN(x, y) (((x) + (y - 1)) & (~((y)-1)))

intsection_offset_table_t get_int_section_offsets(std::vector<intfile_t> &intfiles, int lzss_size) {
    intsection_offset_table_t r;
    r.hdr = 0;
    r.offsets = r.hdr + sizeof(pwf2int::header_t);
    r.filename_table = r.offsets + (sizeof(u32) * (intfiles.size() + 1));
    r.filename_table = ALIGN(r.filename_table, 0x10);
    r.characters = r.filename_table + (sizeof(pwf2int::filename_entry_t) * intfiles.size());
    size_t fnsize = 0;
    for (intfile_t &intfile : intfiles) {
        fnsize += intfile.filename.length() + 1;
    }
    r.lzss = ALIGN(r.characters + fnsize, 0x800);
    r.end = ALIGN((r.lzss + lzss_size) + sizeof(pwf2int::lzss_header_t), 0x800);
    return r;
}

size_t calc_int_section_size(std::vector<intfile_t> &intfiles, int lzss_size) {
  return get_int_section_offsets(intfiles, lzss_size).end;
}

void build_int_section(int restype, std::vector<intfile_t> &intfiles, const byte *lzss, int lzss_size, byte *out) {
    intsection_offset_table_t r = get_int_section_offsets(intfiles, lzss_size);                  // get offsets for the sections
    pwf2int::header_t *hdr = (pwf2int::header_t *)(out + r.hdr);                                 // establish a header
    u32 *offsets = (u32 *)(out + r.offsets);                                                     // pointer of u32s for offsets
    pwf2int::filename_entry_t *fentries = (pwf2int::filename_entry_t*)(out + r.filename_table);  // pointer of filenames entries
    char *characters = (char *)(out + r.characters);                                             // what
    pwf2int::lzss_header_t *lzss_sec = (pwf2int::lzss_header_t*)(out + r.lzss);                  // get the lzss header for the section

    memset(out, 0, r.end);                                // set out to be all 0s

    hdr->unk[0] = 0;                                      // what
    hdr->unk[1] = 0;                                      // what
    hdr->lzss_section_size = r.end - r.lzss;              // set the section size
    hdr->resourcetype = restype;                          // set resource type
    hdr->fntablesizeinbytes = r.lzss - r.filename_table;  // get the table size
    hdr->magic = INTHDR_MAGIC;                            // 0x44332211 as usual
    hdr->fntableoffset = r.filename_table;                // get offset from table
    hdr->filecount = intfiles.size();                     // get # of files

    u32 offset = 0;
    u32 fnoffs = 0;                                       // set offset and fnoffs to 0
    for (u32 i = 0; i < intfiles.size(); i += 1) {        // for each file..
        intfile_t &intfile = intfiles[i];                 // get pointer to intfile
        offsets[i] = offset;                              // set the offset
        fentries[i].offset = fnoffs;                      // set the filename offset
        fentries[i].sizeof_file = intfile.filesize;       // set the size of the file
        size_t fnlen = intfile.filename.length() + 1;     // set length of filename + 1 null terminator
        const char *cstr = intfile.filename.c_str();      // get cstring of filename
        memcpy(characters + fnoffs, cstr, fnlen);         // copy the cstring to characters + file

        offset += ALIGN(intfile.filesize, 0x10);          // add to the offset using align
        fnoffs += fnlen;                                  // add to the filename offsets with fnlen
    }
    offsets[intfiles.size()] = offset;                    // set the new offset

    lzss_sec->uncompressed_size = offset;                 // uncompressed size is offset
    lzss_sec->compressed_size = lzss_size;                // compressed size is the lzss size
    memcpy(lzss_sec->data, lzss, lzss_size);              // copy lzss data to header data

    return;
}

bool isfile(const char *fn) {
    struct stat st;
    if (stat(fn, &st) != 0)
        return false;

    if (S_ISREG(st.st_mode))
        return true;
    else
        return false;
}

struct resfile_iterator_t {
    virtual ~resfile_iterator_t() {

    };
    virtual const char *next() = 0;
};

struct resfile_dir_iterator_t: resfile_iterator_t {
    DIR *dir;
    struct dirent *de;
    resfile_dir_iterator_t(DIR *_dir) : dir(_dir){};
    ~resfile_dir_iterator_t() { closedir(dir); }
    const char *next();
};

const char *resfile_dir_iterator_t::next() {
    de = readdir(dir);
    if (de == NULL) return NULL;
    return de->d_name;
}

struct resfile_order_iterator_t: resfile_iterator_t {
    FILE *orderfile;
    char buf[256];
    resfile_order_iterator_t(FILE *f): orderfile(f) {

    }
    ~resfile_order_iterator_t() {
        this->close();
    }
    const char *next();
    void close() {
        if (NULL != orderfile) {
            fclose(orderfile);
            orderfile = NULL;
        }
        return;
    }
};

const char *resfile_order_iterator_t::next() {
    buf[0] = char(0);
    while (!feof(orderfile)) {
        fscanf(orderfile, "%512s\n", buf);
        if (buf[0] == char(0))
            continue;
        return buf;
    }
    return NULL;
}

resfile_iterator_t *openiterator(const char *dirname) {
    char lbuf[256];
    snprintf(lbuf, sizeof(lbuf), "%s/_order.txt", dirname);
    if (isfile(lbuf)) {
      FILE *orderfile = fopen(lbuf, "r");
      if (NULL != orderfile)
        return new resfile_order_iterator_t(orderfile);
    }
    DIR *dir = opendir(dirname);
    if (NULL == dir)
      return NULL;

    return new resfile_dir_iterator_t(dir);
}

int pad_folderdata(byte *folderdata, int start, int end) {
    int remain = end - start;
    while (remain > 0) {
        // TODO: Repeating last few bytes of file might help compression
        //       instead of zero-filling. The official INT packer
        //       zero-fills.
        folderdata[end - remain] = 0;
        remain--;
    }
    return (end - start);
}

#define REQUIRE(x, c)              \
    if (argc < x) {                \
        commands[c].printhelp(""); \
        return 1;                  \
    }

static int cmd_list(int argc, char *args[]) {
    REQUIRE(1, 0);  // right command?

    const char *intfilename = args[0];  // get filename

    FILE *f = fopen(intfilename, "rb");  // open it
    if (NULL == f) {                     // cant?
        ERROR("Could not open INT file %s\n", intfilename);
        return 1;
    }
    int len = getfilesize(f);
    void *mint = malloc(len);
    fread(mint, 1, len, f);
    fclose(f);

    assertheader(mint, "Not an INT file %s\n", intfilename);                                    // not right header? gotta go . ,. ,

    pwf2int::header_t *hdr = pwf2int::getheader(mint);                                          // get the header data

    while (hdr->resourcetype != INT_RESOURCE_END) {                                             // check all sections
        pwf2int::filename_entry_t *entries =
            pwf2int::getfilenameentries(hdr);                                                   // get the filenames
        char *characters = pwf2int::getfilenames(hdr);                                          // get some more
        for (u32 i = 0; i < hdr->filecount; i += 1) {                                           // for each file
            printf("%s - %d bytes\n", characters + entries[i].offset, entries[i].sizeof_file);  // list name - size
        }
        hdr = pwf2int::getnextheader(hdr);                                                      // iterate to next header
    }
    free(mint);
    printf("Done.\n");
    return 0;
}

static int cmd_extract(int argc, char *args[]) {
    REQUIRE(2, 1);                                            // make sure we're running the right command
    const char *intfilename = args[0];                        // get filename
    const char *outputfoldername = args[1];                   // get output folder name
    FILE *f = fopen(intfilename, "rb");                       // open file
    int len = getfilesize(f);                                 // get size
    void *mint = malloc(len);                                 // allocate file in memory
    fread(mint, 1, len, f);                                   // put file in there

    assertheader(mint, "Not an INT file %s\n", intfilename);  // if not right header return

    INFO("INFLATE %s\n", intfilename);
    pwf2int::header_t *hdr = pwf2int::getheader(mint);
    byte *history = (byte *)(malloc(4096));                   // allow 4096 bytes for history

    while (hdr->resourcetype != INT_RESOURCE_END) {
        const char *restypename = pwf2int::typenames[hdr->resourcetype];                                                  // get the type
        printf("INFLATE: Section %s\n", restypename);
        pwf2int::lzss_header_t *lzss = pwf2int::getlzssheader(hdr);                                                       // get lzzs header
        byte *uncompressed = (byte *)(malloc(lzss->uncompressed_size));                                                   // get the uncompressed size from header
        byte *compressed = lzss->data;                                                                                    // get the data
        memset(history, 0, 4096);                                                                                         // set history to 0s
        lzss_decompress(12, 4, 2, 2, history, compressed, lzss->compressed_size, uncompressed, lzss->uncompressed_size);  // uncompress into uncompressed size
        pwf2int::filename_entry_t *entries = pwf2int::getfilenameentries(hdr);                                            // get names
        u32 *fileoffsets = pwf2int::getfiledataoffsets(hdr);                                                              // get offsets
        char *fnchars = pwf2int::getfilenames(hdr);                                                                       // get chars
        char lbuf[256];                                                                                                   // establish char buffer

        const char *dirname = outputfoldername;                                                                           // get output dir
        makedir(dirname);                                                                                                 // make if doesnt exist

        printf("Extracting %d files (TYPE: %s)\n", hdr->filecount, restypename);                                          // extractin # files (type: resource type)

        snprintf(lbuf, sizeof(lbuf), "%s/%s", dirname, restypename);                                                      // set lbuf to dirname/resource type
        makedir(lbuf);                                                                                                    // make lbuf if it doesnt exist

        snprintf(lbuf, sizeof(lbuf), "%s/%s/_order.txt", dirname, restypename);                                           // set lbuf to include _order
        FILE *orderfile = fopen(lbuf, "w");                                                                               // open the orderfile in write mode
        for (u32 i = 0; i < hdr->filecount; i += 1) {                                                                     // iterate over every file
            pwf2int::filename_entry_t &entry = entries[i];                                                                // get pointer for entry
            const char *filename = fnchars + entry.offset;                                                                // get filename from the offset
            snprintf(lbuf, sizeof(lbuf), "%s/%s/%s", dirname, restypename, filename);                                     // set buf to dir/resource/filename

            printf("WRITE: %s (%d bytes)\n", filename, entry.sizeof_file);                                                // write: filename (file size)
            FILE *outfile = fopen(lbuf, "wb");                                                                            // open out file in writebyte
            if (NULL == outfile) {                                                                                        // if it cant, move on
                INFO("Couldn't open %s...\n", lbuf);
                continue;
            }
            fwrite(uncompressed + fileoffsets[i], 1, entry.sizeof_file, outfile);                                         // write the uncompressed + fileoffsets from memory into outfile
            fclose(outfile);                                                                                              // close outfile
            fprintf(orderfile, "%s\n", filename);                                                                         // in order, print filename
        }
        fclose(orderfile);                                                                                                // close order
        hdr = pwf2int::getnextheader(hdr);                                                                                // move to next section
        free(uncompressed);                                                                                               // free uncompressed data
    }
    free(history);  // free history
    printf("Done.\n");
    return 0;
}

static int cmd_create(int argc, char *args[]) {
    REQUIRE(2, 2);                    // make sure we in create
    char lbuf[256];                   // buffer
    FILE *outfile;                    // file to output to
    const char *intname = args[0];    // name of int
    const char *dirname = args[1];    // name of directory
    std::vector<intfile_t> intfiles;  // vector of int files
    outfile = fopen(intname, "wb");   // open in write-byte format
    if (NULL == outfile) {
        // no ability ??
        ERROR("CREATE: Could not open target %s\n", intname);  // gotta go ., .,., .
        return 1;
    }
    if (!direxists(dirname)) {
        // if the dir doesnt exist
        ERROR("CREATE: Could not find directory %s\n", dirname);
        return 1;
    }

    byte *history = (byte *)(malloc(4096 * 2));                       // For lzss compression
    for (int i = 1; i < INT_RESOURCE_TYPE_COUNT; i += 1) {            // resource type count is 8
        const char *restypename = pwf2int::typenames[i];              // what name do we use? pick from the array
        snprintf(lbuf, sizeof(lbuf), "%s/%s", dirname, restypename);  // make lbuf equal dir/type
        printf("Checking for %s... ", restypename);
        if (!direxists(lbuf)) {
            // if it dont exist, move on
            printf("no\n");
            continue;
        } else {
            // good news it exists
            printf("yes\n");
            intfiles.clear();                               // clear vectorr

            resfile_iterator_t *iter = openiterator(lbuf);  // try to open
            int folderlen = 0;
            if (iter == NULL) {
                // darn, we cant
                fprintf(stderr, "But I couldn't open it :(\n");
                continue;
            }

            byte *folderdata = (byte *)(malloc(4));                                        // allow 4 bytes gfor folder data
            const char *filename;                                                          // char pointer for file name
            while ((filename = iter->next()) != NULL) {
                snprintf(lbuf, sizeof(lbuf), "%s/%s/%s", dirname, restypename, filename);  // set lbuf to dirname/resource type/filename
                printf("CHECK: %s\n", lbuf);  // does it exist?
                if (!isfile(lbuf)) {
                    // nope, move on
                    continue;
                }
                printf("ADD: %s\n", filename);
                FILE *f = fopen(lbuf, "rb");  // open with read
                if (NULL == f) {
                    // oops, its not there
                    fprintf(stderr, "   Could not open %s\n", lbuf);
                    continue;  // move on
                }
                int len = getfilesize(f);                                                    // get length of file
                int fdp = folderlen;                                                         // hold folderlen for now
                folderlen = ALIGN(folderlen + len, 0x10);                                    // align folderlen and len with 0x10
                folderdata = (byte *)(realloc(folderdata, folderlen));                       // extend folder data
                fread(folderdata + fdp, 1, len, f);                                          // export file into folderdata+fdp bytes
                pad_folderdata(folderdata, fdp + len, folderlen);                            // 0fill folderdata
                fclose(f);                                                                   // close the file
                intfiles.push_back(intfile_t(std::string(filename), len, fdp));              // add the new intfile to vector iterate for next resource
            }
            delete iter;                                                                     // get rid of iterator, we dont want any memory leaks

            printf("COMPRESS: Input %d bytes\n", folderlen);
            memset(history, 0, 4096 * 2);                                                    // fill history with 0s
            int complen = lzss_compress(12, 4, 2, 2, history, folderdata, folderlen, NULL);  // compress the folderddata, get the length

            printf("COMPRESS: Output %d bytes\n", complen);                                  // output compressed bytes
            memset(history, 0, 4096 * 2);                                                    // fill history with 0 again
            byte *lzss_data = (byte *)(malloc(complen));                                     // allocate complen to bytes
            lzss_compress(12, 4, 2, 2, history, folderdata, folderlen, lzss_data);           // compress again, but now output lzzs data
            free(folderdata);                                                                // free the data

            printf("Building %s section\n", restypename);
            int seclen = calc_int_section_size(intfiles, complen);                           // calculate the size for the int ssection

            printf("  %d bytes\n", seclen);
            byte *secdata = (byte *)(malloc(seclen));                                        // allocate that amount
            build_int_section(i, intfiles, lzss_data, complen, secdata);                     // build it

            printf("Writing %d bytes\n", seclen);
            fwrite(secdata, 1, seclen, outfile);                                             // write the sectiondata to the file

            free(secdata);                                                                   // free the sectiondata
        }
    }

    printf("Finalizing\n");
    fwrite(&pwf2int::nullhdr, sizeof(pwf2int::nullhdr), 1, outfile);  // write nullhdr to file?

    fclose(outfile);  // close the file
    printf("Done.\n");
    free(history);  // free history
    return 0;       // and we're done
}

int main(int argc, char *args[]) {
    if (pwf2int::checkinstall() == false) {
        // if it didnt compile well, will barely show
        fprintf(stderr, "Bad compile\n");
        return 2;
    }

    if (argc <= 1) {
        // if no arguments
        for (cmd_t &cmd : commands) {
            cmd.printhelp("");
        }
        return 1;
    } else {
        for (cmd_t &cmd : commands) {    // find matching command
            if (cmd.matches(args[1])) {  // epic GAMER STYLE we found a match
                return cmd.exec(argc - 2, args + 2);
            }
        }
        printf("pwf2int:   Unknown command %s\n", args[1]);
        return 1;
    }
}
