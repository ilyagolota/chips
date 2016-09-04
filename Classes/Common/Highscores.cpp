#include "Highscores.h"
#include <fstream>
#include <Utils/BinaryReader.h>
#include <Utils/MD5.h>

Highscores* Highscores::create(const std::string& filename, const std::string& secret)
{
	auto instance = new Highscores(filename, secret);
	instance->autorelease();
	return instance;
}

Highscores::Highscores(const std::string& filename, const std::string& secret)
{
	_filename = filename;
	_secret = secret;
	_load();
}

Highscores::~Highscores()
{ }

HighscoreRecord Highscores::get(size_t packIndex, size_t levelIndex)
{
	if (packIndex < _recordPacks.size())
	{
		auto& records = _recordPacks[packIndex];
		if (levelIndex < records.size())
		{
			return records[levelIndex];
		}
	}

	HighscoreRecord zeroRecord;
	return zeroRecord;
}

void Highscores::set(size_t packIndex, size_t levelIndex, const HighscoreRecord& record)
{
	if (packIndex >= _recordPacks.size())
	{
		_recordPacks.reserve(packIndex);
		for (size_t i = _recordPacks.size(); i <= packIndex; i++)
		{
			_recordPacks.push_back(std::move(std::vector<HighscoreRecord>()));
		}
	}

	auto& records = _recordPacks[packIndex];
	if (levelIndex >= records.size())
	{
		records.reserve(levelIndex);
		for (size_t i = records.size(); i <= levelIndex; i++)
		{
			records.push_back(HighscoreRecord());
		}
	}

	records[levelIndex] = record;

	_save();
}

void Highscores::_load()
{
	_recordPacks.clear();

	auto fs = cocos2d::FileUtils::getInstance();
	auto fullpath = fs->getWritablePath() + _filename;
	if (fs->isFileExist(fullpath))
	{
		std::fstream stream;
		stream.open(fullpath, std::ios::in);
		if (stream)
		{
			utils::BinaryReader reader(&stream);

			short signature = reader.readInt16();
			CC_UNUSED_PARAM(signature);

			short version = reader.readInt16();
			CC_UNUSED_PARAM(version);

			std::vector<char> storedHash;
			storedHash.resize(16);
			reader.read(&storedHash[0], 16);
			
			size_t position = reader.getBaseStream().tellg();
			
			utils::MD5 md5;
			md5.update(&reader.getBaseStream());
			md5.update(_secret);
			std::vector<char> computedHash = md5.getDigest();

			if (computedHash == storedHash)
			{
				reader.getBaseStream().seekg(position, std::ios::beg);

				int packCount = reader.readInt32();
				for (int i = 0; i < packCount; i++)
				{
					int levelCount = reader.readInt32();

					std::vector<HighscoreRecord> records;
					records.reserve(levelCount);

					for (int j = 0; j < levelCount; i++)
					{
						HighscoreRecord record;
						record.minTime = reader.readInt16();
						record.minTryCount = reader.readInt16();
						record.maxScore = reader.readInt32();
						record.currentTryCount = reader.readInt16();
						records.push_back(record);
					}

					_recordPacks.push_back(std::move(records));
				}
			}
		}
		stream.close();
	}
}

void Highscores::_save()
{


	//unsigned int signature = _reader.readUInt32();
	//18     CC_UNUSED_PARAM(signature);

}
