#include "thdecode.h"
#include "zuntypes.h"

#include <sstream>
#include <string>
#include <cstring>

std::string th06csv(unsigned char *buffer, int flength) {
    std::string out = "th06,";
    
	if(flength < sizeof(th06_replay_header_t)) return "\n";
	th06_replay_header_t *header = (th06_replay_header_t *)buffer;



	//	now decode the replay
    unsigned char **buf = (unsigned char**)&buffer;
	flength = th06decode(buf, flength);
	buffer = *buf;

	if(flength < sizeof(th06_replay_t)) return "\n";
	th06_replay_t *rep = (th06_replay_t*)&buffer[0x10];


	//	name, null terminated string
	//	ensure that its null terminated
	if(rep->name[8] != '\0') rep->name[8] = '\0';
	out += rep->name;
    out += ",";
    switch(header->shot) {
        case 0:
            out += "ReimuA,";
            break;
        case 1:
            out += "ReimuB,";
            break;
        case 2:
            out += "MarisaA,";
            break;
        case 3:
            out += "MarisaB,";
            break;
        default:
            out += "Unknown,";
            break;
    }
	switch(header->difficulty) {
        case 0:
            out += "Easy,";
            break;
        case 1:
            out += "Normal,";
            break;
        case 2:
            out += "Hard,";
            break;
        case 3:
            out += "Lunatic,";
            break;
        case 4:
            out += "Extra,";
            break;
        default:
            out += "Unknown,";
            break;
    }
    char score[16];
    sprintf(score, "%u\0", rep->score);
    out += score;
    out += ",";

    th06_replay_stage_t splits[7];

	for(int i = 0; i < 7; i++) {
		if(rep->stage_offsets[i] != 0x00 && rep->stage_offsets[i] + sizeof(th06_replay_stage_t) < flength) {
			th06_replay_stage_t *stage = (th06_replay_stage_t*)&buffer[rep->stage_offsets[i]];
			memcpy(splits + i, stage, sizeof(th06_replay_stage_t));
		}
	}

    for(int i = 0; i < 7; i++) {
        if(rep->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].score);
            out += score;
        }
        out += ",";
    }
    out += ",,";   //  score 8-9
    out += ",,,,,,,";   // piv 1-7
    out += ",,,,,,,";   // graze 1-7
    out += ",,,,,,,";   // other 1-7
    out += "\n";
    free(buffer);
    return out;
}

std::string th07csv(unsigned char *buffer, int flength) {
    std::string out = "th07,";

    if(flength < sizeof(th07_replay_header_t)) return "\n";

    unsigned char **buf = &buffer;
    flength = th07decode1(buf, flength);
	buffer = *buf;

    th07_replay_header_t *header = (th07_replay_header_t*)malloc(sizeof(th07_replay_header_t));
	memcpy(header, buffer, sizeof(th07_replay_header_t));
	for(int i = 0; i < 7; i++) {
		if(header->stage_offsets[i] != 0x00) {
			header->stage_offsets[i] -= 84;
		}
	}

    flength = th07decode2(buf, flength);
	buffer = *buf;

    if(flength < sizeof(th07_replay_t)) {
		free(header);
		return "\n";
	}
    th07_replay_t *rep = (th07_replay_t*)buffer;

    out += rep->name;
    out += ",";
    switch(rep->shot) {
        case 0:
            out += "ReimuA,";
            break;
        case 1:
            out += "ReimuB,";
            break;
        case 2:
            out += "MarisaA,";
            break;
        case 3:
            out += "MarisaB,";
            break;
        case 4:
            out += "SakuyaA,";
            break;
        case 5:
            out += "SakuyaB,";
            break;
        default:
            out += "Unknown,";
            break;
    }
    switch(rep->difficulty) {
        case 0:
            out += "Easy,";
            break;
        case 1:
            out += "Normal,";
            break;
        case 2:
            out += "Hard,";
            break;
        case 3:
            out += "Lunatic,";
            break;
        case 4:
            out += "Extra,";
            break;
        case 5:
            out += "Phantasm,";
            break;
        default:
            out += "Unknown,";
            break;
    }
    char score[16];
    sprintf(score, "%u\0", (uint64_t)rep->score * 10);
    out += score;
    out += ",";

    th07_replay_stage_t splits[7];
    for(int i = 0; i < 7; i++) {
		if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th07_replay_stage_t) < flength) {
			th07_replay_stage_t *stage = (th07_replay_stage_t*)&buffer[header->stage_offsets[i]];
            memcpy(splits + i, stage, sizeof(th07_replay_stage_t));
        }
    }

    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].score);
            out += score;
        }
        out += ",";
    }

    out += ",,";    //  score 8-9

    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].piv);
            out += score;
        }
        out += ",";
    }
    
    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].graze);
            out += score;
        }
        out += ",";
    }

    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].cherrymax);
            out += score;
        }
        out += ",";
    }

    out += "\n";
    free(header);
    free(buffer);

    return out;
}

