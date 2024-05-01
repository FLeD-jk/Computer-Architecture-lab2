
#include "Convertor.h"

Convertor::Convertor():position(0)
{
    Listing.open("output.txt", ios::out);
    if (!Listing.is_open()) {
        cerr << "Error opening output file" << endl;
        exit(-1);
    }
}

int Convertor::Read_Data_From_File(ifstream& input_file) {

    string line;

    if (!input_file.is_open()) {
        cerr << "Error opening input file\n";
        return -1;
    }



    while (getline(input_file, line)) {
        istringstream iss(line);
        string hexString;
        while (iss >> hexString) {
            unsigned long hexValue = stoul(hexString, nullptr, 16);
           // bitset<32> binary(hexValue);
           // output_file << binary << " ";
            Binary_Value.push_back(static_cast<unsigned char>(hexValue));
            Hex_Value.push_back(hexString);
        }
       // output_file << endl;
    }

    input_file.close();

    
/*
    ifstream input_output_file("test_output.txt");
    if (!input_output_file.is_open()) {
        cerr << "Error opening output file for reading\n";
        return -1;
    }

    cout << "Contents of the output file:\n";
    while (getline(input_output_file, line)) {
        cout << line << endl;
    }

    input_output_file.close();
*/
    cout << "File copied successfully." << endl << endl;

    return 1;
}

int Convertor::Command_Converting() {

 
    while (position < Hex_Value.size()) {

        if (Hex_Value[position] == "1A") {
            MOV_reg_reg();
        }
        else if (Hex_Value[position] == "1B") {
            MOV_reg_addr();
        }
        else if (Hex_Value[position] == "20") {
            MUL_reg_reg();
        }
        else if (Hex_Value[position] == "22") {
            MUL_reg_addr();
        }
        else if (Hex_Value[position] == "94") {
            JG_shift();
        }
        else if (Hex_Value[position] == "95") {
            JG_addr();
        }
        else if (Hex_Value[position] == "80") {
            CMP_reg_reg();
        }
        else if (Hex_Value[position] == "1C") {
            MOV_reg_lit();
        }
        else {
            cerr << "ERR0R: Incorrect command" << endl;
            exit(-1);
        }

    }
    Listing.close();
        return 0;

}

void Convertor::MOV_reg_reg() {
    uint8_t regs = 0, reg1 = 0, reg2 = 0;
    position++;
    if (Hex_Value[position].size() != 2) {
        cerr << "ERR0R: Incorrect byte assignment for '1A'." << endl;
        exit(-1);
    }
    else {
        regs = Binary_Value[position];
        reg1 = regs >> 4;
        reg2 = regs & 0b00011111;
        cout << "1A " << uppercase << hex << setfill('0') << setw(2) << (int)regs << ":" << endl;
        cout << "MOV R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl << endl;
        Listing << "MOV R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl;
        position++;
    }
}

void Convertor::MOV_reg_addr() {
    uint8_t  reg = 0;
    uint32_t virtual_adress = 0, physical_adress = 0;
    position++;
    uint8_t operands[5];
    for (int i = 0; i < 5; i++) {
        if (Hex_Value[position].size() != 2) {
            cerr << "ERR0R: Incorrect byte assignment for '1B'." << endl;
            exit(-1);
        }
        else {
            operands[i] = Binary_Value[position];
            position++;
        }
    }
    if ((operands[0] >> 4) != 0) {
        cerr << "ERR0R: Incorrect command variation for '1B'." << endl;
        exit(-1);
    }
    else reg = operands[0] & 0b00011111;

    for (int i = 1; i < 5; i++) {
        virtual_adress = (virtual_adress << 8) | operands[i];
    }

    physical_adress = Convert_Virtual_address_to_Physical(virtual_adress);
    cout << "1B ";
    for (int i = 0; i < 5; i++)
        cout << uppercase << hex << setfill('0') << setw(2) << (int)operands[i] << " ";
   
    cout << ":" << endl << "MOV R" << uppercase << hex << (int)reg << ", [0x" << physical_adress << "]" << endl << endl;
    Listing << "MOV R" << uppercase << hex << (int)reg << ", [0x" << physical_adress << "]" << endl << endl;

}

