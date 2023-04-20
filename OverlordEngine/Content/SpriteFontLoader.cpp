#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//Parse the Identification bytes (B,M,F)
	auto letter = pReader->Read<char>();
	
	if (letter != 'B') { Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font\n"); return nullptr; }
	letter = pReader->Read<char>();
	if (letter != 'M') { Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font\n"); return nullptr; }
	letter = pReader->Read<char>();
	if (letter != 'F') { Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font\n"); return nullptr; }

	//Parse the version (version 3 required)
	letter = pReader->Read<char>();
	if (letter != 3) { Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported\n"); return nullptr; }

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId = pReader->Read<char>();
	auto blockSize = pReader->Read<int>();
	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = pReader->Read<short>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [fontDesc.fontName]
	fontDesc.fontName = pReader->ReadNullString();
	//...

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<unsigned short>();
	fontDesc.textureHeight = pReader->Read<unsigned short>();
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	if (pReader->Read<unsigned short>() > 1)
	{
		Logger::LogError(LogString{ L"SpriteFontLoader::LoadContent > Only one texture per font is allowed" });
		return nullptr;
	}

	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	//Retrieve the PageName (BinaryReader::ReadNullString)
	const std::wstring pageName{ pReader->ReadNullString() };
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	fontDesc.pTexture = ContentManager::Load<TextureData>(loadInfo.assetFullPath.parent_path().append(pageName).wstring());

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	//Retrieve Character Count (see documentation)
	constexpr int charInfoSize{ 20 };
	const int numChars{ blockSize / charInfoSize };
	//Create loop for Character Count, and:
	for (int i{}; i < numChars; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		const wchar_t charId{ static_cast<wchar_t>(pReader->Read<unsigned int>())};
		//> Create instance of FontMetric (struct)
		FontMetric character{};
		//	> Set Character (CharacterId) [FontMetric::character]
		character.character = charId;
		//	> Retrieve Xposition (store Local)
		const unsigned short xPos{ pReader->Read<unsigned short>() };
		//	> Retrieve Yposition (store Local)
		const unsigned short yPos{ pReader->Read<unsigned short>() };
		//	> Retrieve & Set Width [FontMetric::width]
		character.width = pReader->Read<unsigned short>();
		//	> Retrieve & Set Height [FontMetric::height]
		character.height = pReader->Read<unsigned short>();
		//	> Retrieve & Set OffsetX [FontMetric::offsetX]
		character.offsetX = pReader->Read<unsigned short>();
		//	> Retrieve & Set OffsetY [FontMetric::offsetY]
		character.offsetY = pReader->Read<unsigned short>();
		//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
		character.advanceX = pReader->Read<unsigned short>();
		//	> Retrieve & Set Page [FontMetric::page]
		character.page = pReader->Read<unsigned char>();
		//	> Retrieve Channel (BITFIELD!!!) 
		//		> See documentation for BitField meaning [FontMetrix::channel]
		switch (const unsigned char channelBitField{ pReader->Read<unsigned char>() })
		{
		case 0x1:
			character.channel = 2;
			break;
		case 0x2:
			character.channel = 1;
			break;
		case 0x4:
			character.channel = 0;
			break;
		case 0x8:
			character.channel = 4;
			break;
		case 0xF:
		default:
			character.channel = 0;
		}
		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		character.texCoord = XMFLOAT2
		{
			static_cast<float>(xPos) / fontDesc.textureWidth,
			static_cast<float>(yPos) / fontDesc.textureWidth
		};
		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics[character.character] = character;
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
