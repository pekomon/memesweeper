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

		
	private:
		State state = State::Hidden;
		bool hasMeme = false;

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
private:
	static constexpr int width = 24;
	static constexpr int height = 20;

	Tile tiles[width*height];

};