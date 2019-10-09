#include <iostream>
#include <math.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

std::string to_string( int x ) {
  int length = snprintf( NULL, 0, "%d", x );
  assert( length >= 0 );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  std::string str( buf );
  delete[] buf;
  return str;
}

string Registers(int num){

    int t[10] = {8,9,10,11,12,13,14,15,24,25};
    int s[8]  = {16,17,18,19,20,21,22,23};

        for(int i=0;i<=9;i++){
            if(num==t[i]){
                return "$t"+to_string(i);
            }
            if(num==s[i]){
                return "$s"+ to_string(i);
            }
        }
}

int btod(char* line,int last, int first){

    int i,sum=0 ;
    int counter = last-first;
    for(i=last;i>=first;i--){
        sum += (line[31-i]-'0')*pow(2,counter);
        counter -= 1;
    }
    return sum;
}

int PC(int next){

    return next/4;
}

char* control_Unit(int op){
    char* sgnl;
    if (op==0)          sgnl="100001001";//R Type
    if (op==2 || op==3) sgnl="010000000";//J Type
    if (op==4)          sgnl="001001000";
    if (op==43||op==35) sgnl="000001110";
    //if (op==4 || op==5 || op==6 || op==8 || op==35 || op==43) sgnl="000000000";//I Type
    return sgnl;

}

int ALU_Control(char* cstr){
    int func;
     func = btod(cstr,5,0);
    return func;
}

int value(string reg){
    int data = rand()%10;
    return data;
}

int ALU(int func,int data1,int data2){
    int result = 0;
    if (func==26) result =  data1 / data2;  //div
    if (func==32) result =  data1 + data2;  //add
    if (func==34) result =  data1 - data2;  //sub
    if (func==36) result =  data1 & data2;  //and
    if (func==37) result =  data1 | data2;  //or
    if (func==38) result =  data1 ^ data2;  //xor
    if (func==39) result = !(data1|data2);//nor
    if (func==42){                        //slt
        if(data1<data2){
            result = 1;
        }else{
            result=0;
        }
    }
    return result;
}

string ShiftLeft2(string str){
    str +="00";
    return str;
}

int add(int x, int y){
    return x+y;
}

string SignExtend(char* cstr){
    string str2="";
    for(int j=16;j<=31;j++){
        if(cstr[j]-'0'==1){
        str2+="1";
        }else{
            str2+="0";
        }
    }
    for(int i=0;i<=15;i++){
        str2 = "0" + str2;
    }
    return str2;
}


