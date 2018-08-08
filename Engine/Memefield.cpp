#include "Memefield.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"

bool MemeField::Tile::HasMeme() const
{
	
	return hasMeme;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;

}

void MemeField::Tile::Draw(const Vei2 screenPos, Graphics & gfx) const
{
	switch (state)
	{
	case MemeField::Tile::State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case MemeField::Tile::State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case MemeField::Tile::State::Opened:
		if (!hasMeme)
		{	
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else 
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	default:
		break;
	}
}

void MemeField::Tile::Open()
{
	assert(state == State::Hidden);
	state = State::Opened;
}

bool MemeField::Tile::IsOpened() const
{
	
	return state == State::Opened;
}

void MemeField::Tile::ToggleFlag()
{
	if (state == State::Flagged)
	{
		state = State::Hidden;
	}
	else if (state == State::Hidden)
	{
		state = State::Flagged;
	}
}

bool MemeField::Tile::IsFlagged()
{
	return state == State::Flagged;
}

MemeField::MemeField(int nMemes)
{
	assert(nMemes > 0 && nMemes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDistribution(0, width - 1);
	std::uniform_int_distribution<int> yDistribution(0, height - 1);

	for (int i = 0; i < nMemes; i++) {
		Vei2 coordinates;
		do
		{
			coordinates = { xDistribution(rng), yDistribution(rng) };
			
		} while (TileAt(coordinates).HasMeme());

		TileAt(coordinates).SpawnMeme();
		
	}

}

void MemeField::Draw(Graphics & gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw( gridPos * SpriteCodex::tileSize, gfx);
		}

	}

}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

void MemeField::OnOpenClick(const Vei2 screenCoordinates)
{
	const Vei2 gridCoords = ScreenCoord2GridCoord(screenCoordinates);

	if (!TileAt(gridCoords).IsOpened() && !TileAt(gridCoords).IsFlagged())
	{
		TileAt(gridCoords).Open();
	}
}

void MemeField::OnFlagClick(const Vei2 screenCoordinates)
{
	const Vei2 gridCoords = ScreenCoord2GridCoord(screenCoordinates);
	
	if (!TileAt(gridCoords).IsOpened())
	{
		TileAt(gridCoords).ToggleFlag();
	}
}


MemeField::Tile& MemeField::TileAt(const Vei2& gridposition)
{
	return tiles[gridposition.y * width + gridposition.x];
}


const MemeField::Tile& MemeField::TileAt(const Vei2& gridposition) const
{
	return tiles[gridposition.y * width + gridposition.x];
}

Vei2 MemeField::ScreenCoord2GridCoord(const Vei2 screenCoordinates) const
{
	return screenCoordinates / SpriteCodex::tileSize;
}
