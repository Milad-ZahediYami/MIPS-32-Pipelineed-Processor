#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <cassert>
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

std::string dtoh (int num){
    std::string answer;
    int counter=0;
    if (num==0) answer="0";
    while(num>0){
        int rem = num%16;
        if (rem > 9) {
            switch (rem) {
                case 10: answer = "A" + answer;
                case 11: answer = "B" + answer;
                case 12: answer = "C" + answer;
                case 13: answer = "D" + answer;
                case 14: answer = "E" + answer;
                case 15: answer = "F" + answer;
            }
        }else{
            answer+=to_string(rem);
        }
        num/=16;
        counter+=1;
    }

    return answer;
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

std::string registers(int num){

    int t[10]={8,9,10,11,12,13,14,15,24,25};
    int s[8]={16,17,18,19,20,21,22,23};
    std::string reg;
        for(int i=0;i<=9;i++){
            if(num==t[i]){
                return "$t"+to_string(i);
            }else if(num==s[i]){
                return "$s"+ to_string(i);
            }
        }
}


int main(){
   std::ifstream file("read.txt");
   std::string str;
  while (std::getline(file, str)){
       char *cstr = &str[0u];
       int rs,rt,rd,sham,func;
       int op=btod(cstr,31,26);
       if(op==0){
           std::string str1;
           rs = btod(cstr, 25, 21);
           rt = btod(cstr, 20, 16);
           rd = btod(cstr, 15, 11);
           sham = btod(cstr, 10, 6);
           func = btod(cstr, 5, 0);
           if(sham==0){
              if (func==26) str1 += "div ";
              if (func==32) str1 += "add ";
              if (func==34) str1 += "sub ";
              if (func==36) str1 += "and ";
              if (func==37) str1 += "or" ;
              if (func==38) str1 += "xor ";
              if (func==39) str1 += "nor ";
              if (func==42) str1 += "slt ";
              str1 += registers(rd)+", ";
              str1 += registers(rs)+", ";
              str1 += registers(rt);

           }else{
                if(func==0)str1 += "sll ";
                if(func==2)str1 += "srl ";
                str1+=registers(rd)+", ";
                str1+=registers(rt)+",";
                str1+= " "+to_string(sham);
           }
            cout<<str1<<"\n";
       }
       if(op==2 || op==3){
          std::string str1;
          if(op==2)str1+="j ";
          if(op==3)str1+="jal ";
          int address = btod(cstr,25,0);
          str1 += "  0x" + dtoh(address);
           cout<<str1<<"\n";
        }


       if(op==35 || op==43 ||op==4 || op==5 || op==6 || op==8 ){
           std::string str1;
           rs = btod(cstr, 25, 21 );
           rt = btod(cstr, 20, 16);
           int address = btod(cstr,15,0);

           if(op==35) str1 += "lw  " + registers(rt) + ", " + dtoh(address) + "(" + registers(rs) + ")" ;
           if(op==43) str1 += "sw  " + registers(rt) + ", " + dtoh(address) + "(" + registers(rs) + ")" ;

           if(op==4)  str1 += "beq " + registers(rs) + ", " + registers(rt) +", " + dtoh(address);
           if(op==5)  str1 += "bne " + registers(rs) + ", " + registers(rt) +", " + "Exit";
           if(op==8)  str1 += "addi "+ registers(rs) + ", " + registers(rt) +", " + dtoh(address);

            cout << str1<<"\n";
       }
  }

}