int main(){
    string Instruction_Mem[10]={"00000010001100100100000000100000",
                                "00001000000000000000000000000010",
                                "00000010001100100101100000100110",
                                "00010001001010110000000000000000",
                                "00000010001100100110100000101010",
                                "10101101100011010000000000001111",
                                "00010001101010110000000000000000",
                                };
    int PC_Advance=0;
    int CC=0;
    string str2="";
    string str4="";
    string WB,M,EX1,EX2;
    string WB_prev, M_prev;
    cout<<"                                             1       2       3       4       5       6       7       8       9      10      11      12       13     14      "<<"\n";
   // int WB_next=0, M_next=0, EX_next=0;
    int opCode_next;
    int flag=1;
    //#############FETCH##############
    while (!Instruction_Mem[PC(PC_Advance)].empty()){
        string str = Instruction_Mem[PC(PC_Advance)];
        char *cstr = &str[0u];
        PC_Advance = add(PC_Advance, 4);
        CC++;
        //#############DECODE####################
        int opCode = btod(cstr,31,26);
        //cout<<nex t<<":"<<opCode<<"\n";
        char* signals = "";
        signals = control_Unit(opCode);
       // cout<<signals;
        int regDst   = signals[0] - '0';
        int jump     = signals[1] - '0';
        int branch   = signals[2] - '0';
        int memRead  = signals[3] - '0';
        int memToReg = signals[4] - '0';
        int aluOp    = signals[5] - '0';
        int memWrite = signals[6] - '0';
        int aluSrc   = signals[7] - '0';
        int regWrite = signals[8] - '0';
        //cout <<regDst<<jump<<branch<<memRead<<memToReg<<aluOp<<memWrite<<aluSrc<<regWrite<<"\n";
        int func;
        string ReadRegister1, ReadRegister2, WriteRegister;
        int rs,rt,rd;
        int DataReg1,DataReg2,DataRegDest;
        int ALU_result;
        int Zero=0;
        int mul1,mul2;
        int DataMemory;
        int address;
        int offset;
        char* cstr2;
        int MulALU=0;

        if(jump==1){
            mul2 = 1;
            address = btod(cstr,25,0);//address bayad 31 bit bashe

            for(int i=6;i<=31;i++){
                if(cstr[i]-'0'==1){
                    str2 += "1";
                }else{
                    str2 += "0";
                }
            }//previous 4 most significant+ bit[25:0] = 30 bit
            //cout<<str2<"\n";
            str2 = ShiftLeft2(str2);
            //cout<<"\n"<<str2<"\n";
            char *cstr2 = &str2[0u];
            address = btod(cstr2,31,0);
            //cout<<"\n"<<address<<"\n";
            if(mul2==1){
                PC_Advance = address;}
            //cout <<PC_advance;
            M="";
            WB="";
            EX1="";
            EX2="";
            cout<<"j   "<<"0x000000"<<address<<"                            ";
        }else{

            rs = btod(cstr,25,21);
            ReadRegister1 = Registers(rs);
            DataReg1 = value(ReadRegister1);

            rt = btod(cstr,20,16);
            ReadRegister2 = Registers(rt);
            DataReg2 = value(ReadRegister2);

            if(regWrite==1){
                int MulRegister=0;//by default

                if(regDst==1){
                    MulRegister=1;
                    if(MulRegister==1){
                        int rd = btod(cstr,15,11);
                        WriteRegister = Registers(rd);
                        DataRegDest   = value(WriteRegister);
                    }
                }

            }
        }
        CC++;

        //##########fetch_nextl############################

        char *cstr_next = &str[0u];
        opCode_next = btod(cstr_next,31,26);

        //#########EXECUTE  #####################
        if(aluOp==1 ){
            if(opCode==0){//R Type
                func = ALU_Control(cstr);
                ALU_result = ALU(func,DataReg1,DataReg2);
                M="";
                WB=WriteRegister;
                EX1=ReadRegister1;
                EX2=ReadRegister2;

                if (func==26) cout<<"div "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==32) cout<<"add "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==34) cout<<"sub "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==36) cout<<"and "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==37) cout<<"or  "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==38) cout<<"xor "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==39) cout<<"nor "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                if (func==42) cout<<"slt "<<WriteRegister<<", "<<ReadRegister1<<", "<<ReadRegister2<<"   rd:"<<ALU_result<<"      rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";

            }else{

                if(branch==1){
                    str2="";
                    DataReg1 = DataReg2 = 4;

                    if(aluOp==1){
                        if(DataReg1==DataReg2)Zero=1;
                        mul1=branch&Zero;
                    }
                    if(mul1==1){

                    // cout <<"\n" << cstr << "\n";
                       str2 = SignExtend(cstr);//16 bit -> 32 bit
                  //cout << str2 << "\n";
                       str2 = ShiftLeft2(str2);
                     // cout << str2 << "\n";
                       char *cstr2 = &str2[0u];
                       offset = btod(cstr2,15,0);
                       PC_Advance = add(offset,PC_Advance);//chon next barabar ba next+1 bud
                    //cout<<PC_Advance;
                    }
                     offset = btod(cstr,15,0);
                     M  ="";
                     WB ="";
                     EX1=ReadRegister1;
                     EX2=ReadRegister2;
                     cout <<"beq "<<ReadRegister1<<", "<<ReadRegister2<<", "<< offset<<"               rs:"<<DataReg1<<"  rt:"<<DataReg2<<" ";
                }
                if(memWrite==1){
                   int MulRegister=0;
                   if (aluSrc==1){
                        MulALU=1;
                   }
                    str2="";
                    str2 = SignExtend(cstr);
              //      cout << str2 << "\n";
                    offset = btod(cstr,15,0);
                    DataReg2 = add(DataReg1,offset);
              //      cout << DataReg1 << " " << offset << " " << DataReg2 << "\n";
                    M=ReadRegister2;
                    WB="";
                    EX1=ReadRegister1;
                    EX2="";
                    if(opCode=43)cout <<"sw  "<<ReadRegister2<<", "<<offset<<"("<<ReadRegister1<<")"<<"   offset:"<<offset<<"  rs:"<<DataReg1<<"  rt:"<<DataReg2;

                }
            }
        }
        CC++;

        //##############WriteBack######################
        if(memWrite==0){
            DataRegDest = ALU_result;

        }else{
            DataMemory = DataReg2;
        }
        CC++;

        //cout <<DataReg1<<" "<<DataReg2<<" "<<DataRegDest<<"\n";
        //cout<<ReadRegister1+" "<<ReadRegister2+" "<<WriteRegister+" "<<"\n";


        //this part used when jump is high and 4 most significant bits are truncated when converting assembler to address
         str2="";
        if(opCode==0){//4 ta 0
            for(int i=0;i<=3;i++){
                if(cstr[i]-'0'==1){
                    str2 += "1";
                }else{
                    str2 += "0";
                }
            }
        }

        if(flag==1){
            cout<<"[__IM__]"<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
            str4+="        ";
        }
        else{
            if(WB_prev!=""){
                if(WB_prev==EX1 || WB_prev==EX2 ){
                    cout<<str4<<"[__IM__]"<<"        "<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
                    str4+="                ";
                }else{
                    cout<<str4<<"[__IM__]"<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
                    str4+="        ";
                }

            }
            if(M_prev!=""){
                if(M_prev==EX1 || M_prev==EX2){
                    cout<<str4<<"[__IM__]"<<"        "<<"        "<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
                    str4+="                        ";
                }else{
                    cout<<str4<<"[__IM__]"<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
                    str4+="        ";
                }
            }
            if(M_prev==""&&WB_prev==""){
                cout<<str4<<"[__IM__]"<<"[__Re__]"<<"[__Ex__]"<<"[__MA__]"<<"[__WB__]"<<"\n\n";
                str4+="        ";
            }

      //  cout<<" "<<"    [__IM__]    "<<"    [__Re__]    "<<"    [__Ex__]    "<<"    [__MA__]    "<<"    [__WB__]    "<<"\n\n";
        }
        flag=0;
        WB_prev=WB;
        M_prev = M;
        //cout<<"\n"<<WB_prev<<M_prev<<"\n";
    }
 }
