#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle07A
{
	struct File
	{
		std::string path;
		int size = 0;
	};

	struct Directory
	{
		std::string path;
		int size = -1;
		std::unordered_set<std::string> childDirectories;
		std::unordered_set<std::string> childFiles;
	};

	auto allDirectories = std::unordered_map<std::string, Directory>{};
	auto allFiles = std::unordered_map<std::string, File>{};

	void ReadInput()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle07.input");

		allDirectories["/"] = Directory{ "/" };
		auto pwd = std::vector<std::string>{ {"/"} };

		for (const auto& line : input)
		{
			auto tokens = Utilities::SplitString(line, " ");
			if (tokens[0] == "$")
			{
				if (tokens[1] == "ls")
				{
					continue;
				}
				else
				{
					Utilities::VerifyElseCrash(tokens[1] == "cd");
					const auto& target = tokens[2];
					if (target == "/")
					{
						pwd.clear();
						pwd.emplace_back("/");
					}
					else if (target == "..")
					{
						pwd.pop_back();
					}
					else
					{
						pwd.emplace_back(pwd.back() + target + "/");
					}
				}
			}
			else
			{
				auto& parent = allDirectories.at(pwd.back());
				if (tokens[0] == "dir")
				{
					auto path = pwd.back() + tokens[1] + "/";
					if (!allDirectories.contains(path))
					{
						allDirectories[path] = Directory{ path };
					}

					parent.childDirectories.insert(path);
				}
				else
				{
					auto path = pwd.back() + tokens[1];
					if (!allFiles.contains(path))
					{
						allFiles[path] = File{ path, std::stoi(tokens[0]) };
					}

					parent.childFiles.insert(path);
				}
			}
		}
	}

	int CalculateDirectorySize(Directory& directory)
	{
		if (directory.size >= 0)
		{
			return directory.size;
		}

		auto totalSizeOfFiles = ranges::accumulate(directory.childFiles, 0,
			[](int sum, const std::string& path) { return sum + allFiles.at(path).size; });

		directory.size = ranges::accumulate(directory.childDirectories, totalSizeOfFiles,
			[](int sum, const std::string& path) { return sum + CalculateDirectorySize(allDirectories.at(path)); });
		return directory.size;
	}

	void PrintSolution()
	{
		ReadInput();
		CalculateDirectorySize(allDirectories.at("/"));

		auto isSmallDir = [](const Directory& directory){ return directory.size <= 100000; };
		auto totalDirSize = ranges::accumulate(
			allDirectories | std::ranges::views::values | std::ranges::views::filter(isSmallDir),
			0,
			[](int sum, const Directory& directory) { return sum + directory.size; });

		std::cout << totalDirSize;
	}
}