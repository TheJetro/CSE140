#include <stdio.h>
#include <string.h>

char code[33];

void rHandler();
void iHandler();
void jHandler(char opcode[]);
void opCheck(char opcode[]);
int toInt(char convertMe[]);


int main() {   
    
    char opcode[7];
   
    printf("Enter an instruction in machine code:\n");  
    
    // reads and stores input
    scanf("%s", &code);

    for(int i = 0; i < 6; i = i + 1){
        opcode[i]=code[i];
    }

    opCheck(opcode);

    // displays output  
    return 0;
}

void rHandler(){
    char operation[6];
    char rs[6];
    char rt[6];
    char rd[6];
    char funct[7];

    for(int i = 26; i < 32; i = i + 1){
        funct[i-26]=code[i];
    }

    printf("\nOperation:");

    if(toInt(funct) == 32){
        printf(" add");
    } else if (toInt(funct) == 33) {
        printf(" addu");
    } else if (toInt(funct) == 36) {
        printf(" and");
    } else if (toInt(funct) == 8) {
        printf(" jr");
    }else if(toInt(funct) == 39){
        printf(" nor");
    }else if(toInt(funct) == 37){
        printf(" or");
    }else if(toInt(funct) == 42){
        printf(" slt");
    }else if(toInt(funct) == 43){
        printf(" sltu");
    }else if(toInt(funct) == 0){
        printf(" sll");
    }else if(toInt(funct) == 2){
        printf(" srl");
    }else if(toInt(funct) == 34){
        printf(" sub");
    }else if(toInt(funct) == 35){
        printf(" subu");
    }else{
        printf(" Unknown");
    }

    for(int i = 6; i < 11; i = i + 1){
        rs[i-6]=code[i];
    }

    rs[5] - '\0';

    printf("\nRs: ");
    printf("(R%i", toInt(rs));
    printf(")");
    
    
    for(int i = 11; i < 16; i = i + 1){
        rt[i-11]=code[i];
    }

    rt[6] - '\0';

    printf("\nRt: ");
    printf("(R%i", toInt(rt));
    printf(")");
    
    for(int i = 16; i < 21; i = i + 1){
        rd[i-16]=code[i];
    }

    rd[5] = '\0';

    printf("\nRd: ");
    printf("(R%i", toInt(rd));
    printf(")");

    for(int i = 21; i < 26; i = i + 1){
        operation[i-21]=code[i];
    }

    printf("\nShamt: ");
    printf("%i", toInt(operation));

    printf("\nFunct: ");
    printf("%i", toInt(funct));
}

void iHandler(){
    char operation[7];
    char rs[6];
    char rt[6];
    char imm[17];

    for(int i = 0; i < 6; i = i + 1){
        operation[i]=code[i];
    }

    printf("\nOperation:");

    if(toInt(operation) == 8){
        printf(" addi");
    } else if (toInt(operation) == 9) {
        printf(" addiu");
    } else if (toInt(operation) == 12) {
        printf(" andi");
    } else if (toInt(operation) == 4) {
        printf(" beq");
    } else if (toInt(operation) == 5) {
        printf(" bne");
    } else if (toInt(operation) == 36) {
        printf(" lbu");
    } else if (toInt(operation) == 37) {
        printf(" lhu");
    } else if (toInt(operation) == 43) {
        printf(" sw");
    } else if (toInt(operation) == 15) {
        printf(" lui");
    } else if (toInt(operation) == 35) {
        printf(" lw");
    } else if (toInt(operation) == 13) {
        printf(" ori");
    } else {
        printf(" Unknown");
    }

    for(int i = 6; i < 11; i = i + 1){
        rs[i-6]=code[i];
    }

    rs[5] = '\0';

    printf("\nRs: ");
    printf("(R%i", toInt(rs));
    printf(")");

    for(int i = 11; i < 16; i = i + 1){
        rt[i-11]=code[i];
    }

    rt[6] = '\0';

    printf("\nRt: ");
    printf("(R%i", toInt(rt));
    printf(")");
    
    for(int i = 16; i < 32; i = i + 1){
        imm[i-16]=code[i];
    }
    
    printf("\nImmediate: ");
    printf("%i", toInt(imm));
}

void jHandler(char opcode[]){
    char Address[25];

    for(int i = 6; i <= 32; i++){
        Address[i-6] = code[i];
    }
    if(toInt(opcode) == 2){
        printf("\nOperation: j \n");
        printf("Target address: ");
        printf("%i", toInt(Address));
    } else {
        printf("\nOperation: jal \n");
        printf("Target address: ");
        printf("%i", toInt(Address));
    }
}

void opCheck(char opcode[]){
    if(toInt(opcode) == 2){
        printf("\nInstruction Type: J");
        jHandler(opcode);
    } else if(toInt(opcode) == 3){
        printf("\nInstruction Type: J");
        jHandler(opcode);
    } else if (toInt(opcode) == 0){ //if opcode is zero we know instruction is R type
        printf("\nInstruction Type: R");
        rHandler(code);
    } else { //if opcode is not zero we know instruction is I type
        printf("\nInstruction Type: I");
        iHandler(code);
    }
}

int toInt(char convertMe[]){ //reads in char input and converts it to integer
    char* begin = &convertMe[0];
    int total = 0;
    while (*begin)
    {
        total *= 2;
        if (*begin++ == '1') total += 1;
    }

    return total;
}