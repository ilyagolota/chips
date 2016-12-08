#include <fstream>

#include "LevelPack.h"
#include <Utils/MD5.h>

LevelPack* LevelPack::create(const std::string& filename, const std::vector<char>& hash)
{
    auto instance = new LevelPack(filename, hash);
    instance->autorelease();
    return instance;
}

LevelPack::LevelPack(const std::string& filename, const std::vector<char>& hash)
{
    _filename = filename;
    _hash = hash;
    
    auto path = cocos2d::FileUtils::getInstance()->fullPathForFilename(_filename);
    std::fstream stream(path, std::ios::binary | std::ios::in);
    
    auto buf = stream.rdbuf();
    
    int signature;
    buf->sgetn((char*)&signature, 4);
    CC_UNUSED_PARAM(signature);
    
    short totalLevelCount;
    buf->sgetn((char*)&totalLevelCount, 2);
    
    _entries.reserve(totalLevelCount);
    for (int levelIndex = 0; levelIndex < totalLevelCount; levelIndex++)
    {
        LevelPackEntry entry;
        
        entry.size = 0;
        buf->sgetn((char*)&entry.size, 2);
        
        entry.offset = buf->pubseekoff(0, std::ios::cur);
        
        struct
        {
            short int number;
            short int time;
            short int chipsRequired;
            short int fieldCount;
        } params;
        buf->sgetn((char*)&params, 8);
        
        const int LAYER_COUNT = 2;
        for (int j = 0; j < LAYER_COUNT; j++)
        {
            unsigned short layerSize;
            buf->sgetn((char*)&layerSize, 2);
            buf->pubseekoff(layerSize, std::ios::cur);
        }
        
        while (buf->pubseekoff(0, std::ios::cur) < entry.offset + entry.size)
        {
            unsigned char fieldType;
            buf->sgetn((char*)&fieldType, 1);
            
            unsigned char fieldSize;
            buf->sgetn((char*)&fieldSize, 1);
            
            if (fieldType != 3)
            {
                buf->pubseekoff(fieldSize, std::ios::cur);
            }
            else
            {
                std::string title(fieldSize, '\0');
                buf->sgetn((char*)&title[0], fieldSize);
                entry.title = title;
                break;
            }
        }
        
        buf->pubseekoff(entry.offset + entry.size, std::ios::beg);
        _entries.push_back(std::move(entry));
    }
    
    stream.close();
}

const std::vector<LevelPackEntry>& LevelPack::getEntries() const
{
    return _entries;
}

LevelConfig* LevelPack::readLevelConfig(size_t entryIndex)
{
    auto path = cocos2d::FileUtils::getInstance()->fullPathForFilename(_filename);
    std::fstream stream(path, std::ios::binary | std::ios::in);
    std::istream::sentry sen(stream);
    if (!sen)
    {
        throw std::runtime_error("Level pack is corrupted");
    }
    
    utils::MD5 md5;
    md5.update(stream);
    
    if (md5.getDigest() != _hash)
    {
        throw std::runtime_error("Level pack is corrupted");
    }
    
    auto& levelEntry = _entries[entryIndex];
    
    auto buf = stream.rdbuf();
    buf->pubseekoff(levelEntry.offset, std::ios::beg);
    
    auto* levelConfig = LevelConfig::create();
    
    short number;
    buf->sgetn((char*)&number, 2);
    levelConfig->setNumber(number);
    
    short time;
    buf->sgetn((char*)&time, 2);
    levelConfig->setTime(time);
    
    short chipsRequired;
    buf->sgetn((char*)&chipsRequired, 2);
    levelConfig->setChipsRequired(chipsRequired);
    
    short fieldCount;
    buf->sgetn((char*)&fieldCount, 2);
    CC_UNUSED_PARAM(fieldCount);
    
    const int LAYER_COUNT = 2;
    for (int fieldIndex = 0; fieldIndex < LAYER_COUNT; fieldIndex++)
    {
        std::vector<TileType> layer(LevelConfig::LAYER_SIZE);
        
        short size;
        buf->sgetn((char*)&size, 2);
        
        for (short i = 0, k = 0; i < size; i++)
        {
            unsigned char byte1;
            buf->sgetn((char*)&byte1, 1);
            
            if (byte1 != 255)
            {
                layer[k] = static_cast<TileType>(byte1);
                k++;
            }
            else
            {
                unsigned char count;
                buf->sgetn((char*)&count, 1);
                
                unsigned char byte2;
                buf->sgetn((char*)&byte2, 1);
                
                for (unsigned char j = 0; j < count; j++)
                {
                    layer[k] = static_cast<TileType>(byte2);
                    k++;
                }
                i += 2;
            }
        }
        levelConfig->getLayers().push_back(std::move(layer));
    }
    
    std::streamsize position = buf->pubseekoff(0, std::ios::cur);
    std::streamsize end = 0;
    buf->sgetn((char*)&end, 2);
    end += position;
    
    while (position < end)
    {
        unsigned char fieldType;
        buf->sgetn((char*)&fieldType, 1);
        
        unsigned char fieldSize;
        buf->sgetn((char*)&fieldSize, 1);
        
        if (fieldType == 1)
        {
            short time;
            buf->sgetn((char*)&time, 2);
            levelConfig->setTime(time);
        }
        else if (fieldType == 2)
        {
            short chipsRequired;
            buf->sgetn((char*)&chipsRequired, 2);
            levelConfig->setChipsRequired(chipsRequired);
        }
        else if (fieldType == 3)
        {
            std::string title(fieldSize - 1, '\0');
            buf->sgetn(&title[0], fieldSize);
            levelConfig->setTitle(title);
        }
        else if (fieldType == 4)
        {
            int trapCount = fieldSize / 10;
            for (int i = 0; i < trapCount; i++)
            {
                short wireData[5];
                buf->sgetn((char*)wireData, fieldSize);
                
                LevelWire wire;
                wire.startCoordinate = cocos2d::Vec2(wireData[0], wireData[1]);
                wire.endCoordinate = cocos2d::Vec2(wireData[2], wireData[3]);
                levelConfig->getWires().push_back(wire);
            }
        }
        else if (fieldType == 5)
        {
            int clonerCount = fieldSize / 8;
            for (int i = 0; i < clonerCount; i++)
            {
                short wireData[4];
                buf->sgetn((char*)wireData, fieldSize);
                
                LevelWire wire;
                wire.startCoordinate = cocos2d::Vec2(wireData[0], wireData[1]);
                wire.endCoordinate = cocos2d::Vec2(wireData[2], wireData[3]);
                levelConfig->getWires().push_back(wire);
            }
        }
        else if (fieldType == 6)
        {
            std::string password(fieldSize - 1, '\0');
            buf->sgetn(&password[0], fieldSize);
            levelConfig->setPassword(password);
        }
        else if (fieldType == 7)
        {
            std::string hint(fieldSize - 1, '\0');
            buf->sgetn(&hint[0], fieldSize);
            levelConfig->setHint(hint);
        }
        else if (fieldType == 10)
        {
            /*var creatureCount = fieldSize / 2;
             var x = input.getUint8();
             var y = input.getUint8();*/
        }
        
        position += fieldSize + sizeof(char) + sizeof(char);
        buf->pubseekoff(position, std::ios::beg);
    }
    
    stream.close();
    
    return levelConfig;
}
