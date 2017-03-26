#include <fstream>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <algorithm>
#pragma pack(1)
#pragma pack(push, bfhead)
using namespace std;
char color_array[10000][10000];
int main()
{
    tagBITMAPFILEHEADER FH;
    tagBITMAPINFOHEADER IH;
    char File_Name[100], Out_Name[100];
    cout << "Please Input the name of your picture(with .bmp): " << endl;
    cin >> File_Name;
    cout << "Please Input the name you want for output(with .bmp):"<<endl;
    cin >> Out_Name;
    fstream openbmp, writebmp;
    openbmp.open(File_Name,ios::in|ios::binary);
    writebmp.open(Out_Name, ios::out|ios::binary);
    openbmp.read((char*)&FH, sizeof(tagBITMAPFILEHEADER));
    int infosize = int(FH.bfOffBits) - 14 - 40;
    openbmp.read((char*)&IH, sizeof IH);
    int color_byte; //一个颜色所占的字节数
    color_byte = IH.biBitCount / 8;
    swap(IH.biXPelsPerMeter, IH.biYPelsPerMeter); // 交换长宽上的分辨率
    swap(IH.biWidth,IH.biHeight); // 交换长和高
    writebmp.write((char*)&FH, sizeof(FH));
    writebmp.write((char*)&IH, sizeof(IH)); // 输出改变后的文件头以及信息头
    if (infosize != 0)
    {
        char another[100];
        openbmp.read((char*)&another, infosize);
        writebmp.write((char*)&another, infosize);
    }
    cout << IH.biWidth << " " << IH.biHeight << endl;
    for (int i = 0; i < IH.biWidth;i++)
            for (int j = 0; j < IH.biHeight;j++)
    {
        char temp;
        int flag = 0;
        for (int k = 0; k < color_byte; k++)
        {
            openbmp.read((char*)&temp, 1);
            color_array[IH.biHeight - j - 1][i*color_byte + flag] = temp;
            flag++;
        }
    }
    for (int i = 0; i < IH.biHeight; i++)
        for (int j = 0; j < color_byte * IH.biWidth;j++)
        {
            writebmp.write((char*)&color_array[i][j], 1);
        }

    openbmp.close();
    writebmp.close();
    cout << "Rotation succeeded!";
    return 0;
}