void Convertor::MUL_reg_reg() {
    uint8_t regs = 0, reg1 = 0, reg2 = 0;
    position++;
    if (Hex_Value[position].size() != 2) {
        cerr << "ERR0R: Incorrect byte assignment for '20'." << endl;
        exit(-1);
    }
    else {
        regs = Binary_Value[position];
        reg1 = regs >> 4;
        reg2 = regs & 0b00011111;
        cout << "20 " << uppercase << hex << setfill('0') << setw(2) << (int)regs << ":" << endl;
        cout << "MUL R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl << endl;
        Listing << "MUL R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl;
        position++;
    }
}

void Convertor::MUL_reg_addr() {
    uint8_t  reg = 0;
    uint32_t virtual_adress = 0, physical_adress = 0;
    position++;
    uint8_t operands[5];
    for (int i = 0; i < 5; i++) {
        if (Hex_Value[position].size() != 2) {
            cerr << "ERR0R: Incorrect byte assignment for '22'." << endl;
            exit(-1);
        }
        else {
            operands[i] = Binary_Value[position];
            position++;
        }
    }
    if ((operands[0] >> 4) != 0) {
        cerr << "ERR0R: Incorrect command variation for '22'." << endl;
        exit(-1);
    }
    else reg = operands[0] & 0b00011111;
    for (int i = 1; i < 5; i++) {
        virtual_adress = (virtual_adress << 8) | operands[i];
    }

    physical_adress = Convert_Virtual_address_to_Physical(virtual_adress);
    cout << "22 ";
    for (int i = 0; i < 5; i++)
        cout << uppercase << hex << setfill('0') << setw(2) << (int)operands[i] << " ";

    cout << ":" << endl << "MUL R" << uppercase << hex << (int)reg << ", [0x" << physical_adress << "]" << endl << endl;
    Listing << "MUL R" << uppercase << hex << (int)reg << ", [0x" << physical_adress << "]" << endl;
}

void Convertor::JG_shift() {
    uint8_t shift = 0;
    position++;
    if (Hex_Value[position].size() != 2) {
        cerr << "ERR0R: Incorrect byte assignment for '94'." << endl;
        exit(-1);
    }
    else {
        shift = Binary_Value[position];
        cout << "94 " << uppercase << hex << setfill('0') << setw(2) << (int)shift << ":" << endl;
        cout << "JG [0x" << uppercase << hex << (int)shift << "]" << endl << endl;
        Listing << "JG [0x" << uppercase << hex << (int)shift << "]" << endl;
        position++;
    }
}

void Convertor::JG_addr() {
    uint32_t virtual_adress = 0, physical_adress = 0;
    position++;
    uint8_t operands[4];
    for (int i = 0; i < 4; i++) {
        if (Hex_Value[position].size() != 2) {
            cerr << "ERR0R: Incorrect byte assignment for '95'." << endl;
            exit(-1);
        }
        else {
            operands[i] = Binary_Value[position];
            position++;
        }
    }
    for (int i = 0; i < 4; i++) {
        virtual_adress = (virtual_adress << 8) | operands[i];
    }

    physical_adress = Convert_Virtual_address_to_Physical(virtual_adress);
    cout << "95 ";
    for (int i = 0; i < 4; i++)
        cout << uppercase << hex << setfill('0') << setw(2) << (int)operands[i] << " ";

    cout << ":" << endl << "JG " << uppercase << hex << "[0x" << physical_adress << "]" << endl << endl;
    Listing << "JG " << uppercase << hex << "[0x" << physical_adress << "]" << endl;
}

void Convertor::CMP_reg_reg() {
    uint8_t regs = 0, reg1 = 0, reg2 = 0;
    position++;
    if (Hex_Value[position].size() != 2) {
        cerr << "ERR0R: Incorrect byte assignment for '80'." << endl;
        exit(-1);
    }
    else {
        regs = Binary_Value[position];
        reg1 = regs >> 4;
        reg2 = regs & 0b00011111;
        cout << "80 " << uppercase << hex << setfill('0') << setw(2) << (int)regs << ":" << endl;
        cout << "CMP R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl << endl;
        Listing << "CMP R" << uppercase << hex << (int)reg1 << ", R" << (int)reg2 << endl;
        position++;
    }
}

