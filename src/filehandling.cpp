#include "filehandling.h"
#include <sstream>
#include <array>


Byte ReadByte(std::ifstream &file){
    Byte value;
    file.read((char*)(&value),1);
    return value;
}
Short ReadShort(std::ifstream &file){
    Short value;
    std::array<Byte,2> bytes;
    file.read((char*)(bytes.data()),2);
    value = bytes[0] | (bytes[1]<<8);
    return value;
}
Int ReadInt(std::ifstream &file){
    Int value;
    std::array<Byte,4> bytes;
    file.read((char*)(bytes.data()),4);
    value = bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
    return value;
}
Long ReadLong(std::ifstream &file){
    Long value;
    std::array<Byte,8> bytes;
    file.read((char*)(bytes.data()),8);
    value = bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24) | (bytes[4]<<32) | (bytes[5]<<40) | (bytes[6]<<48) | (bytes[7]<<56);
    return value;
}

void WriteByte(std::ofstream &file, Byte value){
    file.write((char*)(&value),1);
}
void WriteShort(std::ofstream &file, Short value){
    std::array<Byte,2> bytes;
    bytes[0]= value & 0xFF;
    bytes[1]= (value>>8) & 0xFF;
    file.write((char*)(bytes.data()),2);
}

void WriteInt(std::ofstream &file, Int value){
    std::array<Byte,4> bytes;
    bytes[0]= value & 0xFF;
    bytes[1]= (value>>8) & 0xFF;
    bytes[2]= (value>>16) & 0xFF;
    bytes[3]= (value>>24) & 0xFF;
    file.write((char*)(bytes.data()),4);
}
void WriteLong(std::ofstream &file, Long value){
    std::array<Byte,8> bytes;
    bytes[0]= value & 0xFF;
    bytes[1]= (value>>8) & 0xFF;
    bytes[2]= (value>>16) & 0xFF;
    bytes[3]= (value>>24) & 0xFF;
    bytes[4]= (value>>32) & 0xFF;
    bytes[5]= (value>>40) & 0xFF;
    bytes[6]= (value>>48) & 0xFF;
    bytes[7]= (value>>56) & 0xFF;
    file.write((char*)(bytes.data()),8);
}

std::string SimplifyString(const std::string &string){
    std::size_t firstPos=string.find_first_not_of("\t\n\v\f\r ");
    std::size_t lastPos=string.find_last_not_of("\t\n\v\f\r ");
    if (firstPos==std::string::npos){ // string only contains whitespace
        return "";
    }else{
        // trim string first
        std::string trimmedString=string.substr(firstPos,lastPos-firstPos+1);
        // now remove extra whitespaces
        std::string result;
        bool whitespaceEncountered=false;
        for (std::size_t i=0; i<trimmedString.size();i++){
            if (trimmedString[i]=='\t' || trimmedString[i]=='\n' || trimmedString[i]=='\v' || trimmedString[i]=='\f' || trimmedString[i]=='\r' || trimmedString[i]==' '){
                if (!whitespaceEncountered){
                    result+=' ';
                    whitespaceEncountered=true;
                }
            }else{
                whitespaceEncountered=false;
                result+=trimmedString[i];
            }
        }
        return result;
    }
}

std::string StringToLower(const std::string &string){
    std::string result;
    for(auto elem : string){
        if (elem!=' '){
            result+=std::tolower(elem);
        }
    }
    return result;
}