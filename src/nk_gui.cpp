#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include "nk_gui.h"

#include "nuklear/nuklear.h"
#include "nuklear/nuklear_gdi.h"
#include "window.h"
#include "filehandling.h"
#include "cards.h"
#include <algorithm>
#include <set>

GdiFont* font;
struct nk_context *ctx;
unsigned int windowWidth;
unsigned int windowHeight;


std::string infoString;
std::unordered_map<Short,std::string> ID_to_name;
std::unordered_map<std::string,Short> name_to_ID;
int checkedDontCheckDeckSize;


void SetupGui(WINDOW_DATA &windowData,unsigned int initialWindowWidth, unsigned int initialWindowHeight){
    font = nk_gdifont_create("Segoe UI", 18);
    windowWidth=initialWindowWidth;
    windowHeight=initialWindowHeight;
    ctx = nk_gdi_init(font, windowData.dc, initialWindowWidth, initialWindowHeight);
    
    LoadCards(ID_to_name,name_to_ID);
    
    infoString="";
    checkedDontCheckDeckSize=0;
    
}

void HandleInput(WINDOW_DATA &windowData){
        MSG msg;
        nk_input_begin(ctx);
        if (windowData.needs_refresh == 0) {
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                windowData.running = 0;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            windowData.needs_refresh = 1;
        } else windowData.needs_refresh = 0;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                windowData.running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            windowData.needs_refresh = 1;
        }
        nk_input_end(ctx);
        
}

int HandleEvent(const EVENT_DATA &eventData){
    switch (eventData.msg)
    {
	case WM_SIZE:
        windowWidth=LOWORD(eventData.lparam);
        windowHeight=HIWORD(eventData.lparam);
		return nk_gdi_handle_event(eventData.wnd, eventData.msg, eventData.wparam, eventData.lparam);
    default:
        return nk_gdi_handle_event(eventData.wnd, eventData.msg, eventData.wparam, eventData.lparam);
    }
    
}

