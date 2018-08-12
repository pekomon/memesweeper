#pragma once

#include "Graphics.h"

class MemeField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Opened
		};
	public:
		bool HasMeme() const;
		void SpawnMeme();
		void Draw(const Vei2 screenPos, Graphics& gfx) const;
		void Open();
		bool IsOpened() const;
		void ToggleFlag();
		bool IsFlagged();
		void setNeighbourMemes(int nMemes);
		
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighbourMemes = -1;

	};

public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnOpenClick(const Vei2 screenCoordinates);
	void OnFlagClick(const Vei2 screenCoordinates);
	
private:
	Tile& TileAt(const Vei2& gridposition);
	const Tile& TileAt(const Vei2& gridposition) const;
	Vei2 ScreenCoord2GridCoord(const Vei2 screenCoordinates) const;
	int CountNeighbourMemes(const Vei2& gridPosition);
private:
	static constexpr int width = 14;
	static constexpr int height = 16;

	Tile tiles[width*height];

};