#include "HighscoreStorage.h"
#include <fstream>
#include <utils/BinaryReader.h>
#include <Utils/MD5.h>

HighscoreStorage* HighscoreStorage::create(const std::string& filename, const std::vector<char>& secret)
{
	auto instance = new HighscoreStorage(filename, secret);
	instance->autorelease();
	return instance;
}

HighscoreStorage::HighscoreStorage(const std::string& filename, const std::vector<char>& secret)
{
	_filename = filename;
	_secret = secret;
	load();
}

const HighscoreRecord& HighscoreStorage::getRecord(size_t packIndex, size_t levelIndex)
{
	if (packIndex < _recordPacks.size())
	{
		auto& records = _recordPacks[packIndex];
		if (levelIndex < records.size())
		{
			return records[levelIndex];
		}
	}

	static HighscoreRecord zeroRecord;
	return zeroRecord;
}

void HighscoreStorage::saveRecord(size_t packIndex, size_t levelIndex, const HighscoreRecord& record)
{
	if (packIndex >= _recordPacks.size())
	{
		_recordPacks.reserve(packIndex + 1);
		for (size_t i = _recordPacks.size(); i <= packIndex; i++)
		{
			_recordPacks.push_back(std::move(std::vector<HighscoreRecord>()));
		}
	}

	auto& records = _recordPacks[packIndex];
	if (levelIndex >= records.size())
	{
		records.reserve(levelIndex + 1);
		for (size_t i = records.size(); i <= levelIndex; i++)
		{
			records.push_back(HighscoreRecord());
		}
	}

	records[levelIndex] = record;

	save();
}

void HighscoreStorage::load()
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
			md5.update(reader.getBaseStream());
			md5.update(_secret);
			std::vector<char> computedHash = md5.getDigest();

			if (computedHash == storedHash)
			{
				reader.getBaseStream().seekg(position, std::ios::beg);

				int packCount = reader.readInt32();
                _recordPacks.reserve(packCount);
                
				for (int i = 0; i < packCount; i++)
				{
					int levelCount = reader.readInt32();

					std::vector<HighscoreRecord> records;
					records.reserve(levelCount);

					for (int j = 0; j < levelCount; i++)
					{
						HighscoreRecord record;
                        record.completed = (reader.readInt8() != 0);
                        record.tried = (reader.readInt8() != 0);
						record.minTime = reader.readInt16();
						record.minDeathCount = reader.readInt16();
						record.maxScore = reader.readInt32();
						record.deathCount = reader.readInt16();
						records.push_back(record);
					}

					_recordPacks.push_back(std::move(records));
				}
			}
		}
		stream.close();
	}
}

void HighscoreStorage::save()
{
    std::stringstream stream;
    
    int packCount = _recordPacks.size();
    stream.write((const char*)&packCount, 4);
    
    for (auto& records : _recordPacks)
    {
        int recordCount = records.size();
        stream.write((const char*)&recordCount, 4);
        
        for (auto& record : records)
        {
            stream.write((const char*)&record.completed, 1);
            stream.write((const char*)&record.tried, 1);
            stream.write((const char*)&record.minTime, 2);
            stream.write((const char*)&record.minDeathCount, 2);
            stream.write((const char*)&record.maxScore, 4);
            stream.write((const char*)&record.deathCount, 2);
        }
    }
    
    utils::MD5 md5;
    md5.update(stream);
    md5.update(_secret);
    std::vector<char> computedHash = md5.getDigest();
    
    short version = 1;
    
    auto fs = cocos2d::FileUtils::getInstance();
    auto fullpath = fs->getWritablePath() + _filename;
    
    std::fstream fileStream;
    fileStream.open(fullpath, std::ios::out);
    
    fileStream.write("hs", 2);
    fileStream.write((const char*)&version, 2);
    fileStream.write(computedHash.data(), computedHash.size());
    
    stream.seekg(0, std::ios::beg);
    fileStream << stream.rdbuf();
    
    fileStream.close();
}