void Convertor::MOV_reg_lit() {
    uint8_t  reg = 0, tmpreg = 0;
    char lit32 = 0;
    uint8_t lit8 = 0;
    position++;
    if (Hex_Value[position].size() != 2) {
        cerr << "ERR0R: Incorrect byte assignment for '1C'." << endl;
        exit(-1);
    }
    else {
        tmpreg = Binary_Value[position];
        if ((tmpreg >> 4) == 0) {
            reg = tmpreg & 0b00011111;
            position++;
            if (Hex_Value[position].size() != 2) {
                cerr << "Incorrect constant length for '1C 0'." << endl;
                exit(-1);
            }
            lit8 = Binary_Value[position];
            cout << "1C " << uppercase << hex << setfill('0') << setw(2) << (int)tmpreg << " " << (int)lit8 << ":" << endl;
            cout << "MOV R" << uppercase << hex << (int)reg << ", " << setfill('0') << setw(2) << (int)lit8 << endl << endl;
            Listing << "MOV R" << uppercase << hex << (int)reg << ", " << setfill('0') << setw(2) << (int)lit8 << endl;
            position++;
        }
        else if ((tmpreg >> 4) == 2) {
            reg = tmpreg & 15;
            position++;
            if (Hex_Value[position].size() != 8) {
                cerr << "ERR0R: Incorrect constant length for '1C 2'." << endl;
                exit(-1);
            }
            lit32 = Binary_Value[position];
            cout << "1C " << uppercase << hex << setfill('0') << setw(2) << (int)tmpreg << " " << (int)lit32 << ":" << endl;
            cout << "MOV R" << uppercase << hex << (int)reg << ", " << setfill('0') << setw(2) << (int)lit32 << endl << endl;
            Listing << "MOV R" << uppercase << hex << (int)reg << ", " << setfill('0') << setw(2) << (int)lit32 << endl;
            position++;
        }
        else {
            cerr << "ERR0R: Incorrect command variation for '1C'.";
            exit(-1);
        }

    }

}


void Convertor::Get_Pages_info(ifstream& csv_file)
{
    string line;
    while (getline(csv_file, line))
    {
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        Page page;
        
        page.Number_of_page = stoi(tokens[0] , nullptr , 10);

        if (page.Number_of_page <= 0xFFFFF) {

            page.check_bit = stoi(tokens[1], nullptr, 2);

            if (page.check_bit)
            {
                page.frame_number = stoi(tokens[2], nullptr, 2);
            }
            else {
                page.frame_number = 0;
                
            }

            pages.push_back(page);
        }
        else {
            cerr << "ERR0R: Incorrect length of page number" << endl;
            exit(-1);
        }
          
       
    }
}


uint32_t Convertor::Convert_Virtual_address_to_Physical(uint32_t virtual_adress) {
    uint32_t number_of_page = virtual_adress >> 12;
    uint32_t page_offset = (virtual_adress << 20) >> 20;

    bool search_result = false;
    uint8_t frame_number = Search_frame_page(number_of_page, search_result);

    if (search_result == false) {
        std::cerr << "ERROR: Failed to convert virtual address [0x" << uppercase << hex << setfill('0') << virtual_adress << "] because page " << dec << number_of_page << " doesn't exist.";
        exit(-1);
    }

    uint32_t physical_address = (frame_number << 12) | page_offset;
    return physical_address;
}

uint8_t Convertor::Search_frame_page(uint32_t number_of_page, bool& search_result) {
    for (const auto& page : pages) {
        if (page.Number_of_page == number_of_page && page.check_bit) {
            search_result = true;
            return page.frame_number;
        }
    }
    search_result = false;
}


void Convertor::Check_Expect(ifstream& expect_file) {

    ifstream Listing("output.txt");

    if (!Listing.is_open()) {
        cerr << "Error opening Listing file" << endl;
        return;
    }

    if (!expect_file.is_open()) {
        cerr << "Error opening expect file" << endl;
        Listing.close(); 
        return;
    }

    string value1, value2;

    while (Listing >> value1 && expect_file >> value2) {


        if (value1 != value2) {
            cout << "Files do not match." << endl;
            cout << "Expectation: " << value2 << endl;
            cout << "Reality: " << value1 << endl;
            Listing.close();
            expect_file.close();
            return;
        }
    }

    if ((Listing >> value1) || (expect_file >> value2)) {
        cout << "Files do not match." << endl;
    }
    else {
        cout << "Files match." << endl;
    }


    Listing.close();
    expect_file.close();
}