std::string th08csv(unsigned char *buffer, int flength) {
    std::string out = "th08,";

    if(flength < sizeof(th08_replay_header_t)) return "\n";

    unsigned char **buf = &buffer;
    flength = th08decode1(buf, flength);
	buffer = *buf;

    th08_replay_header_t *header = (th08_replay_header_t*)malloc(sizeof(th08_replay_header_t));
	memcpy(header, buffer, sizeof(th08_replay_header_t));
	for(int i = 0; i < 9; i++) {
		if(header->stage_offsets[i] != 0x00) {
			header->stage_offsets[i] -= 104;
		}
	}

    flength = th08decode2(buf, flength);
	buffer = *buf;

    if(flength < sizeof(th08_replay_t)) {
		free(header);
		return "\n";
	}
    th08_replay_t *rep = (th08_replay_t*)buffer;

    out += rep->name;
    out += ",";
    switch(rep->shot) {
        case 0:
            out += "Border Team (Reimu & Yukari),";
            break;
        case 1:
            out += "Magic Team (Marisa & Alice),";
            break;
        case 2:
            out += "Scarlet Devil Team (Sakuya & Remilia),";
            break;
        case 3:
            out += "Ghost Team (Youmu & Yuyuko),";
            break;
        case 4:
            out += "Reimu,";
            break;
        case 5:
            out += "Yukari,";
            break;
        case 6:
            out += "Marisa,";
            break;
        case 7:
            out += "Alice,";
            break;
        case 8:
            out += "Sakuya,";
            break;
        case 9:
            out += "Remilia,";
            break;
        default:
            out += "Unknown,";
            break;
    }
    switch(rep->difficulty) {
        case 0:
            out += "Easy,";
            break;
        case 1:
            out += "Normal,";
            break;
        case 2:
            out += "Hard,";
            break;
        case 3:
            out += "Lunatic,";
            break;
        case 4:
            out += "Extra,";
            break;
        case 5:
            out += "Phantasm,";
            break;
        default:
            out += "Unknown,";
            break;
    }

    char score[16];
    // sprintf(score, "%u\0", (uint64_t)rep->score * 10);
    // out += score;
    // out += ",";


    th08_replay_stage_t splits[9];
    for(int i = 0; i < 9; i++) {
		if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th08_replay_stage_t) < flength) {
			th08_replay_stage_t *stage = (th08_replay_stage_t*)&buffer[header->stage_offsets[i]];
            memcpy(splits + i, stage, sizeof(th08_replay_stage_t));
        }
    }

    bool finalA, extra;

    //  0  1  2  3   4   5  6   7   8
    //  1, 2, 3, 4a, 4b, 5, 6a, 6b, 7

    if(header->stage_offsets[8] == 0) {
        //  not extra
        extra = false;
        if(header->stage_offsets[3] == 0x00) {
            //  4b
            memcpy(&splits[3], &splits[4], sizeof(th08_replay_stage_t));
            header->stage_offsets[3] = header->stage_offsets[4];
        } else {
            //  4a
            //  dont need to copy anything
            //  4b remains 0
        }

        //  copy stage 5 to 4b slot
        memcpy(&splits[4], &splits[5], sizeof(th08_replay_stage_t));
        header->stage_offsets[4] = header->stage_offsets[5];
        
        if(header->stage_offsets[6] == 0x00) {
            //  6b
            finalA = true;
            //  copy 6b to 5 (real 6) slot
            //  nuke the rest, though it really doesnt matter
            memcpy(&splits[5], &splits[7], sizeof(th08_replay_stage_t));
            header->stage_offsets[5] = header->stage_offsets[7];
            header->stage_offsets[6] = 0;
            header->stage_offsets[7] = 0;
        } else {
            //6a
            finalA = false;
            //  copy 6a to 5 (real 6) slot
            //  nuke the rest, though it really doesnt matter
            memcpy(&splits[5], &splits[6], sizeof(th08_replay_stage_t));
            header->stage_offsets[5] = header->stage_offsets[6];
            header->stage_offsets[6] = 0;
            header->stage_offsets[7] = 0;
        }
    } else {
        //  extra, copy to slot 6
        extra = true;
        memcpy(&splits[6], &splits[8], sizeof(th08_replay_stage_t));
        header->stage_offsets[6] = header->stage_offsets[8];
    }

    if(extra) {
        //  print extra stage score as final score
        sprintf(score, "%u\0", (uint64_t)splits[6].score * 10);
        out += score;
        out += ",";
    } else {
        //  print stage 6 score as final score
        sprintf(score, "%u\0", (uint64_t)splits[5].score * 10);
        out += score;
        out += ",";
    }

    //  now start printing the stage data as normal

    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].score);
            out += score;
        }
        out += ",";
    }

    out += ",,";    //  score 8-9

    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].piv);
            out += score;
        }
        out += ",";
    }
    
    for(int i = 0; i < 7; i++) {
        if(header->stage_offsets[i] != 0x00) {
            sprintf(score, "%u\0", splits[i].graze);
            out += score;
        }
        out += ",";
    }

    if(!extra) {
        if(finalA) {
            out += "finalA,";
        } else {
            out += "finalB,";
        }
    } else {
        out += ",";
    }

    out += ",,,,,,";   //  other 2-7

    out += "\n";
    free(header);
    free(buffer);

    return out;
}