void HandleGui(){
    if (nk_begin(ctx, "Demo", nk_rect(0, 0, windowWidth, windowHeight),
        0))
    {
        nk_layout_row_dynamic(ctx,125,1);
        if(nk_group_begin_titled(ctx, "group_convert", "Convert", NK_WINDOW_TITLE|NK_WINDOW_BORDER)){
            nk_layout_row_static(ctx, 0, 100, 2);
            if (nk_button_label(ctx, "YDC to TXT")){
                std::string ydcFilename=OpenFilename("YGO LOTD LE YDC Deck Files (*.ydc)\0*.*\0");
                if (!ydcFilename.empty()){
                    std::ifstream ydcFile(ydcFilename,std::ios::in|std::ios::binary);
                    if (ydcFile.is_open()){
                        std::vector<short> mainIDs;
                        std::vector<short> extraIDs;
                        std::vector<short> sideIDs;
                        Long unknownValue=ReadLong(ydcFile);// no idea what this value does, so ignoring it
                        int mainCount=ReadShort(ydcFile);
                        for (int i=0; i< mainCount;i++){
                            mainIDs.push_back(ReadShort(ydcFile));
                        }
                        int extraCount=ReadShort(ydcFile);
                        for (int i=0; i< extraCount;i++){
                            extraIDs.push_back(ReadShort(ydcFile));
                        }
                        int sideCount=ReadShort(ydcFile);
                        for (int i=0; i< sideCount;i++){
                            sideIDs.push_back(ReadShort(ydcFile));
                        }
                        ydcFile.close();

                        std::string txtFilename=SaveFilename("YGO LOTD LE TXT Deck Files (*.txt)\0*.*\0");
                        if (!txtFilename.empty()){
                            std::ofstream txtFile(txtFilename);
                            if (txtFile.is_open()){
                                txtFile << "[main]\n";
                                //txtFile << mainCount << "\n";
                                for (int i=0; i<mainCount; i++){
                                    txtFile << ID_to_name[mainIDs[i]] << "\n";
                                }
                                txtFile << "[extra]\n";
                                //txtFile << extraCount << "\n";
                                for (int i=0; i<extraCount; i++){
                                    txtFile << ID_to_name[extraIDs[i]] << "\n";
                                }
                                txtFile << "[side]\n";
                                //txtFile << sideCount << "\n";
                                for (int i=0; i<sideCount; i++){
                                    //txtFile << ID_to_name[sideIDs[i]] << "\n";
                                }
                                txtFile << "[unknown]\n";
                                txtFile << unknownValue;
                                txtFile.close();
                                infoString="Successfully exported the deck to "+txtFilename;
                            }else{
                                infoString="Unable to open " + txtFilename;
                            }
                        }else{
                            infoString="Canceled file dialog";
                        }
                    }else{
                        infoString="Unable to open " + ydcFilename;
                    }
                }else{
                    infoString="Canceled file dialog";
                }
            }
            if (nk_button_label(ctx, "TXT to YDC")){
                std::string txtFilename=OpenFilename("YGO LOTD LE TXT Deck Files (*.txt)\0*.*\0");
                if (!txtFilename.empty()){
                    std::ifstream txtFile(txtFilename);
                    if(txtFile.is_open()){
                        std::string currLine;
                        std::vector<std::string> lines;
                        while (std::getline(txtFile, currLine)){
                            std::string simplifiedString=SimplifyString(currLine);
                            if (!simplifiedString.empty() && simplifiedString.front()!='#'){ // skip comments and empty lines
                                lines.push_back(simplifiedString);
                            }
                        }
                        txtFile.close();
                        std::vector<std::string> mainCards;
                        std::vector<std::string> extraCards;
                        std::vector<std::string> sideCards;
                        int unknownValue=0x648C;
                        bool readableDeck=false;
                        std::vector<std::string> missingMain;
                        std::vector<std::string> missingExtra;
                        std::vector<std::string> missingSide;
                        int parseMode=0; //0=None, 1=Main, 2=Extra, 3=Side, 4=unknownValue
                        if (lines.empty()){
                            infoString="Deck contains no cards, please check your .txt file!";
                        }else{
                            for (int i=0; i<lines.size();i++){
                                if (lines[i]=="[main]")
                                    parseMode=1;
                                else if (lines[i]=="[extra]")
                                    parseMode=2;
                                else if (lines[i]=="[side]")
                                    parseMode=3;
                                else if (lines[i]=="[unknown]")
                                    parseMode=4;
                                else{
                                    if (parseMode==4){
                                        if (std::all_of(lines[i].begin(), lines[i].end(), ::isdigit)){
                                            unknownValue=std::stoi(lines[i]);
                                        }
                                    }else{
                                        std::string cardName=StringToLower(lines[i]);
                                        bool cardExists=name_to_ID.count(cardName)==1;
                                        if (cardExists){
                                            switch(parseMode){
                                                case 1:
                                                    mainCards.push_back(cardName);
                                                    break;
                                                case 2:
                                                    extraCards.push_back(cardName);
                                                    break;
                                                case 3:
                                                    sideCards.push_back(cardName);
                                                    break;
                                                default:
                                                    break;
                                            }
                                        }else{
                                            switch(parseMode){
                                                case 1:
                                                    missingMain.push_back(cardName);
                                                    break;
                                                case 2:
                                                    missingExtra.push_back(cardName);
                                                    break;
                                                case 3:
                                                    missingSide.push_back(cardName);
                                                    break;
                                                default:
                                                    break;
                                            }
                                        }
                                    }
                                    
                                }
                            }
                        }
                        bool abort=false;
                        if (!checkedDontCheckDeckSize){
                            if (mainCards.size() <40 || mainCards.size() >60){
                                infoString="Main Deck must be between 40 and 60 cards";
                                abort=true;
                            }else if (extraCards.size() <0 || extraCards.size() >15){
                                infoString="Extra Deck must be between 0 and 15 cards";
                                abort=true;
                            }else if (sideCards.size() <0 || sideCards.size() >15){
                                infoString="Side Deck must be between 0 and 15 cards";
                                abort=true;
                            }
                            if (abort){
                                std::ofstream missingFile("missingCards.txt");
                                if (missingFile.is_open()){
                                    for (int i=0;i<missingMain.size();i++){
                                        missingFile << missingMain[i] << "\n";
                                    }
                                    for (int i=0;i<missingExtra.size();i++){
                                        missingFile << missingExtra[i] << "\n";
                                    }
                                    for (int i=0;i<missingSide.size();i++){
                                        missingFile << missingSide[i] << "\n";
                                    }
                                    missingFile.close();
                                    infoString+=", check missingCards.txt for omitted cards";
                                }else{
                                    infoString+=", also note that some cards could not be identified";
                                }
                            }
                        }
                        if (!abort){
                            std::string ydcFilename=SaveFilename("YGO LOTD LE YDC Deck Files (*.ydc)\0*.*\0");
                            if (!ydcFilename.empty()){
                                std::ofstream ydcFile(ydcFilename,std::ios::out|std::ios::binary);
                                if (ydcFile.is_open()){
                                    WriteLong(ydcFile,unknownValue);
                                    WriteShort(ydcFile,mainCards.size());
                                    for (int i=0;i< mainCards.size();i++){
                                        WriteShort(ydcFile,name_to_ID[mainCards[i]]);
                                    }
                                    WriteShort(ydcFile,extraCards.size());
                                    for (int i=0;i< extraCards.size();i++){
                                        WriteShort(ydcFile,name_to_ID[extraCards[i]]);
                                    }
                                    WriteShort(ydcFile,sideCards.size());
                                    for (int i=0;i< sideCards.size();i++){
                                        WriteShort(ydcFile,name_to_ID[sideCards[i]]);
                                    }
                                    ydcFile.close();
                                    if (missingMain.empty() && missingExtra.empty() && missingSide.empty()){
                                        infoString="Successfully exported the deck to "+ydcFilename;
                                    }else{
                                        std::ofstream missingFile("missingCards.txt");
                                        if (missingFile.is_open()){
                                            for (int i=0;i<missingMain.size();i++){
                                                missingFile << missingMain[i] << "\n";
                                            }
                                            for (int i=0;i<missingExtra.size();i++){
                                                missingFile << missingExtra[i] << "\n";
                                            }
                                            for (int i=0;i<missingSide.size();i++){
                                                missingFile << missingSide[i] << "\n";
                                            }
                                            missingFile.close();
                                            infoString="Partially exported deck to "+ydcFilename+", check missingCards.txt for omitted cards";
                                        }else{
                                            infoString="Partially exported deck to "+ydcFilename+", but some cards could not be identified";
                                        }
                                    }
                                }else{
                                    infoString="Unable to open " + ydcFilename;
                                }
                            }else{
                                infoString="Canceled file dialog";
                            }
                        }
                    }else{
                        infoString="Unable to open " + txtFilename;
                    }
                }else{
                    infoString="Canceled file dialog";
                }
                
            }
            nk_layout_row_dynamic(ctx,0,1);
            nk_checkbox_label(ctx, "Don't check deck size", &checkedDontCheckDeckSize);
            nk_group_end(ctx);
        }
        
        nk_layout_row_dynamic(ctx,0,1);
        nk_edit_string_zero_terminated(ctx,NK_EDIT_INACTIVE,const_cast<char*>(("INFO: "+infoString).c_str()),INT_MAX,nk_filter_default);
    }
    nk_end(ctx);
}


void RenderGui(){
    nk_gdi_render(nk_rgb(30,30,30));
}

void CleanupGui(){
    nk_gdifont_del(font);
}

void UpdateWindowSize(unsigned int width, unsigned int height){
    windowWidth=width;
    windowHeight=height;
}