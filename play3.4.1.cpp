#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

bool file_exists(const std::string& filepath)
{
	struct stat buffer;
	return (stat(filepath.c_str(), &buffer) == 0);
}

double get_video_duration(const std::string& filepath)
{
	char buffer[128];
	std::string command = "ffprobe -v error -show_entries format=duration -of default=nw=1:nk=1 -i \"" + filepath + "\"";
	std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	double video_duration = 0.0;
	if (fgets(buffer, 128, pipe.get()) != nullptr) {
		video_duration = std::stod(buffer);
	}
	return video_duration;
}

std::string format_duration(double duration)
{
	int hours = static_cast<int>(duration / 3600);
	duration -= hours * 3600;
	int minutes = static_cast<int>(duration / 60);
	duration -= minutes * 60;
	int seconds = static_cast<int>(duration);

	std::ostringstream formatted_duration;
	formatted_duration << std::setfill('0') << std::setw(2) << hours << "\\:"
	                   << std::setw(2) << minutes << "\\:"
	                   << std::setw(2) << seconds;

	return formatted_duration.str();
}

void play_video(const std::string &filepath)
{
	double video_duration = get_video_duration(filepath);
	std::string formatted_duration = format_duration(video_duration);

	std::string command = "ffplay -i \"" + filepath + "\""
	                      " -vf \"scale='min(1280,iw)':min'(850,ih)':force_original_aspect_ratio=decrease,drawtext=text='%{pts\\:gmtime\\:0\\:%H\\\\\\:%M\\\\\\:%S}/" + formatted_duration +
	                      "':box=1:fontcolor=black:shadowcolor=white:shadowx=1:shadowy=1:fontsize=16:x=(w-tw)-(lh):y=h-(2*lh)\""
	                      " -autoexit -stats ";

	system(command.c_str());
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <video-file>" << std::endl;
		return 1;
	}

	std::string filepath = argv[1];
	if (!file_exists(filepath)) {
		std::cerr << "Error: The provided file does not exist." << std::endl;
		return 1;
	}

	try {
		play_video(filepath);
	} catch (const std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
