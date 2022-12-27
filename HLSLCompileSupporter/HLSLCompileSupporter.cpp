#include<fstream>
#include <iostream>
#include<string>
int Compile(const std::string& arg_inputPath,const std::string& arg_outputPath) {

}

int main(const std::int32_t argCount, const char* args[])
{
	switch (argCount)
	{
	case 1:

		break;
	case 2:
		Compile(args[0],args[1]);
		break;
	default:
		std::cout << "入力パス 出力パス　を入力してください" << std::endl;
		return 0;
		break;
	}

}
