#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int pc = 0, next_pc = 0, branch_target = 0, jump_target = 0, alu_zero = 0, total_clock_cycles = 0;
int RegWrite = 0, RegDst = 0, Branch = 0, ALUSrc = 0, InstType = 0, MemWrite = 0, MemtoReg = 0, MemRead = 0, Jump = 0; 
int registerfile[32] = {0};
int dmem[32] = {0};
bool done = false;

void rHandler(string instruction);
void iHandler(string instruction);
void jHandler(string instruction);
string registerHandler(int reg);
void Decode(string instruction);
void Execute(string alu_op, string rt, string rs, string imm);
void Mem(string saveorload, string rt, int data);
void Writeback(string rt, int value);
string ControlUnit(string operation);
int toInt(string convertMe);

void Fetch(string instruction){
  next_pc = pc + 4;
  
  Decode(instruction);
}

void rHandler(string instruction){
    string rs = instruction.substr(6,5);
    string rt = instruction.substr(11,5);
    string rd = instruction.substr(16,5);
    string shamt = instruction.substr(21,5);
    string funct = instruction.substr(26,6);

    // printf("\nOperation:");

    if(toInt(funct) == 32){
        //printf(" add");
    }else if (toInt(funct) == 36) {
        //printf(" and");
    }else if(toInt(funct) == 39){
        //printf(" nor");
    }else if(toInt(funct) == 37){
        //printf(" or");
    }else if(toInt(funct) == 42){
        //printf(" slt");
        Execute(ControlUnit(funct), rd, rs, rt); //executing slt for SLT    R[rd] = (R[rs] < R[rt]) ? 1 : 0
    }else if(toInt(funct) == 34){
        //printf(" sub");
        Execute(ControlUnit(funct), rd, rs, rt); //executing sub for SUB    R[rd] = R[rs] - R[rt]
    }else{
        cout << "Unknown" << toInt(funct) << endl;
    }

    // printf("\nRs: ");
    // printf("(R%i", toInt(rs));
    // printf(")");

    // printf("\nRt: ");
    // printf("(R%i", toInt(rt));
    // printf(")");
    
    // printf("\nRd: ");
    // printf("(R%i", toInt(rd));
    // printf(")");

    // printf("\nShamt: ");
    // printf("%i", toInt(shamt));

    // printf("\nFunct: ");
    // printf("%i", toInt(funct));
}

void iHandler(string instruction){
    string operation = instruction.substr(0,6);
    string rs = instruction.substr(6,5);
    string rt = instruction.substr(11,5);
    string imm = instruction.substr(16,16);

    //printf("\nOperation:");

    if (toInt(operation) == 4) {
        //printf(" beq");
        Execute(ControlUnit(operation), rt, rs, imm); //executing sub for BEQ
    }else if (toInt(operation) == 43) {
        //printf(" sw");
        Execute(ControlUnit(operation), rt, rs, imm); //executing add for SW
    }else if (toInt(operation) == 35) {
        //printf(" lw");
        Execute(ControlUnit(operation), rt, rs, imm); //executing add for LW
    } else {
        printf(" Unknown");
    }
}

void jHandler(string instruction){
    string operation = instruction.substr(0,6);
    string address = instruction.substr(6,26);

    if(toInt(operation) == 2){
        printf("\nOperation: j \n");
        printf("Target address: ");
        printf("%i", toInt(address));
    } else {
        printf("\nOperation: UNONWN \n"); //used to be JAL
        printf("Target address: ");
        printf("%i", toInt(address));
    }
}

void Decode(string instruction){
    string opcode = instruction.substr(0,6);
    if(toInt(opcode) == 2 || toInt(opcode) == 3){
        printf("\nInstruction Type: J");
        jHandler(instruction);
    } else if (toInt(opcode) == 0){ //if opcode is zero we know instruction is R type
        //printf("\nInstruction Type: R");
        rHandler(instruction);
    } else { //if opcode is not zero we know instruction is I type
        //printf("\nInstruction Type: I");
        iHandler(instruction);
    }
}

