#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#define spd 25
#define sz  30


struct seg{
    char key;
    int x;
    int y;
};

void create_seg(struct seg *Aseg,int *len, int sx, int sy){
    struct seg new_seg = {'_',sx,sy};
    Aseg[*len] = new_seg;
    *len = *len +1;
}

int disp_segs(struct seg *Aseg,int len, int x, int y){
    for(int i=0;i<len;i++){
        if(Aseg[i].x == y && Aseg[i].y == x){
            return 1;
        }
    }
    return 0;
}


int main() {
    SetConsoleOutputCP(65001); // for emojis;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curs_info;
    GetConsoleCursorInfo(hConsole,&curs_info);
    curs_info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole,&curs_info);
    COORD topLeft = {0,0};


    

    int exitas = 0;
    int buf_sz = 3*sz*sz + sz +1;
    char key;
    
    // int bx=0;
    // int by = 0;

    struct seg Arr_seg[sz*sz];
    int Arr_len_inUse = 1; 
    Arr_seg[0].key = key;
    Arr_seg[0].x = 0;
    Arr_seg[0].y = 0;
    

    char *str= malloc(buf_sz);

    while(!exitas){
        Sleep(1000/spd);

        if(kbhit()){ // for virtually locking the pressed key
            key = getch();
        }

        for (int i = Arr_len_inUse - 1; i > 0; i--) { //fill what key needs to be used for each segment's movement
            Arr_seg[i].key = Arr_seg[i - 1].key;
        }
        Arr_seg[0].key = key;
        
        str[0] = '\0';
        char *pointr = str;
        
        if(key == 'w') by = by - 1; 
        if(key == 's') by = by + 1; 
        if(key == 'a') bx = bx - 1; 
        if(key == 'd') bx = bx + 1;

        if (bx < 0) bx = sz - 1;
        if (bx >= sz) bx = 0;
        if (by < 0) by = sz - 1;
        if (by >= sz) by = 0;


        if(key == '0')
            exitas = 1;


        
        for(int x =0;x<sz;x++){
            for(int y=0;y<sz;y++){

                // if(bx==y && by==x){
                //     strcpy(pointr,"\xE2\xAC\x9B");
                //     pointr += 3;
                // }else{
                //     strcat(pointr,"\xE2\xAC\x9C");
                //     pointr += 3;
                // }
                if(disp_segs(&Arr_seg,Arr_len_inUse,x,y)){
                    strcpy(pointr,"\xE2\xAC\x9B");
                    pointr += 3;

                }else{
                    strcat(pointr,"\xE2\xAC\x9C");
                    pointr += 3;
                }
            }

            strcpy(pointr,"\n");
            pointr += 1;
        }
        SetConsoleCursorPosition(hConsole,topLeft);
        fwrite(str,1,pointr-str,stdout);
        fflush(stdout);
        printf("\nScore: ");
        
    }

    
    curs_info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole,&curs_info);
    free(str);
    return 0;
}