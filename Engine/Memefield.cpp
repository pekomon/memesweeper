#include "Memefield.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>

bool MemeField::Tile::HasMeme() const
{
	
	return hasMeme;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;

}

void MemeField::Tile::Draw(const Vei2 screenPos, const bool gameOver, Graphics & gfx) const
{
	if (!gameOver)
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
				SpriteCodex::DrawTile(nNeighbourMemes, screenPos, gfx);
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
	else //game over
	{
		switch (state)
		{
		case MemeField::Tile::State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case MemeField::Tile::State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case MemeField::Tile::State::Opened:
			if (!hasMeme)
			{
				SpriteCodex::DrawTile(nNeighbourMemes, screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		default:
			break;
		}
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

void MemeField::Tile::setNeighbourMemes(int nMemes)
{
	assert(nNeighbourMemes = -1);
	assert(nMemes >= 0 && nMemes <= 8);
	nNeighbourMemes = nMemes;
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

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			if (!TileAt(gridPos).HasMeme())
			{
				TileAt(gridPos).setNeighbourMemes(CountNeighbourMemes(gridPos));
			}
		}

	}

}

void MemeField::Draw(Graphics & gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw( gridPos * SpriteCodex::tileSize, isFailed, gfx);
		}

	}

}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

void MemeField::OnOpenClick(const Vei2 screenCoordinates)
{
	if (!isFailed)
	{
		const Vei2 gridCoords = ScreenCoord2GridCoord(screenCoordinates);

		if (!TileAt(gridCoords).IsOpened() && !TileAt(gridCoords).IsFlagged())
		{
			TileAt(gridCoords).Open();
			if (TileAt(gridCoords).HasMeme())
			{
				isFailed = true;
			}
		}
	}
}

void MemeField::OnFlagClick(const Vei2 screenCoordinates)
{
	if (!isFailed)
	{
		const Vei2 gridCoords = ScreenCoord2GridCoord(screenCoordinates);

		if (!TileAt(gridCoords).IsOpened())
		{
			TileAt(gridCoords).ToggleFlag();
		}
	}
}

int MemeField::CountNeighbourMemes(const Vei2 & gridPosition)
{

	const int startX = std::max(0, gridPosition.x - 1);
	const int endX = std::min(gridPosition.x + 1, width-1);

	const int startY = std::max(0, gridPosition.y - 1);
	const int endY = std::min(gridPosition.y + 1, height-1);

	int memes = 0;
	for (int x = startX; x <= endX; x++)
	{
		for (int y = startY; y <= endY; y++)
		{
			if (TileAt({ x,y }).HasMeme()) 
			{
				memes++;
				assert(memes < 9);
			}
		}
	}
	return memes;
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