void Execute(string alu_op, string destination, string rs, string immORrt){
    if(alu_op == "0000"){
        //AND &&
    } else if (alu_op == "0001"){
        //OR ||
    } else if (alu_op == "0010"){
        //ADD +
        if (MemRead == 0 && InstType == 0){  //if we are SW instruction
        int result = registerfile[toInt(rs)] + toInt(immORrt); 
        Mem("sw", destination, result);
        } else if (MemRead == 1 && InstType == 0){ //if we are LW instruction
        int result = registerfile[toInt(rs)] + toInt(immORrt); 
        Mem("lw", destination, result);
        }
    } else if (alu_op == "0110"){
        //SUB -   also used by BEQ
        if (Jump == 1 && InstType == 0){
            int result = registerfile[toInt(rs)] - registerfile[toInt(immORrt)];  //NEED IMPLEMENTATION OF CONTROL UNIT TO BE ABLE TO FUNCTION AS INTENDED
            
            //USING THIS FOR NOW UNTIL CONTROL UNIT IS IMPLEMENTED PROPERLY
            total_clock_cycles = total_clock_cycles + 1;
            cout << "\ntotal_clock_cycles " << total_clock_cycles << " :" << endl;    
                if(result == 0){
                    int sl2 = toInt(immORrt);
                    sl2 = sl2 << 2;
                    branch_target = next_pc + sl2;
                } else {
                    branch_target = next_pc;
                }

            pc = branch_target;
            //next 3 lines convert simp int pc value to chad hex pc value
            stringstream ssp;
            ssp << hex << branch_target;
            string pcres = ssp.str();

            cout << "pc is modified to 0x" << pcres << endl;
        } else {
            int result = registerfile[toInt(rs)] - registerfile[toInt(immORrt)];  //NEED IMPLEMENTATION OF CONTROL UNIT TO BE ABLE TO FUNCTION AS INTENDED
            registerfile[toInt(destination)] = result; 

            //USING THIS FOR NOW UNTIL CONTROL UNIT IS IMPLEMENTED PROPERLY
            total_clock_cycles = total_clock_cycles + 1;

            cout << "\ntotal_clock_cycles " << total_clock_cycles << " :" << endl;
            
            //next 3 lines convert simp int value to chad hex result
            stringstream ss;
            ss << hex << registerfile[toInt(destination)];
            string res = ss.str();

            //next 3 lines convert simp int pc value to chad hex pc value
            pc = next_pc;
            stringstream ssp;
            ssp << hex << pc;
            string pcres = ssp.str();

            cout << registerHandler(toInt(destination)) << " is modified to 0x" << res << endl;
            cout << "pc is modified to 0x" << pcres << endl;
        }
    } else if (alu_op == "0111"){
        int result = 0;
        //SLT For set-on-less-than, you may want to run a 
        //combination of C/C++ lines to compare two values 
        //and generate the set/unset output
        if (registerfile[toInt(rs)] < registerfile[toInt(immORrt)]){
            result = 1;
        }
        //NEED IMPLEMENTATION OF CONTROL UNIT TO BE ABLE TO FUNCTION AS INTENDED
        registerfile[toInt(destination)] = result; 

        //USING THIS FOR NOW UNTIL CONTROL UNIT IS IMPLEMENTED PROPERLY
        total_clock_cycles = total_clock_cycles + 1;

        cout << "\ntotal_clock_cycles " << total_clock_cycles << " :" << endl;
        
        //next 3 lines convert simp int value to chad hex result
        stringstream ss;
        ss << hex << registerfile[toInt(destination)];
        string res = ss.str();

        //next 3 lines convert simp int pc value to chad hex pc value
        pc = next_pc;
        stringstream ssp;
        ssp << hex << pc;
        string pcres = ssp.str();

        cout << registerHandler(toInt(destination)) << " is modified to 0x" << res << endl;
        cout << "pc is modified to 0x" << pcres << endl;
    } else if (alu_op == "1100"){
        //NOR
    } else {
        //error
    }

    alu_zero = 1;
}

void Mem(string saveorload, string rt, int data){
    if (saveorload == "lw"){
        // For the LW, the value stored in the indicated d-mem array 
        // entry should be retrieved and sent to the Writeback() function
        
        int value = dmem[data/4]; //dividing by 4 to pass correct location of data
        Writeback(rt, value);
    } else if (saveorload == "sw"){
        int value = registerfile[toInt(rt)];
        total_clock_cycles = total_clock_cycles + 1;

        cout << "\ntotal_clock_cycles " << total_clock_cycles << " :" << endl;
        
        dmem[data/4]=value;
        //value for lw should be 116 here
        //next 3 lines convert simp int value to chad hex result
        stringstream ss;
        ss << hex << dmem[data/4];
        string res = ss.str();

        stringstream sd;
        sd << hex << data;
        string mres = sd.str();

        //next 3 lines convert simp int pc value to chad hex pc value
        pc = next_pc;
        stringstream ssp;
        ssp << hex << pc;
        string pcres = ssp.str();

        cout << "memory 0x" << mres << " is modified to 0x" << res << endl;
        cout << "pc is modified to 0x" << pcres << endl;
    }
}

void Writeback(string rt, int value){
// total_clock_cycles 1 :
// $t3 is modified to 0x10
// pc is modified to 0x4
    total_clock_cycles = total_clock_cycles + 1;

    cout << "\ntotal_clock_cycles " << total_clock_cycles << " :" << endl;
    
    registerfile[toInt(rt)] = value; //value for lw should be 116 here
    //next 3 lines convert simp int value to chad hex result
    stringstream ss;
    ss << hex << registerfile[toInt(rt)];
    string res = ss.str();

    //next 3 lines convert simp int pc value to chad hex pc value
    pc = next_pc;
    stringstream ssp;
    ssp << hex << pc;
    string pcres = ssp.str();

    cout << registerHandler(toInt(rt)) << " is modified to 0x" << res << endl;
    cout << "pc is modified to 0x" << pcres << endl;
}

