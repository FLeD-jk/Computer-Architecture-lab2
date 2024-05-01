#include "Testing.h"


void Testing(string Input_File,  string csv_File ,string expect_File)
{
    ifstream input_file(Input_File);
    ifstream csv_file(csv_File);
    ifstream expect_file(expect_File);
    Convertor lab2;
    lab2.Get_Pages_info(csv_file);
    lab2.Read_Data_From_File(input_file);
    lab2.Command_Converting();
    cout << "Conversion is completely successful" << endl;
    lab2.Check_Expect(expect_file);


}