std::string th09csv(unsigned char *buffer, int flength) {
    std::string out = "th09,";

    if(flength < sizeof(th09_replay_header_t)) return "\n";

    unsigned char **buf = &buffer;
    flength = th09decode1(buf, flength);
	buffer = *buf;

    th09_replay_header_t *header = (th09_replay_header_t*)malloc(sizeof(th09_replay_header_t));
	memcpy(header, buffer, sizeof(th09_replay_header_t));
	for(int i = 0; i < 40; i++) {
		if(header->stage_offsets[i] != 0x00) {
			header->stage_offsets[i] -= 192;
		}
	}

    flength = th09decode2(buf, flength);
	buffer = *buf;

    if(flength < sizeof(th09_replay_t)) {
		free(header);
		return "\n";
	}
    th09_replay_t *rep = (th09_replay_t*)buffer;

    out += rep->name;
    out += ",";
    
    
    char score[16];
    // sprintf(score, "%u\0", (uint64_t)rep->score * 10);
    // out += score;
    // out += ",";

    th09_replay_stage_t splits[20];
    for(int i = 0; i < 20; i++) {
		if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th09_replay_stage_t) < flength) {
			th09_replay_stage_t *stage = (th09_replay_stage_t*)&buffer[header->stage_offsets[i]];
            memcpy(splits + i, stage, sizeof(th09_replay_stage_t));
        }
    }

    
    int shot;
    //  get shot from one of the stages
    if(header->stage_offsets[9] != 0) {
        //  extra
        shot = splits[9].shot;
    } else {
        //  regular
        shot = splits[0].shot;
    }

    switch(shot) {
        case 0:
            out += "Reimu,";
            break;
        case 1:
            out += "Marisa,";
            break;
        case 2:
            out += "Sakuya,";
            break;
        case 3:
            out += "Youmu,";
            break;
        case 4:
            out += "Reisen,";
            break;
        case 5:
            out += "Cirno,";
            break;
        case 6:
            out += "Lyrica,";
            break;
        case 7:
            out += "Mystia,";
            break;
        case 8:
            out += "Tewi,";
            break;
        case 9:
            out += "Aya,";
            break;
        case 10:
            out += "Medicine,";
            break;
        case 11:
            out += "Yuuka,";
            break;
        case 12:
            out += "Komachi,";
            break;
        case 13:
            out += "Eiki,";
            break;
        case 14:
            out += "Merlin,";
            break;
        case 15:
            out += "Lunasa,";
            break;
        default:
            out += "Unknown,";
            break;
    }

    switch(rep->difficulty) {
        case 0:
            out += "Easy,";
            break;
        case 1:
            out += "Normal,";
            break;
        case 2:
            out += "Hard,";
            break;
        case 3:
            out += "Lunatic,";
            break;
        case 4:
            out += "Extra,";
            break;
        case 5:
            out += "Phantasm,";
            break;
        default:
            out += "Unknown,";
            break;
    }

    if(header->stage_offsets[9] != 0) {
        //  extra
        sprintf(score, "%u\0", (uint64_t)splits[9].score * 10);
        out += score;
        out += ",";

        //  score 1-9
        out += ",,,,,,,,,";
    } else {
        //  regular
        int i;
        for(i=2;i < 8;i++) {
            if(splits[i].score == 0) break;
        }
        sprintf(score, "%u\0", (uint64_t)splits[i].score * 10);
        out += score;
        out += ",";

        for(i = 0; i < 9; i++) {
            if(header->stage_offsets[i] != 0x00) {
                sprintf(score, "%u\0", (uint64_t)splits[i].score * 10);
                out += score;
            }
            out += ",";
        }
    }

    out += ",,,,,,,";   //  piv 1-7
    out += ",,,,,,,";   //  graze 1-7
    out += ",,,,,,,";   //  other 1-7


    out += "\n";
    free(header);
    free(buffer);

    return out;
}