string ControlUnit(string operation){
    string alu_op; // 0000 is AND, 0001 is OR, 0010 is ADD, 0110 is SUB, 0111 is SLT, 1100 is NOR
    if (toInt(operation) == 35) {
        //if operation is LW
        RegDst = 0, ALUSrc = 1, MemtoReg = 1, RegWrite = 1, MemRead = 1, MemWrite = 0, Branch = 0;
        Jump = 0, InstType = 0;
        alu_op = "0010";
    }else if (toInt(operation) == 34){
        //if operation is SUB
        RegDst = 1, ALUSrc = 0, MemtoReg = 0, RegWrite = 1, MemRead = 0, MemWrite = 0, Branch = 0;
        Jump = 0, InstType = 0;
        alu_op = "0110";
    }else if (toInt(operation) == 42){
        //if operation is SLT
        RegDst = 1, ALUSrc = 0, MemtoReg = 0, RegWrite = 1, MemRead = 0, MemWrite = 0, Branch = 0;
        Jump = 0, InstType = 0;
        alu_op = "0111";
    }else if (toInt(operation) == 4){
        //if operation is BEQ
        //RegDst = X, MemtoReg = X unsure how to implement when it is X
        RegDst = 0, ALUSrc = 0, MemtoReg = 0, RegWrite = 0, MemRead = 0, MemWrite = 0, Branch = 1;
        Jump = 1, InstType = 0;
        alu_op = "0110";
    }else if (toInt(operation) == 43){
        //if operation is SW
        //RegDst = X, MemtoReg = X unsure how to implement when it is X
        RegDst = 0, ALUSrc = 1, MemtoReg = 0, RegWrite = 0, MemRead = 0, MemWrite = 1, Branch = 1;
        Jump = 0, InstType = 0;
        alu_op = "0010";
    }

    return alu_op;
}

string registerHandler(int reg){
    if(reg == 0){
        return "$zero";
    }else if (reg == 1){
        return "$at";
    }else if (reg == 2){
        return "$v0";
    }else if (reg == 3){
        return "$v1";
    }else if (reg == 4){
        return "$a0";
    }else if (reg == 5){
        return "$a1";
    }else if (reg == 6){
        return "$a2";
    }else if (reg == 7){
        return "$a3";
    }else if (reg == 8){
        return "$t0";
    }else if (reg == 9){
        return "$t1";
    }else if (reg == 10){
        return "$t2";
    }else if (reg == 11){
        return "$t3";
    }else if (reg == 12){
        return "$t4";
    }else if (reg == 13){
        return "$t5";
    }else if (reg == 14){
        return "$t6";
    }else if (reg == 15){
        return "$t7";
    }else if (reg == 16){
        return "$s0";
    }else if (reg == 17){
        return "$s1";
    }else if (reg == 18){
        return "$s2";
    }else if (reg == 19){
        return "$s3";
    }else if (reg == 20){
        return "$s4";
    }else if (reg == 21){
        return "$s5";
    }else if (reg == 22){
        return "$s6";
    }else if (reg == 23){
        return "$s7";
    }else if (reg == 24){
        return "$t8";
    }else if (reg == 25){
        return "$t9";
    }else if (reg == 26){
        return "$k0";
    }else if (reg == 27){
        return "$k1";
    }else if (reg == 28){
        return "$gp";
    }else if (reg == 29){
        return "$sp";
    }else if (reg == 30){
        return "$fp";
    }else if (reg == 31){
        return "$ra";
    }else{
        return "Unknown";
    }
}

int toInt(string convertMe){ //reads in char input and converts it to integer
    char* begin = &convertMe[0];
    int total = 0;
    while (*begin)
    {
        total *= 2;
        if (*begin++ == '1') total += 1;
    }

    return total;
}

int main () {
    registerfile[9] = 32; //32    $t1 = 0x20
    registerfile[10] = 5; //5     $t2 = 0x5
    registerfile[16] = 112; //112  $s0 = 0x70
    dmem[28] = 5; //5             0x70 = 0x5
    dmem[29] = 16; //16           0x74 = 0x10

    string instruction;
    int filelinecount = 0;

    ifstream inputFile ("sample_binary.txt");
    while(getline(inputFile,instruction)){
        ++filelinecount;
    }

    ifstream textFile ("sample_binary.txt");
    if (textFile.is_open())
    {
        for (int lineno = 0; getline (textFile,instruction) && lineno < filelinecount; lineno++){ 
            if (lineno == pc/4){
                Fetch(instruction);
            }
        }
        textFile.close();
    }
    else cout << "Unable to open file"; 

    cout << "\nprogram terminated:\ntotal execution time is " << total_clock_cycles << " cycles"; 

    return 0;
}