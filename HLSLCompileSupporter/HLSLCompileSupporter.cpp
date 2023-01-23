#include<fstream>
#include <iostream>
#include<vector>
#include<string>
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<cereal/cereal.hpp>
#include<cereal/archives/json.hpp>
#include<cereal/types/map.hpp>
#include<filesystem>
int Compile(const std::string& arg_inputPath, const std::string& arg_outputPath,const char arg_shaderType) {
	std::string line; 
	switch (arg_shaderType)
	{
	case 'v':
	case 'V':
		line = "fxc.exe /T vs_5_1 /E VSMain /Fh ";
		break;
	case 'p':
	case 'P':
		line = "fxc.exe /T ps_5_1 /E PSMain /Fh ";
		break;
	case 'g':
	case 'G':
		line = "fxc.exe /T gs_5_1 /E GSMain /Fh ";
		break;
	default:
		line = "fxc.exe /T vs_5_1 /E VSMain /Fh ";
		break;
	} 
	line += arg_outputPath + " " + arg_inputPath;
	std::system(line.c_str());
	/*
	auto lineChar = const_cast<char*> (line.c_str());
	PROCESS_INFORMATION processInfo{};
	STARTUPINFOA startUpInfo{};
	startUpInfo.cb = sizeof(STARTUPINFO);
	startUpInfo.dwFlags = STARTF_USESTDHANDLES;
	startUpInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	startUpInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);*/
	return 0;// CreateProcessA(nullptr, lineChar, nullptr, nullptr, true, 0, nullptr, nullptr, &startUpInfo, &processInfo);
	
}

std::vector<std::string> Split(const std::string& arg_source, const std::string& cuttingSouece)
{
	auto output = std::vector<std::string>();
	std::int32_t first = 0;
	std::int32_t last = arg_source.find_first_of(cuttingSouece);
	if (last == std::string::npos) {
		output.push_back(arg_source);
		return output;
	}
	while (first < arg_source.size())
	{
		auto subString = arg_source.substr(first, last - first);
		output.push_back(subString);
		first = last + 1;
		last = arg_source.find_first_of(cuttingSouece, first);
		if (last == std::string::npos) {
			last = arg_source.size();
		}
	}
	return output;
}
struct ShaderCompileHistory {
	std::map<std::string, std::int64_t> m_map_lastEdit;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_map_lastEdit);
	}
};

int main(const std::int32_t argCount, const char* args[])
{
	constexpr const char* logFilePath="ShaderCompileLog.txt";
	switch (argCount)
	{
	case 2:
	{
		std::ifstream input(args[1]), log(logFilePath);
		ShaderCompileHistory history;
		if(log.is_open())
		{
			std::stringstream stream;
			stream << log.rdbuf();
			cereal::JSONInputArchive jsonInputARCHIVE_BUTI(stream);
			try
			{
				jsonInputARCHIVE_BUTI(history);
			}
			catch (const std::exception& ex)
			{
				std::cout << "ShaderCompileLog.txt" << "の読み込み中に例外発生" << std::endl;
			}
			stream.clear();
		}

		constexpr std::int32_t buffSize=1024;
		char buffer[buffSize];
		for (;;) {
			input.getline(buffer, buffSize); 
			if (input.bad() || input.eof()) {
				break;
			}
			std::string line = buffer;
			auto args = Split(line, " ");
			if (args.size() != 3) {
				continue;
			}
			std::filesystem::directory_entry inputFileEntry(args[2]),outputFileEntry(args[1]);
			std::int64_t sec = (std::chrono::duration_cast<std::chrono::seconds>(inputFileEntry.last_write_time().time_since_epoch()).count());
			
			if (history.m_map_lastEdit.count(line)&&
				history.m_map_lastEdit.at(line)==sec&&
				outputFileEntry.exists()
				) {

			}
			else {
				Compile(args[2], args[1], args[0][0]);
				if (history.m_map_lastEdit.count(line)) {
					history.m_map_lastEdit.at(line) = sec;
				}
				else {
					history.m_map_lastEdit.emplace(line, sec);
				}
			}
		}

		{
			std::stringstream stream;
			{
				cereal::JSONOutputArchive jsonOutARCHIVE_BUTI(stream);
				jsonOutARCHIVE_BUTI(history);
			}
			std::ofstream outputFile(logFilePath);
			outputFile << stream.str();
			outputFile.close();
			stream.clear();
		}
	}
		break;
	case 4:
	{
		Compile(args[3], args[2], *args[1]);
	}
		break;
	default:
		std::cout << "入力パス 出力パス　を入力してください" << std::endl;
		return 0;
		break;
	}
	std::system("pause");
}
