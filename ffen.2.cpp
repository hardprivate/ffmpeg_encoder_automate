
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>

std::vector<std::string> valid_extensions = {".wmv", ".avi", ".asf", ".flv", ".mkv"};

void convert_video(const std::string& input_path)
{
    const auto last_dot_index = input_path.find_last_of('.');
    std::string output_path = input_path.substr(0, last_dot_index) + ".mp4";

    std::string command = "ffmpeg -i \"" + input_path + "\" -c:v libx264 -c:a aac \"" + output_path + "\"";
    
	//system(command.c_str());
}

void traverse_directory()
{
    DIR* dir;
    dirent* entry;
    const std::string current_directory = ".";

    if ((dir = opendir(current_directory.c_str())) != nullptr)
    {
        while ((entry = readdir(dir)) != nullptr)
        {
            const std::string entry_name = entry->d_name;
            const auto ext_start = entry_name.find_last_of('.');
			
			if (ext_start != std::string::npos)
			{
				const std::string ext = entry_name.substr(ext_start);
				if (std::find(valid_extensions.begin(), valid_extensions.end(), std::string(ext)) != valid_extensions.end())
				{
					std::cout << "Converting video file: " << entry_name << std::endl;
					convert_video(entry_name);
				}
			}

			
			
        }
        closedir(dir);
    }
}

int main()
{
    traverse_directory();
    return 0;
}
