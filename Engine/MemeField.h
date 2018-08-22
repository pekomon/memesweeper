#pragma once

#include "Graphics.h"
#include "Sound.h"

class MemeField
{
public:
	enum class State
	{
		Fucked,
		Winrar,
		Memeing
	};
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw( const Vei2& screenPos,MemeField::State fucked,Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount( int memeCount );
		bool HasNeighbourMemes() const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
public:
	MemeField( const Vei2& center,int nMemes );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	State GetState() const;
private:
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos );
	bool GameIsWon() const;
	void RevealTile(const Vei2& gridPos);
private:
	static constexpr int width = 16;
	static constexpr int height = 24;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	Tile field[width * height];
};