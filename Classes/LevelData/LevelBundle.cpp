#include <fstream>
#include <Utils/BinaryReader.h>
#include <Utils/MD5.h>
#include "LevelBundle.h"

LevelBundle* LevelBundle::create()
{
    auto instance = new LevelBundle();
    instance->autorelease();
    return instance;
}

void LevelBundle::preloadLevelPack(const std::string& filename, const std::vector<char>& hash)
{
    auto path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
    std::fstream stream(path, std::ios::binary | std::ios::in);
    utils::BinaryReader reader(&stream);
    
    int signature = reader.readInt32();
    CC_UNUSED_PARAM(signature);
    
    size_t totalLevelCount = static_cast<size_t>(reader.readInt16());
    
    LevelPack levelPack;
    levelPack.filename = filename;
    levelPack.hash = hash;
    levelPack.levelInfo.reserve(totalLevelCount);
    
    for (int levelIndex = 0; levelIndex < totalLevelCount; levelIndex++)
    {
        LevelInfo levelInfo;
        levelInfo.size = reader.readUInt16();
        levelInfo.offset = reader.getBaseStream().tellg();
        
        struct
        {
            short int number;
            short int time;
            short int chipsRequired;
            short int fieldCount;
        } params;
        reader.read((char *)&params, sizeof params);
        
        const int LAYER_COUNT = 2;
        for (int j = 0; j < LAYER_COUNT; j++)
        {
            int layerSize = reader.readUInt16();
            reader.getBaseStream().seekg(layerSize, std::ios::cur);
        }
        
        while (reader.getBaseStream().tellg() < levelInfo.offset + levelInfo.size)
        {
            unsigned int fieldType = reader.readUInt8();
            unsigned int fieldSize = reader.readUInt8();
            
            if (fieldType != 3)
            {
                reader.getBaseStream().seekg(fieldSize, std::ios::cur);
            }
            else
            {
                std::string title(fieldSize, '\0');
                reader.read(&title[0], fieldSize);
                levelInfo.title = title;
                break;
            }
        }
        
        reader.getBaseStream().seekg(levelInfo.offset + levelInfo.size, std::ios::beg);
        levelPack.levelInfo.push_back(std::move(levelInfo));
    }
    
    _levelPacks.push_back(std::move(levelPack));
    
    stream.close();
}

LevelData* LevelBundle::readLevelData(size_t packIndex, size_t index)
{
    auto& levelPack = _levelPacks[packIndex];
    
    auto path = cocos2d::FileUtils::getInstance()->fullPathForFilename(levelPack.filename);
    std::fstream stream(path, std::ios::binary | std::ios::in);
    
    utils::MD5 md5;
    md5.update(&stream);
    
    if (md5.getDigest() != levelPack.hash)
    {
        throw std::runtime_error("Level pack is corrupted");
    }
    
    auto& levelInfo = levelPack.levelInfo[index];
    stream.seekg(levelInfo.offset, std::ios::beg);
    
    utils::BinaryReader reader(&stream);
    auto* level = LevelData::create();
    
    level->setNumber(reader.readUInt16());
    level->setTime(reader.readUInt16());
    level->setChipsRequired(reader.readUInt16());
    
    int fieldCount = reader.readUInt16();
    CC_UNUSED_PARAM(fieldCount);
    
    for (int fieldIndex = 0; fieldIndex < fieldCount; fieldIndex++)
    {
        std::vector<TileType> layer(LevelData::LAYER_SIZE);
        
        short size = reader.readInt16();
        for (short i = 0, k = 0; i < size; i++)
        {
            unsigned char byte1 = reader.readUInt8();
            if (byte1 != 255)
            {
                layer[k] = static_cast<TileType>(byte1);
                k++;
            }
            else
            {
                unsigned char count = reader.readUInt8();
                unsigned char byte2 = reader.readUInt8();
                for (unsigned char j = 0; j < count; j++)
                {
                    layer[k] = static_cast<TileType>(byte2);
                    k++;
                }
                i += 2;
            }
        }
    }
    
    size_t position = reader.getBaseStream().tellg();
    size_t end = position + reader.readInt16();
    
    while (position < end)
    {
        short fieldType = reader.readUInt8();
        short fieldSize = reader.readUInt8();
        
        if (fieldType == 1)
        {
            level->setTime(reader.readUInt16());
        }
        else if (fieldType == 2)
        {
            level->setChipsRequired(reader.readUInt16());
        }
        else if (fieldType == 3)
        {
            std::string title(fieldSize, '\0');
            reader.read(&title[0], fieldSize);
            level->setTitle(title);
        }
        else if (fieldType == 4)
        {
            int trapCount = fieldSize / 10;
            for (int i = 0; i < trapCount; i++)
            {
                short wireData[4];
                reader.read((char*)&wireData, sizeof wireData);
                reader.readUInt16();
                
                LevelWire wire;
                wire.startCoordinate = cocos2d::Vec2(wireData[0], wireData[1]);
                wire.endCoordinate = cocos2d::Vec2(wireData[2], wireData[3]);
                level->getWires().push_back(wire);
            }
        }
        else if (fieldType == 5)
        {
            int clonerCount = fieldSize / 8;
            for (int i = 0; i < clonerCount; i++)
            {
                short wireData[4];
                reader.read((char*)&wireData, sizeof wireData);

                LevelWire wire;
                wire.startCoordinate = cocos2d::Vec2(wireData[0], wireData[1]);
                wire.endCoordinate = cocos2d::Vec2(wireData[2], wireData[3]);
                level->getWires().push_back(wire);
            }
        }
        else if (fieldType == 6)
        {
            std::string password(fieldSize, '\0');
            reader.read(&password[0], fieldSize);
            level->setPassword(password);
        }
        else if (fieldType == 7)
        {
            std::string hint(fieldSize, '\0');
            reader.read(&hint[0], fieldSize);
            level->setHint(hint);
        }
        else if (fieldType == 10)
        {
            /*var creatureCount = fieldSize / 2;
             var x = input.getUint8();
             var y = input.getUint8();*/
        }
        
        position += fieldSize + sizeof(unsigned char) + sizeof(unsigned char);
        reader.getBaseStream().seekg(position, std::ios::beg);
    }
    
    stream.close();
    
    return level;
}

const std::vector<LevelPack>& LevelBundle::getLevelPacks() const
{
    return _levelPacks;
}
