    #include <stdio.h>
    #include <windows.h>
    #include <conio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <time.h>

    // #define spd 25
    #define sz  30

    //-----------------------Segment struct and its related functions-----------------

    struct Seg{
        char key;
        int x;
        int y;
    };

    void create_seg(struct Seg *Aseg,int *len, int sx, int sy){
        struct Seg new_seg = {'_',sx,sy};
        Aseg[*len] = new_seg;
        *len = *len +1;
    }

    int disp_segs(struct Seg *Aseg,int len, int x, int y){
        for(int i=0;i<len;i++){
            if(Aseg[i].x == y && Aseg[i].y == x){
                return 1;
            }
        }
        return 0;
    }

    //---------------------------Food struct and its related fucntions------------------------
    struct Food{
        int x;
        int y;
        int isEaten;
        // int onTail;
    };



    void gen_food(struct Seg *Aseg, struct Food *fd, int len,int siz){
        
        int fx = (rand() % siz);
        int fy = (rand() % siz);
        fd->x =fx;
        fd->y = fy;
        fd->isEaten = 0;
        for(int i=0;i<len;i++){
            if(Aseg[i].x == fy && Aseg[i].y == fx){
                gen_food(Aseg, fd, len, siz);
                return;
            }
        }
    }

    int is_eaten(struct Seg *Aseg, struct Food *fd, int idx){
        if(Aseg[idx].x == fd->y && Aseg[idx].y == fd->x){
            fd->isEaten = 1;
        }
    }
//-----------------Bit my own body test---------------------
int bit_body(struct Seg *Aseg, int len){
    for(int i=1;i<len;i++){
        if(Aseg[0].x == Aseg[i].x && Aseg[0].y == Aseg[i].y) // head and a seg share same position(bit itself)
            return 1;       
    }
    return 0;
}



    int main() {//---------------------------------------------------------------------
        int spd = 15;
        srand(time(NULL));
        SetConsoleOutputCP(65001); // for emojis;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO curs_info;
        GetConsoleCursorInfo(hConsole,&curs_info);
        curs_info.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole,&curs_info);
        COORD topLeft = {0,0};


        

        int exitas = 0;
        int buf_sz = 3*sz*sz + sz +1;
        char key = 'd';
        
    
        //---Segments---
        struct Seg Arr_seg[sz*sz];
        int Arr_len_inUse = 3; 
        Arr_seg[0].key = key;
        Arr_seg[0].x = 0;
        Arr_seg[0].y = 0;

        Arr_seg[1].key = key;
        Arr_seg[1].x = 0;
        Arr_seg[1].y = 1;

        Arr_seg[2].key = key;
        Arr_seg[2].x = 0;
        Arr_seg[2].y = 2;

        //----Food----
        int fx = 0;
        int fy = 0;
        struct Food food = {0,0,0};
        gen_food(Arr_seg, &food,Arr_len_inUse,sz);
        

        char *str= malloc(buf_sz);


        // int fxx = -1;
        // int fyy = -1;
        while(!exitas){
            Sleep(1000/spd);

            if(kbhit()){ // for virtually locking the pressed key
                key = getch();
            }




    //-----------------Movement of segments---------------

            for (int i = Arr_len_inUse - 1; i > 0; i--) {
                Arr_seg[i].x = Arr_seg[i - 1].x;
                Arr_seg[i].y = Arr_seg[i - 1].y;
            }
            






            
            str[0] = '\0';
            char *pointr = str;
            
    
    //---------------------------- moving head----------------------------

            if(key == 'w' && Arr_seg[0].key != 's')      Arr_seg[0].key = 'w';
            else if(key == 's' && Arr_seg[0].key != 'w') Arr_seg[0].key = 's';
            else if(key == 'a' && Arr_seg[0].key != 'd') Arr_seg[0].key = 'a';
            else if(key == 'd' && Arr_seg[0].key != 'a') Arr_seg[0].key = 'd';
            else {
                key = Arr_seg[0].key; 
            }        


            if(Arr_seg[0].key == 'w') Arr_seg[0].y = Arr_seg[0].y - 1;
            if(Arr_seg[0].key == 's') Arr_seg[0].y = Arr_seg[0].y + 1;
            if(Arr_seg[0].key == 'a') Arr_seg[0].x = Arr_seg[0].x - 1;
            if(Arr_seg[0].key == 'd') Arr_seg[0].x = Arr_seg[0].x + 1;


            if (Arr_seg[0].x < 0) Arr_seg[0].x = sz - 1;
            if (Arr_seg[0].x >= sz) Arr_seg[0].x = 0;
            if (Arr_seg[0].y < 0) Arr_seg[0].y = sz - 1;
            if (Arr_seg[0].y >= sz) Arr_seg[0].y = 0;



            is_eaten(Arr_seg, &food,0);
            
            if(food.isEaten){
                int tailx = Arr_seg[Arr_len_inUse-1].x;
                int taily = Arr_seg[Arr_len_inUse-1].y;
                create_seg(Arr_seg,&Arr_len_inUse,tailx,taily);
                gen_food(Arr_seg, &food, Arr_len_inUse, sz);
                food.isEaten = 0;
                spd = spd*1.06;
            }

            
            if(bit_body(Arr_seg,Arr_len_inUse)){
                key = '0';
            }

            if(key == '0')
                exitas = 1;

    //--------------------Buffering screen-----------------------
            
            for(int x =0;x<sz;x++){
                for(int y=0;y<sz;y++){

        
                    if(disp_segs(Arr_seg,Arr_len_inUse,x,y)){
                        strcpy(pointr,"\xE2\xAC\x9B"); //
                        pointr += 3;
                    }else if(food.x == x && food.y == y){
                        strcpy(pointr,"\xE2\xAD\x90");
                        pointr += 3;
                    }else{
                        strcpy(pointr,"\xE2\xAC\x9C");
                        pointr += 3;
                    }
                }

                strcpy(pointr,"\n");
                pointr += 1;
            }
            SetConsoleCursorPosition(hConsole,topLeft);
            fwrite(str,1,pointr-str,stdout);
            fflush(stdout);
            // printf("\nScore: %d",Arr_len_inUse-2);
            
        }
        int midx = sz/2;
        int midy = sz/2;
        COORD MID = {midx+3,midy-2};
        COORD end = {0,sz+1};

        SetConsoleCursorPosition(hConsole,MID);
        printf("Game Over >.<");
        SetConsoleCursorPosition(hConsole,end);

        curs_info.bVisible = TRUE;
        SetConsoleCursorInfo(hConsole,&curs_info);
        free(str);
        return 0;
    }