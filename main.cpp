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
                txt = th09csv(buffer, flength);
                break;
            case 0x72303174:  //"t10r"
                txt = th10csv(buffer, flength);
                break;
            case 0x72313174:  //"t11r"
                txt = th11csv(buffer, flength);
                break;
            case 0x72323174:  //"t12r"
                txt = th12csv(buffer, flength);
                break;
            case 0x72383231:  //"128r"
                break;
            case 0x72333174:  //"t13r"
                {
                    if(flength > sizeof(th13_replay_header_t)) {
                        th13_replay_header_t *header = (th13_replay_header_t*)buffer;
                        if(flength > header->userdata_offset + 0x10) {
                            unsigned char id = buffer[header->userdata_offset + 0x10];
                            if(id == 144) {
                                txt = th13csv(buffer, flength);
                            } else {
                                txt = th14csv(buffer, flength);
                            }
                        } else {
                            // txt = nullptr;
                        }
                    } else {
                        // txt = nullptr;
                    }
                }
                break;
            case 0x72353174: 	//"t15r"
                txt = th15csv(buffer, flength);
                break;
            case 0x72363174: 	//"t16r"
                txt = th16csv(buffer, flength);
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

    if(success != args - 1) {
        printf("\nPress enter to continue ...");
        std::cin.get();
    }


    return 0;
}