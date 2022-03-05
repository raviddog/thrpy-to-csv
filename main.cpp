#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <filesystem>

#include "zuntypes.h"
#include "thcsv.h"

int main(int args, char *argv[]) {
    if(args < 2) {
        printf("Usage: %s [filename]", argv[0]);
        return 0;
    }

    std::filesystem::path dir(argv[0]);
    dir = dir.replace_filename("data.csv");

    FILE *out = fopen(dir.string().c_str(), "a");
    if(!out) {
        printf("Error opening output file data.csv");
        return 0;
    }
    

    int success = 0;


    for(int i = 1; i < args; i++) {
        char * file = argv[i];
        FILE * fp;
        fp = fopen(file, "rb");
        if (!fp) {
            printf("file %s not found.\n", file);
            continue;
        }

        unsigned int flength;
        fseek(fp, 0L, SEEK_END);
        flength = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        unsigned char *buffer = (unsigned char*)malloc(flength);
        fread(buffer, flength, 1, fp);
        fclose(fp);

        int magic = *(int*) buffer;

        std::string txt = "\n";
        
        switch(magic) {
            case 0x50523654:  //"T6RP"
                txt = th06csv(buffer, flength);
                break;
            case 0x50523754:  //"T7RP"
                txt = th07csv(buffer, flength);
                break;
            case 0x50523854:  //"T8RP"
                txt = th08csv(buffer, flength);
                break;
            case 0x50523954: 	//"T9RP"
                // out = th09csv(buf, flength);
                break;
            case 0x72303174:  //"t10r"
                // out = th10csv(buf, flength);
                break;
            case 0x72313174:  //"t11r"
                // out = th11csv(buf, flength);
                break;
            case 0x72323174:  //"t12r"
                // out = th12csv(buf, flength);
                break;
            case 0x72383231:  //"128r"
                break;
            case 0x72333174:  //"t13r"
                {
                    if(flength > sizeof(th13_replay_header_t)) {
                        th13_replay_header_t *header = (th13_replay_header_t*)buffer;
                        if(flength > header->userdata_offset + 0x10) {
                            const char id = buffer[header->userdata_offset + 0x10];
                            if(id == 144) {
                                // out = th13csv(buf, flength);
                            } else {
                                // out = th14csv(buf, flength);
                            }
                        } else {
                            // out = nullptr;
                        }
                    } else {
                        // out = nullptr;
                    }
                }
                break;
            case 0x72353174: 	//"t15r"
                // out = th15csv(buf, flength);
                break;
            case 0x72363174: 	//"t16r"
                // out = th16csv(buf, flength);
                break;
            case 0x72373174: 	//"t17r"
                break;
            case 0x72383174: 	//"t18r"
                break;
            default:
                break;
        }

        if(txt != "\n") {
            printf("file: %s\n", argv[i]);
            fwrite(txt.c_str(), txt.length(), 1, out);
            // printf("%s\n", txt.c_str());
            success++;
        }
    }

    //  print runtime stats to console
    printf("\nProcessed: %d / %d", success, args - 1);

    fflush(out);
    fclose(out);

    printf("\nPress enter to continue ...");
    std::cin.get();

    return 0;
}