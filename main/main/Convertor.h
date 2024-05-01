#pragma once
#include <sstream>
#include <ostream>
#include <istream>
#include <string>
#include <bitset>
#include <iomanip>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm> 
#include <functional> 

using namespace std;

struct Page
{
	int Number_of_page;
	uint8_t frame_number;
	bool check_bit;
};

class Convertor
{
private:
	vector<unsigned char> Binary_Value;
	vector<string> Hex_Value;
	vector<Page> pages;
	fstream Listing;
	int position;

private:
	uint32_t Convert_Virtual_address_to_Physical(uint32_t virtual_adress);
	uint8_t Search_frame_page(uint32_t number_of_page, bool& search_result);
	void MOV_reg_reg();
	void MOV_reg_addr();
	void MUL_reg_reg();
	void MUL_reg_addr();
	void JG_shift();
	void JG_addr();
	void CMP_reg_reg();
	void MOV_reg_lit();


public:
	int Read_Data_From_File(ifstream& input_file);
	int Command_Converting();
	void Get_Pages_info(ifstream& csv_file);
	void Check_Expect(ifstream& expect_file);
	Convertor();
};

