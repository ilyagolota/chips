#include "HighscoreStorage.h"
#include <fstream>
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
            auto buf = stream.rdbuf();
            
			short signature;
            buf->sgetn((char*)&signature, 2);
			CC_UNUSED_PARAM(signature);

			short version;
            buf->sgetn((char*)&version, 2);
			CC_UNUSED_PARAM(version);

			std::vector<char> storedHash;
			storedHash.resize(16);
			buf->sgetn(&storedHash[0], 16);
			
            size_t position = buf->pubseekoff(0, std::ios::cur);
			
			utils::MD5 md5;
			md5.update(stream);
			md5.update(_secret);
			std::vector<char> computedHash = md5.getDigest();

			if (computedHash == storedHash)
			{
                buf->pubseekoff(position, std::ios::beg);

                int packCount;
                buf->sgetn((char*)&packCount, 4);
                _recordPacks.reserve(packCount);
                
				for (int i = 0; i < packCount; i++)
				{
                    int levelCount;
                    buf->sgetn((char*)&levelCount, 4);

					std::vector<HighscoreRecord> records;
					records.reserve(levelCount);

					for (int j = 0; j < levelCount; j++)
					{
						HighscoreRecord record;
                        buf->sgetn((char*)&record.completed, 1);
                        buf->sgetn((char*)&record.tried, 1);
						buf->sgetn((char*)&record.minTime, 2);
						buf->sgetn((char*)&record.minDeathCount, 2);
						buf->sgetn((char*)&record.maxScore, 4);
						buf->sgetn((char*)&record.deathCount, 2);
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
    
    auto buf = stream.rdbuf();
    
    int packCount = _recordPacks.size();
    buf->sputn((const char*)&packCount, 4);
    
    for (auto& records : _recordPacks)
    {
        int recordCount = records.size();
        buf->sputn((const char*)&recordCount, 4);
        
        for (auto& record : records)
        {
            buf->sputn((const char*)&record.completed, 1);
            buf->sputn((const char*)&record.tried, 1);
            buf->sputn((const char*)&record.minTime, 2);
            buf->sputn((const char*)&record.minDeathCount, 2);
            buf->sputn((const char*)&record.maxScore, 4);
            buf->sputn((const char*)&record.deathCount, 2);
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
